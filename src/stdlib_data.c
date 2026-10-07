#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "object.h"
#include "stdlib_modules.h"
#include "text.h"
#include "value.h"
#include "vm.h"

/* Standart kütüphanenin veri modülleri: json ve tr. */

#define JSON_MAX_DEPTH 200

/* ---- Büyüyen arabellek ---- */

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} Buffer;

static void bufferReserve(Buffer *buffer, size_t extra) {
    if (buffer->length + extra + 1 <= buffer->capacity) return;
    size_t capacity = buffer->capacity < 64 ? 64 : buffer->capacity;
    while (buffer->length + extra + 1 > capacity) capacity *= 2;
    char *data = (char *)realloc(buffer->data, capacity);
    if (data == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    buffer->data = data;
    buffer->capacity = capacity;
}

static void bufferAppend(Buffer *buffer, const char *text, size_t length) {
    bufferReserve(buffer, length);
    memcpy(buffer->data + buffer->length, text, length);
    buffer->length += length;
}

static void bufferAppendText(Buffer *buffer, const char *text) {
    bufferAppend(buffer, text, strlen(text));
}

/* ---- json.çöz ---- */

typedef struct {
    const char *text;
    int length;
    int position;
    const char *problem; /* ilk hatanın açıklaması */
} JsonParser;

static bool jsonFail(JsonParser *parser, const char *problem) {
    if (parser->problem == NULL) parser->problem = problem;
    return false;
}

static void jsonSkipSpace(JsonParser *parser) {
    while (parser->position < parser->length) {
        char c = parser->text[parser->position];
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') break;
        parser->position++;
    }
}

static bool jsonLiteral(JsonParser *parser, const char *word) {
    int length = (int)strlen(word);
    if (parser->position + length > parser->length ||
        memcmp(parser->text + parser->position, word, (size_t)length) != 0) {
        return false;
    }
    parser->position += length;
    return true;
}

static bool jsonHex4(JsonParser *parser, uint32_t *out) {
    if (parser->position + 4 > parser->length) return false;
    uint32_t value = 0;
    for (int i = 0; i < 4; i++) {
        char c = parser->text[parser->position + i];
        uint32_t digit;
        if (c >= '0' && c <= '9') digit = (uint32_t)(c - '0');
        else if (c >= 'a' && c <= 'f') digit = (uint32_t)(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') digit = (uint32_t)(c - 'A' + 10);
        else return false;
        value = value * 16 + digit;
    }
    parser->position += 4;
    *out = value;
    return true;
}

/* Açılış tırnağından başlayarak bir metni çözer ve yığına koyar. */
static bool jsonString(JsonParser *parser) {
    parser->position++;
    Buffer buffer = {NULL, 0, 0};
    bufferReserve(&buffer, 16);

    for (;;) {
        if (parser->position >= parser->length) {
            free(buffer.data);
            return jsonFail(parser, "metin kapatılmamış");
        }
        char c = parser->text[parser->position++];
        if (c == '"') break;
        if ((unsigned char)c < 0x20) {
            free(buffer.data);
            return jsonFail(parser, "metin içinde denetim karakteri var");
        }
        if (c != '\\') {
            bufferAppend(&buffer, &c, 1);
            continue;
        }

        if (parser->position >= parser->length) {
            free(buffer.data);
            return jsonFail(parser, "metin kapatılmamış");
        }
        char escape = parser->text[parser->position++];
        switch (escape) {
            case '"': bufferAppend(&buffer, "\"", 1); break;
            case '\\': bufferAppend(&buffer, "\\", 1); break;
            case '/': bufferAppend(&buffer, "/", 1); break;
            case 'b': bufferAppend(&buffer, "\b", 1); break;
            case 'f': bufferAppend(&buffer, "\f", 1); break;
            case 'n': bufferAppend(&buffer, "\n", 1); break;
            case 'r': bufferAppend(&buffer, "\r", 1); break;
            case 't': bufferAppend(&buffer, "\t", 1); break;
            case 'u': {
                uint32_t codePoint;
                if (!jsonHex4(parser, &codePoint)) {
                    free(buffer.data);
                    return jsonFail(parser, "geçersiz \\u kaçış dizisi");
                }
                /* UTF-16 vekil çifti: iki \u dizisi tek karakter oluşturur. */
                if (codePoint >= 0xD800 && codePoint <= 0xDBFF &&
                    parser->position + 1 < parser->length &&
                    parser->text[parser->position] == '\\' &&
                    parser->text[parser->position + 1] == 'u') {
                    int saved = parser->position;
                    parser->position += 2;
                    uint32_t low;
                    if (jsonHex4(parser, &low) && low >= 0xDC00 && low <= 0xDFFF) {
                        codePoint = 0x10000 + ((codePoint - 0xD800) << 10) + (low - 0xDC00);
                    } else {
                        parser->position = saved;
                    }
                }
                char encoded[4];
                int size = utf8Encode(codePoint, encoded);
                bufferAppend(&buffer, encoded, (size_t)size);
                break;
            }
            default:
                free(buffer.data);
                return jsonFail(parser, "geçersiz kaçış dizisi");
        }
    }

    push(OBJ_VAL(copyString(buffer.data, (int)buffer.length)));
    free(buffer.data);
    return true;
}

static bool jsonNumber(JsonParser *parser) {
    int start = parser->position;
    const char *text = parser->text;
    int end = parser->length;
    int p = start;

    if (p < end && text[p] == '-') p++;
    int digits = 0;
    while (p < end && text[p] >= '0' && text[p] <= '9') { p++; digits++; }
    if (digits == 0) return jsonFail(parser, "geçersiz sayı");
    if (p < end && text[p] == '.') {
        p++;
        int fraction = 0;
        while (p < end && text[p] >= '0' && text[p] <= '9') { p++; fraction++; }
        if (fraction == 0) return jsonFail(parser, "geçersiz sayı");
    }
    if (p < end && (text[p] == 'e' || text[p] == 'E')) {
        p++;
        if (p < end && (text[p] == '+' || text[p] == '-')) p++;
        int exponent = 0;
        while (p < end && text[p] >= '0' && text[p] <= '9') { p++; exponent++; }
        if (exponent == 0) return jsonFail(parser, "geçersiz sayı");
    }

    char number[64];
    int length = p - start;
    if (length >= (int)sizeof(number)) return jsonFail(parser, "sayı çok uzun");
    memcpy(number, text + start, (size_t)length);
    number[length] = '\0';
    parser->position = p;
    push(NUMBER_VAL(strtod(number, NULL)));
    return true;
}

/* Bir JSON değeri çözer; başarılıysa sonucu yığına koyar. */
static bool jsonValue(JsonParser *parser, int depth) {
    if (depth > JSON_MAX_DEPTH) return jsonFail(parser, "iç içe yapı çok derin");
    jsonSkipSpace(parser);
    if (parser->position >= parser->length) return jsonFail(parser, "değer bekleniyor");

    char c = parser->text[parser->position];
    if (c == '"') return jsonString(parser);
    if (c == '-' || (c >= '0' && c <= '9')) return jsonNumber(parser);
    if (jsonLiteral(parser, "true")) { push(BOOL_VAL(true)); return true; }
    if (jsonLiteral(parser, "false")) { push(BOOL_VAL(false)); return true; }
    if (jsonLiteral(parser, "null")) { push(NIL_VAL); return true; }

    if (c == '[') {
        parser->position++;
        ObjList *list = newList();
        push(OBJ_VAL(list));
        jsonSkipSpace(parser);
        if (parser->position < parser->length && parser->text[parser->position] == ']') {
            parser->position++;
            return true;
        }
        for (;;) {
            if (!jsonValue(parser, depth + 1)) return false;
            listAppend(list, vm.stackTop[-1]);
            pop();
            jsonSkipSpace(parser);
            if (parser->position >= parser->length) return jsonFail(parser, "',' ya da ']' bekleniyor");
            char next = parser->text[parser->position++];
            if (next == ']') return true;
            if (next != ',') return jsonFail(parser, "',' ya da ']' bekleniyor");
        }
    }

    if (c == '{') {
        parser->position++;
        ObjMap *map = newMap();
        push(OBJ_VAL(map));
        jsonSkipSpace(parser);
        if (parser->position < parser->length && parser->text[parser->position] == '}') {
            parser->position++;
            return true;
        }
        for (;;) {
            jsonSkipSpace(parser);
            if (parser->position >= parser->length || parser->text[parser->position] != '"') {
                return jsonFail(parser, "anahtar olarak metin bekleniyor");
            }
            if (!jsonString(parser)) return false;
            jsonSkipSpace(parser);
            if (parser->position >= parser->length || parser->text[parser->position] != ':') {
                return jsonFail(parser, "anahtardan sonra ':' bekleniyor");
            }
            parser->position++;
            if (!jsonValue(parser, depth + 1)) return false;
            mapSet(map, vm.stackTop[-2], vm.stackTop[-1]);
            pop();
            pop();
            jsonSkipSpace(parser);
            if (parser->position >= parser->length) return jsonFail(parser, "',' ya da '}' bekleniyor");
            char next = parser->text[parser->position++];
            if (next == '}') return true;
            if (next != ',') return jsonFail(parser, "',' ya da '}' bekleniyor");
        }
    }

    return jsonFail(parser, "beklenmeyen karakter");
}

/* json.çöz(metin): JSON metnini JUS değerine çevirir. */
static bool jsonCozNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'json.çöz' fonksiyonu metin ister; %s verildi.", valueTypeName(args[0]));
    }

    JsonParser parser;
    parser.text = AS_STRING(args[0])->chars;
    parser.length = AS_STRING(args[0])->length;
    parser.position = 0;
    parser.problem = NULL;

    Value *base = vm.stackTop;
    bool ok = jsonValue(&parser, 0);
    if (ok) {
        jsonSkipSpace(&parser);
        if (parser.position != parser.length) ok = jsonFail(&parser, "değerden sonra fazladan içerik var");
    }
    if (!ok) {
        vm.stackTop = base;
        int character = utf8Length(parser.text, parser.position > parser.length ? parser.length
                                                                               : parser.position);
        return nativeFail("Geçersiz JSON (%d. karakter): %s.", character + 1, parser.problem);
    }

    *result = vm.stackTop[-1];
    vm.stackTop = base;
    return true;
}

/* ---- json.yaz ---- */

static void jsonWriteString(Buffer *buffer, ObjString *string) {
    bufferAppend(buffer, "\"", 1);
    for (int i = 0; i < string->length; i++) {
        unsigned char c = (unsigned char)string->chars[i];
        switch (c) {
            case '"': bufferAppend(buffer, "\\\"", 2); break;
            case '\\': bufferAppend(buffer, "\\\\", 2); break;
            case '\n': bufferAppend(buffer, "\\n", 2); break;
            case '\r': bufferAppend(buffer, "\\r", 2); break;
            case '\t': bufferAppend(buffer, "\\t", 2); break;
            default:
                if (c < 0x20) {
                    char escaped[8];
                    int length = snprintf(escaped, sizeof(escaped), "\\u%04x", c);
                    bufferAppend(buffer, escaped, (size_t)length);
                } else {
                    bufferAppend(buffer, (const char *)&c, 1);
                }
                break;
        }
    }
    bufferAppend(buffer, "\"", 1);
}

static void jsonNewline(Buffer *buffer, int indent, int depth) {
    if (indent <= 0) return;
    bufferAppend(buffer, "\n", 1);
    for (int i = 0; i < indent * depth; i++) bufferAppend(buffer, " ", 1);
}

static bool jsonWrite(Buffer *buffer, Value value, int indent, int depth) {
    if (depth > JSON_MAX_DEPTH) {
        return nativeFail("JSON'a çevrilecek yapı çok derin; kendi kendini içeriyor olabilir.");
    }

    if (IS_NIL(value)) {
        bufferAppendText(buffer, "null");
    } else if (IS_BOOL(value)) {
        bufferAppendText(buffer, AS_BOOL(value) ? "true" : "false");
    } else if (IS_NUMBER(value)) {
        double number = AS_NUMBER(value);
        if (isnan(number) || isinf(number)) {
            return nativeFail("Sonsuz ya da tanımsız sayılar JSON'a çevrilemez.");
        }
        char text[40];
        int length = 0;
        if (number == floor(number) && fabs(number) < 1e15) {
            length = snprintf(text, sizeof(text), "%.0f", number == 0 ? 0.0 : number);
        } else {
            /* Geri okunduğunda aynı sayıyı veren en kısa yazım. */
            for (int precision = 15; precision <= 17; precision++) {
                length = snprintf(text, sizeof(text), "%.*g", precision, number);
                if (strtod(text, NULL) == number) break;
            }
        }
        bufferAppend(buffer, text, (size_t)length);
    } else if (IS_STRING(value)) {
        jsonWriteString(buffer, AS_STRING(value));
    } else if (IS_LIST(value)) {
        ObjList *list = AS_LIST(value);
        bufferAppend(buffer, "[", 1);
        for (int i = 0; i < list->count; i++) {
            if (i > 0) bufferAppend(buffer, ",", 1);
            jsonNewline(buffer, indent, depth + 1);
            if (!jsonWrite(buffer, list->items[i], indent, depth + 1)) return false;
        }
        if (list->count > 0) jsonNewline(buffer, indent, depth);
        bufferAppend(buffer, "]", 1);
    } else if (IS_MAP(value)) {
        ObjMap *map = AS_MAP(value);
        bufferAppend(buffer, "{", 1);
        bool first = true;
        for (int i = 0; i < map->used; i++) {
            if (!map->entries[i].live) continue;
            if (!IS_STRING(map->entries[i].key)) {
                return nativeFail("JSON'a çevrilecek sözlüklerde anahtarlar metin olmalı; %s bulundu.",
                                  valueTypeName(map->entries[i].key));
            }
            if (!first) bufferAppend(buffer, ",", 1);
            first = false;
            jsonNewline(buffer, indent, depth + 1);
            jsonWriteString(buffer, AS_STRING(map->entries[i].key));
            bufferAppendText(buffer, indent > 0 ? ": " : ":");
            if (!jsonWrite(buffer, map->entries[i].value, indent, depth + 1)) return false;
        }
        if (!first) jsonNewline(buffer, indent, depth);
        bufferAppend(buffer, "}", 1);
    } else {
        return nativeFail("%s türündeki değerler JSON'a çevrilemez.", valueTypeName(value));
    }
    return true;
}

/* json.yaz(değer) / json.yaz(değer, girinti): değeri JSON metnine çevirir. */
static bool jsonYazNative(int argCount, Value *args, Value *result) {
    if (argCount < 1 || argCount > 2) {
        return nativeFail("'json.yaz' fonksiyonu 1 ya da 2 argüman alır, %d verildi.", argCount);
    }
    int indent = 0;
    if (argCount == 2) {
        if (!IS_NUMBER(args[1]) || AS_NUMBER(args[1]) < 0 || AS_NUMBER(args[1]) > 16) {
            return nativeFail("'json.yaz' fonksiyonunda girinti 0 ile 16 arasında bir sayı olmalı.");
        }
        indent = (int)AS_NUMBER(args[1]);
    }

    Buffer buffer = {NULL, 0, 0};
    bufferReserve(&buffer, 64);
    if (!jsonWrite(&buffer, args[0], indent, 0)) {
        free(buffer.data);
        return false;
    }
    *result = OBJ_VAL(copyString(buffer.data, (int)buffer.length));
    free(buffer.data);
    return true;
}

/* ---- tr ---- */

static const char *const provinces[] = {
    "Adana", "Adıyaman", "Afyonkarahisar", "Ağrı", "Amasya", "Ankara", "Antalya", "Artvin",
    "Aydın", "Balıkesir", "Bilecik", "Bingöl", "Bitlis", "Bolu", "Burdur", "Bursa", "Çanakkale",
    "Çankırı", "Çorum", "Denizli", "Diyarbakır", "Edirne", "Elazığ", "Erzincan", "Erzurum",
    "Eskişehir", "Gaziantep", "Giresun", "Gümüşhane", "Hakkari", "Hatay", "Isparta", "Mersin",
    "İstanbul", "İzmir", "Kars", "Kastamonu", "Kayseri", "Kırklareli", "Kırşehir", "Kocaeli",
    "Konya", "Kütahya", "Malatya", "Manisa", "Kahramanmaraş", "Mardin", "Muğla", "Muş",
    "Nevşehir", "Niğde", "Ordu", "Rize", "Sakarya", "Samsun", "Siirt", "Sinop", "Sivas",
    "Tekirdağ", "Tokat", "Trabzon", "Tunceli", "Şanlıurfa", "Uşak", "Van", "Yozgat", "Zonguldak",
    "Aksaray", "Bayburt", "Karaman", "Kırıkkale", "Batman", "Şırnak", "Bartın", "Ardahan",
    "Iğdır", "Yalova", "Karabük", "Kilis", "Osmaniye", "Düzce",
};

#define PROVINCE_COUNT ((int)(sizeof(provinces) / sizeof(provinces[0])))

/* Değerin metin karşılığını (sayıysa ondalıksız yazımını) arabelleğe kopyalar. */
static bool textOf(const char *name, Value value, char *out, size_t size) {
    if (IS_STRING(value)) {
        snprintf(out, size, "%s", AS_CSTRING(value));
        return true;
    }
    if (IS_NUMBER(value)) {
        snprintf(out, size, "%.0f", AS_NUMBER(value));
        return true;
    }
    return nativeFail("'%s' fonksiyonu metin ya da sayı ister; %s verildi.", name,
                      valueTypeName(value));
}

/* tr.kimlik_no_geçerli_mi(no): T.C. kimlik numarasının denetim basamaklarını doğrular. */
static bool kimlikNoNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    char text[64];
    if (!textOf("tr.kimlik_no_geçerli_mi", args[0], text, sizeof(text))) return false;

    *result = BOOL_VAL(false);
    if (strlen(text) != 11 || text[0] == '0') return true;
    int digits[11];
    for (int i = 0; i < 11; i++) {
        if (text[i] < '0' || text[i] > '9') return true;
        digits[i] = text[i] - '0';
    }

    int odd = digits[0] + digits[2] + digits[4] + digits[6] + digits[8];
    int even = digits[1] + digits[3] + digits[5] + digits[7];
    int tenth = ((odd * 7 - even) % 10 + 10) % 10;
    int sum = 0;
    for (int i = 0; i < 10; i++) sum += digits[i];

    *result = BOOL_VAL(tenth == digits[9] && sum % 10 == digits[10]);
    return true;
}

/* tr.iban_geçerli_mi(iban): Türkiye IBAN'ının biçimini ve denetim basamaklarını doğrular. */
static bool ibanNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'tr.iban_geçerli_mi' fonksiyonu metin ister; %s verildi.",
                          valueTypeName(args[0]));
    }

    char iban[40];
    int length = 0;
    for (const char *p = AS_CSTRING(args[0]); *p != '\0'; p++) {
        if (*p == ' ') continue;
        if (length >= 34) {
            length = 0;
            break;
        }
        iban[length++] = (*p >= 'a' && *p <= 'z') ? (char)(*p - 32) : *p;
    }

    *result = BOOL_VAL(false);
    if (length != 26 || iban[0] != 'T' || iban[1] != 'R') return true;

    /* İlk dört karakter sona alınır, harfler sayıya çevrilir, 97'ye göre kalan 1 olmalıdır. */
    int remainder = 0;
    for (int i = 0; i < length; i++) {
        char c = iban[(i + 4) % length];
        if (c >= '0' && c <= '9') {
            remainder = (remainder * 10 + (c - '0')) % 97;
        } else if (c >= 'A' && c <= 'Z') {
            remainder = (remainder * 100 + (c - 'A' + 10)) % 97;
        } else {
            return true;
        }
    }
    *result = BOOL_VAL(remainder == 1);
    return true;
}

/* tr.telefon_geçerli_mi(no): Türkiye telefon numarası biçimini doğrular. */
static bool telefonNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'tr.telefon_geçerli_mi' fonksiyonu metin ister; %s verildi.",
                          valueTypeName(args[0]));
    }

    char digits[20];
    int length = 0;
    *result = BOOL_VAL(false);
    for (const char *p = AS_CSTRING(args[0]); *p != '\0'; p++) {
        if (*p == ' ' || *p == '(' || *p == ')' || *p == '-' || *p == '+') continue;
        if (*p < '0' || *p > '9' || length >= 15) return true;
        digits[length++] = *p;
    }

    /* Ülke kodunu (90) ya da baştaki 0'ı at; geriye 10 haneli numara kalmalı. */
    const char *number = digits;
    if (length == 12 && digits[0] == '9' && digits[1] == '0') {
        number += 2;
        length -= 2;
    } else if (length == 11 && digits[0] == '0') {
        number += 1;
        length -= 1;
    }
    *result = BOOL_VAL(length == 10 && number[0] >= '2' && number[0] <= '5');
    return true;
}

/* tr.para(tutar): tutarı Türkiye'de kullanılan biçimde yazar: 1.234,50 ₺ */
static bool paraNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_NUMBER(args[0])) {
        return nativeFail("'tr.para' fonksiyonu sayı ister; %s verildi.", valueTypeName(args[0]));
    }
    double amount = AS_NUMBER(args[0]);
    if (isnan(amount) || isinf(amount) || fabs(amount) >= 1e15) {
        return nativeFail("'tr.para' bu büyüklükteki bir tutarı biçimlendiremez.");
    }

    char plain[40];
    snprintf(plain, sizeof(plain), "%.2f", fabs(amount));
    const char *dot = strchr(plain, '.');
    int wholeLength = (int)(dot - plain);

    char text[80];
    int length = 0;
    if (amount < 0 && strcmp(plain, "0.00") != 0) text[length++] = '-';
    for (int i = 0; i < wholeLength; i++) {
        if (i > 0 && (wholeLength - i) % 3 == 0) text[length++] = '.';
        text[length++] = plain[i];
    }
    text[length++] = ',';
    text[length++] = dot[1];
    text[length++] = dot[2];
    length += snprintf(text + length, sizeof(text) - (size_t)length, " ₺");

    *result = OBJ_VAL(copyString(text, length));
    return true;
}

/* tr.plaka_ili(kod): plaka koduna karşılık gelen il. */
static bool plakaIliNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_NUMBER(args[0])) {
        return nativeFail("'tr.plaka_ili' fonksiyonu sayı ister; %s verildi.", valueTypeName(args[0]));
    }
    double code = AS_NUMBER(args[0]);
    if (code != floor(code) || code < 1 || code > PROVINCE_COUNT) {
        return nativeFail("Plaka kodu 1 ile %d arasında bir tam sayı olmalı.", PROVINCE_COUNT);
    }
    const char *name = provinces[(int)code - 1];
    *result = OBJ_VAL(copyString(name, (int)strlen(name)));
    return true;
}

void defineDataModules(void) {
    ObjModule *json = defineModule("json");
    moduleDefineNative(json, "çöz", 1, jsonCozNative);
    moduleDefineNative(json, "yaz", -1, jsonYazNative);

    ObjModule *tr = defineModule("tr");
    moduleDefineNative(tr, "kimlik_no_geçerli_mi", 1, kimlikNoNative);
    moduleDefineNative(tr, "iban_geçerli_mi", 1, ibanNative);
    moduleDefineNative(tr, "telefon_geçerli_mi", 1, telefonNative);
    moduleDefineNative(tr, "para", 1, paraNative);
    moduleDefineNative(tr, "plaka_ili", 1, plakaIliNative);

    ObjList *list = newList();
    push(OBJ_VAL(list));
    for (int i = 0; i < PROVINCE_COUNT; i++) {
        push(OBJ_VAL(copyString(provinces[i], (int)strlen(provinces[i]))));
        listAppend(list, vm.stackTop[-1]);
        pop();
    }
    moduleDefine(tr, "iller", OBJ_VAL(list));
    pop();
}
