#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bytes.h"
#include "memory.h"
#include "vm.h"

/* ---- Nesne ---- */

ObjBytes *newBytes(int count) {
    ObjBytes *bytes = (ObjBytes *)allocateObject(sizeof(ObjBytes), OBJ_BYTES);
    bytes->count = 0;
    bytes->capacity = 0;
    bytes->data = NULL;
    if (count > 0) {
        /* Bellek ayırma çöp toplayıcıyı çalıştırabilir; nesneyi yığında koru. */
        push(OBJ_VAL(bytes));
        bytes->data = ALLOCATE(uint8_t, count);
        memset(bytes->data, 0, (size_t)count);
        bytes->capacity = count;
        bytes->count = count;
        pop();
    }
    return bytes;
}

void bytesAppend(ObjBytes *bytes, uint8_t value) {
    if (bytes->capacity < bytes->count + 1) {
        int oldCapacity = bytes->capacity;
        bytes->capacity = GROW_CAPACITY(oldCapacity);
        bytes->data = GROW_ARRAY(uint8_t, bytes->data, oldCapacity, bytes->capacity);
    }
    bytes->data[bytes->count++] = value;
}

bool bytesElement(const char *what, Value value, uint8_t *out) {
    if (!IS_NUMBER(value)) {
        return nativeFail("%s 0 ile 255 arasında bir tam sayı olmalı; %s verildi.", what, valueTypeName(value));
    }
    double number = AS_NUMBER(value);
    if (number != floor(number) || number < 0 || number > 255) {
        char shown[32];
        formatNumber(number, shown, sizeof(shown));
        return nativeFail("%s 0 ile 255 arasında bir tam sayı olmalı; %s verildi.", what, shown);
    }
    *out = (uint8_t)number;
    return true;
}

bool validUtf8(const uint8_t *data, int length) {
    int i = 0;
    while (i < length) {
        uint8_t first = data[i];
        int size;
        uint32_t minimum;
        if (first < 0x80) {
            i++;
            continue;
        } else if ((first & 0xE0) == 0xC0) {
            size = 2;
            minimum = 0x80;
        } else if ((first & 0xF0) == 0xE0) {
            size = 3;
            minimum = 0x800;
        } else if ((first & 0xF8) == 0xF0) {
            size = 4;
            minimum = 0x10000;
        } else {
            return false;
        }
        if (i + size > length) return false;
        uint32_t codePoint = first & (0xFF >> (size + 1));
        for (int k = 1; k < size; k++) {
            if ((data[i + k] & 0xC0) != 0x80) return false;
            codePoint = (codePoint << 6) | (data[i + k] & 0x3F);
        }
        if (codePoint < minimum || codePoint > 0x10FFFF || (codePoint >= 0xD800 && codePoint <= 0xDFFF)) {
            return false;
        }
        i += size;
    }
    return true;
}

/* ---- Yerleşik: baytlar(...) ---- */

static bool requireBytes(const char *name, Value value) {
    if (IS_BYTES(value)) return true;
    return nativeFail("'%s' fonksiyonu baytlar ister; %s verildi.", name, valueTypeName(value));
}

/* baytlar(uzunluk): sıfırlarla dolu dizi; baytlar(liste): listedeki sayılarla dolu dizi. */
static bool baytlarNative(int argCount, Value *args, Value *result) {
    if (argCount > 1) return nativeFail("'baytlar' fonksiyonu en çok 1 argüman alır, %d verildi.", argCount);
    if (argCount == 0) {
        *result = OBJ_VAL(newBytes(0));
        return true;
    }
    if (IS_NUMBER(args[0])) {
        double number = AS_NUMBER(args[0]);
        if (number != floor(number) || number < 0 || number > 1e9) {
            return nativeFail("'baytlar' için uzunluk negatif olmayan bir tam sayı olmalı.");
        }
        *result = OBJ_VAL(newBytes((int)number));
        return true;
    }
    if (IS_LIST(args[0])) {
        ObjList *list = AS_LIST(args[0]);
        ObjBytes *bytes = newBytes(list->count);
        for (int i = 0; i < list->count; i++) {
            if (!bytesElement("Bayt değeri", list->items[i], &bytes->data[i])) return false;
        }
        *result = OBJ_VAL(bytes);
        return true;
    }
    if (IS_BYTES(args[0])) {
        ObjBytes *source = AS_BYTES(args[0]);
        ObjBytes *copy = newBytes(source->count);
        if (source->count > 0) memcpy(copy->data, source->data, (size_t)source->count);
        *result = OBJ_VAL(copy);
        return true;
    }
    return nativeFail("'baytlar' fonksiyonu uzunluk ya da sayı listesi ister; %s verildi.", valueTypeName(args[0]));
}

void defineBytesBuiltins(void) {
    defineNative("baytlar", -1, baytlarNative);
}

/* ---- bayt modülü ---- */

/* bayt.metinden(metin): metnin UTF-8 baytları. */
static bool metindenNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'bayt.metinden' metin ister; %s verildi.", valueTypeName(args[0]));
    }
    ObjString *string = AS_STRING(args[0]);
    ObjBytes *bytes = newBytes(string->length);
    if (string->length > 0) memcpy(bytes->data, string->chars, (size_t)string->length);
    *result = OBJ_VAL(bytes);
    return true;
}

/* bayt.metne(baytlar): baytları UTF-8 metin olarak okur; geçersiz UTF-8 hatadır. */
static bool metneNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireBytes("bayt.metne", args[0])) return false;
    ObjBytes *bytes = AS_BYTES(args[0]);
    if (!validUtf8(bytes->data, bytes->count)) {
        return nativeFail("Baytlar geçerli bir UTF-8 metni değil.");
    }
    *result = OBJ_VAL(copyString((const char *)bytes->data, bytes->count));
    return true;
}

static const char HEX_DIGITS[] = "0123456789abcdef";

/* bayt.onaltılık_kodla(baytlar): her bayt iki küçük harfli onaltılık basamakla. */
static bool onaltilikKodlaNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireBytes("bayt.onaltılık_kodla", args[0])) return false;
    ObjBytes *bytes = AS_BYTES(args[0]);
    char *text = (char *)malloc((size_t)bytes->count * 2 + 1);
    if (text == NULL) return nativeFail("Bellek yetersiz.");
    for (int i = 0; i < bytes->count; i++) {
        text[i * 2] = HEX_DIGITS[bytes->data[i] >> 4];
        text[i * 2 + 1] = HEX_DIGITS[bytes->data[i] & 15];
    }
    *result = OBJ_VAL(copyString(text, bytes->count * 2));
    free(text);
    return true;
}

static int hexValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* bayt.onaltılık_çöz(metin): onaltılık basamak çiftlerini baytlara çevirir; boşluklar yok sayılır. */
static bool onaltilikCozNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'bayt.onaltılık_çöz' metin ister; %s verildi.", valueTypeName(args[0]));
    }
    ObjString *text = AS_STRING(args[0]);
    ObjBytes *bytes = newBytes(0);
    push(OBJ_VAL(bytes));
    int pending = -1;
    for (int i = 0; i < text->length; i++) {
        char c = text->chars[i];
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') continue;
        int digit = hexValue(c);
        if (digit < 0) {
            pop();
            return nativeFail("Onaltılık metinde geçersiz karakter: '%c'.", c);
        }
        if (pending < 0) {
            pending = digit;
        } else {
            bytesAppend(bytes, (uint8_t)(pending * 16 + digit));
            pending = -1;
        }
    }
    pop();
    if (pending >= 0) return nativeFail("Onaltılık metnin basamak sayısı tek; her bayt iki basamakla yazılır.");
    *result = OBJ_VAL(bytes);
    return true;
}

static const char BASE64_DIGITS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* bayt.base64_kodla(baytlar): standart Base64 metni (dolgulu). */
static bool base64KodlaNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireBytes("bayt.base64_kodla", args[0])) return false;
    ObjBytes *bytes = AS_BYTES(args[0]);
    int length = (bytes->count + 2) / 3 * 4;
    char *text = (char *)malloc((size_t)length + 1);
    if (text == NULL) return nativeFail("Bellek yetersiz.");
    int out = 0;
    for (int i = 0; i < bytes->count; i += 3) {
        uint32_t chunk = (uint32_t)bytes->data[i] << 16;
        if (i + 1 < bytes->count) chunk |= (uint32_t)bytes->data[i + 1] << 8;
        if (i + 2 < bytes->count) chunk |= bytes->data[i + 2];
        text[out++] = BASE64_DIGITS[(chunk >> 18) & 63];
        text[out++] = BASE64_DIGITS[(chunk >> 12) & 63];
        text[out++] = i + 1 < bytes->count ? BASE64_DIGITS[(chunk >> 6) & 63] : '=';
        text[out++] = i + 2 < bytes->count ? BASE64_DIGITS[chunk & 63] : '=';
    }
    *result = OBJ_VAL(copyString(text, out));
    free(text);
    return true;
}

static int base64Value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+' || c == '-') return 62;
    if (c == '/' || c == '_') return 63;
    return -1;
}

/* bayt.base64_çöz(metin): Base64 metnini baytlara çevirir; dolgu ve boşluklar isteğe bağlıdır. */
static bool base64CozNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'bayt.base64_çöz' metin ister; %s verildi.", valueTypeName(args[0]));
    }
    ObjString *text = AS_STRING(args[0]);
    ObjBytes *bytes = newBytes(0);
    push(OBJ_VAL(bytes));
    uint32_t accumulator = 0;
    int bits = 0;
    int digits = 0;
    for (int i = 0; i < text->length; i++) {
        char c = text->chars[i];
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') continue;
        if (c == '=') break;
        int value = base64Value(c);
        if (value < 0) {
            pop();
            return nativeFail("Base64 metninde geçersiz karakter: '%c'.", c);
        }
        accumulator = (accumulator << 6) | (uint32_t)value;
        bits += 6;
        digits++;
        if (bits >= 8) {
            bits -= 8;
            bytesAppend(bytes, (uint8_t)((accumulator >> bits) & 255));
            accumulator &= (1u << bits) - 1;
        }
    }
    pop();
    if (digits % 4 == 1) return nativeFail("Base64 metni eksik; uzunluğu 4'ün katı olmalı.");
    *result = OBJ_VAL(bytes);
    return true;
}

void defineBytesModule(void) {
    ObjModule *module = defineModule("bayt");
    moduleDefineNative(module, "metinden", 1, metindenNative);
    moduleDefineNative(module, "metne", 1, metneNative);
    moduleDefineNative(module, "onaltılık_kodla", 1, onaltilikKodlaNative);
    moduleDefineNative(module, "onaltılık_çöz", 1, onaltilikCozNative);
    moduleDefineNative(module, "base64_kodla", 1, base64KodlaNative);
    moduleDefineNative(module, "base64_çöz", 1, base64CozNative);
}
