#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "builtins.h"
#include "object.h"
#include "text.h"
#include "value.h"
#include "vm.h"

/*
 * Yerleşik fonksiyonlar. Argümanlar sanal makinenin yığınında durduğu için çöp
 * toplayıcıdan korunur; bir fonksiyon birden çok nesne ayırıyorsa ara nesneleri
 * push/pop ile kendisi korumalıdır.
 */

#define MAX_RANGE_LENGTH 10000000

/* ---- Yardımcılar ---- */

static bool requireNumber(const char *name, Value value) {
    if (IS_NUMBER(value)) return true;
    return nativeFail("'%s' fonksiyonu sayı ister; %s verildi.", name, valueTypeName(value));
}

static bool requireString(const char *name, Value value) {
    if (IS_STRING(value)) return true;
    return nativeFail("'%s' fonksiyonu metin ister; %s verildi.", name, valueTypeName(value));
}

static bool requireList(const char *name, Value value) {
    if (IS_LIST(value)) return true;
    return nativeFail("'%s' fonksiyonu liste ister; %s verildi.", name, valueTypeName(value));
}

static bool requireMap(const char *name, Value value) {
    if (IS_MAP(value)) return true;
    return nativeFail("'%s' fonksiyonu sözlük ister; %s verildi.", name, valueTypeName(value));
}

static bool requireKey(Value key) {
    if (isHashable(key)) return true;
    return nativeFail("Sözlük anahtarı metin, sayı ya da mantıksal olmalı; %s verildi.",
                      valueTypeName(key));
}

static bool toInteger(Value value, int *out) {
    if (!IS_NUMBER(value)) return false;
    double number = AS_NUMBER(value);
    if (number != floor(number) || number < -2147483648.0 || number > 2147483647.0) return false;
    *out = (int)number;
    return true;
}

/* Dizini doğrular; negatif dizinler sondan sayılır. limit, geçerli en büyük dizinin bir fazlasıdır. */
static bool resolveIndex(Value value, int count, int limit, int *out) {
    int index;
    if (!toInteger(value, &index)) {
        return nativeFail("Dizin bir tam sayı olmalı; %s verildi.", valueTypeName(value));
    }
    int resolved = index < 0 ? index + count : index;
    if (resolved < 0 || resolved >= limit) {
        return nativeFail("Dizin sınırların dışında: uzunluk %d, istenen dizin %d.", count, index);
    }
    *out = resolved;
    return true;
}

static void *allocateOrDie(size_t size) {
    void *memory = malloc(size == 0 ? 1 : size);
    if (memory == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    return memory;
}

/* needle'ın haystack içindeki ilk bayt konumu; yoksa -1. */
static int findBytes(const char *haystack, int haystackLength, const char *needle,
                     int needleLength, int from) {
    for (int i = from; i + needleLength <= haystackLength; i++) {
        if (memcmp(haystack + i, needle, (size_t)needleLength) == 0) return i;
    }
    return -1;
}

/* ---- Girdi ve çıktı ---- */

/* yaz(...): argümanları aralarında boşlukla yazar, satırı bitirir. */
static bool yazNative(int argCount, Value *args, Value *result) {
    (void)result;
    for (int i = 0; i < argCount; i++) {
        if (i > 0) fputc(' ', stdout);
        printValue(stdout, args[i], false);
    }
    fputc('\n', stdout);
    return true;
}

/* oku([istem]): standart girdiden bir satır okur; girdi bittiyse boş döndürür. */
static bool okuNative(int argCount, Value *args, Value *result) {
    if (argCount > 1) {
        return nativeFail("'oku' fonksiyonu en çok 1 argüman alır, %d verildi.", argCount);
    }
    if (argCount == 1) {
        if (!requireString("oku", args[0])) return false;
        printValue(stdout, args[0], false);
    }
    fflush(stdout);

    size_t capacity = 128;
    size_t length = 0;
    char *buffer = (char *)allocateOrDie(capacity);

    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *grown = (char *)realloc(buffer, capacity);
            if (grown == NULL) {
                free(buffer);
                return nativeFail("Bellek yetersiz.");
            }
            buffer = grown;
        }
        buffer[length++] = (char)c;
    }

    if (c == EOF && length == 0) {
        free(buffer);
        *result = NIL_VAL;
        return true;
    }
    if (length > 0 && buffer[length - 1] == '\r') length--;

    *result = OBJ_VAL(copyString(buffer, (int)length));
    free(buffer);
    return true;
}

/* ---- Tür dönüşümü ve sorgulama ---- */

/* metin(değer): herhangi bir değeri metne çevirir. */
static bool metinNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    *result = OBJ_VAL(valueToString(args[0]));
    return true;
}

/* "12", "-3.5" gibi onluk yazımları kabul eder; baştaki ve sondaki boşluklar yok sayılır. */
static bool parseDecimal(const char *text, double *out) {
    const char *p = text;
    while (*p == ' ' || *p == '\t') p++;
    const char *start = p;
    if (*p == '+' || *p == '-') p++;

    int digits = 0;
    while (*p >= '0' && *p <= '9') { p++; digits++; }
    if (*p == '.') {
        p++;
        int fraction = 0;
        while (*p >= '0' && *p <= '9') { p++; fraction++; }
        if (fraction == 0) return false;
        digits += fraction;
    }
    if (digits == 0) return false;

    const char *end = p;
    while (*p == ' ' || *p == '\t') p++;
    if (*p != '\0') return false;

    char *parsedEnd;
    *out = strtod(start, &parsedEnd);
    return parsedEnd == end;
}

/* sayı(değer): metni sayıya çevirir; sayı verilirse aynen döndürür. */
static bool sayiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (IS_NUMBER(args[0])) {
        *result = args[0];
        return true;
    }
    if (IS_STRING(args[0])) {
        double number;
        if (!parseDecimal(AS_CSTRING(args[0]), &number)) {
            return nativeFail("\"%.100s\" bir sayıya dönüştürülemez.", AS_CSTRING(args[0]));
        }
        *result = NUMBER_VAL(number);
        return true;
    }
    return nativeFail("'sayı' fonksiyonu metin ya da sayı ister; %s verildi.",
                      valueTypeName(args[0]));
}

/* tür(değer): değerin tür adını metin olarak döndürür. */
static bool turNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    const char *name = valueTypeName(args[0]);
    *result = OBJ_VAL(copyString(name, (int)strlen(name)));
    return true;
}

/* uzunluk(kap): metnin karakter, listenin öğe, sözlüğün anahtar sayısı. */
static bool uzunlukNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (IS_STRING(args[0])) {
        ObjString *string = AS_STRING(args[0]);
        *result = NUMBER_VAL(utf8Length(string->chars, string->length));
    } else if (IS_LIST(args[0])) {
        *result = NUMBER_VAL(AS_LIST(args[0])->count);
    } else if (IS_MAP(args[0])) {
        *result = NUMBER_VAL(AS_MAP(args[0])->count);
    } else {
        return nativeFail("'uzunluk' fonksiyonu metin, liste ya da sözlük ister; %s verildi.",
                          valueTypeName(args[0]));
    }
    return true;
}

/* örneği_mi(değer, sınıf): değer bu sınıfın ya da ondan türeyen bir sınıfın nesnesi mi? */
static bool ornegiMiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_CLASS(args[1])) {
        return nativeFail("'örneği_mi' fonksiyonunun ikinci argümanı sınıf olmalı; %s verildi.",
                          valueTypeName(args[1]));
    }
    bool matches = false;
    if (IS_INSTANCE(args[0])) {
        for (ObjClass *klass = AS_INSTANCE(args[0])->klass; klass != NULL && !matches;
             klass = klass->superclass) {
            matches = klass == AS_CLASS(args[1]);
        }
    }
    *result = BOOL_VAL(matches);
    return true;
}

/* doğrula(koşul) / doğrula(koşul, ileti): koşul doğru değilse hata verir. */
static bool dogrulaNative(int argCount, Value *args, Value *result) {
    (void)result;
    if (argCount < 1 || argCount > 2) {
        return nativeFail("'doğrula' fonksiyonu 1 ya da 2 argüman alır, %d verildi.", argCount);
    }
    if (!IS_BOOL(args[0])) {
        return nativeFail("'doğrula' mantıksal bir değer ister; %s verildi.", valueTypeName(args[0]));
    }
    if (AS_BOOL(args[0])) return true;
    if (argCount == 1) return nativeFail("Doğrulama başarısız.");

    char *message = valueToChars(args[1], false, NULL);
    nativeFail("Doğrulama başarısız: %.400s", message);
    free(message);
    return false;
}

/* eşit_olmalı(bulunan, beklenen): iki değer eşit değilse ikisini de gösteren bir hata verir. */
static bool esitOlmaliNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (valuesEqual(args[0], args[1])) return true;

    char *found = valueToChars(args[0], true, NULL);
    char *expected = valueToChars(args[1], true, NULL);
    nativeFail("Beklenen %.200s, bulunan %.200s.", expected, found);
    free(found);
    free(expected);
    return false;
}

/* ---- Sayılar ---- */

/* saat(): programın kullandığı işlemci süresi, saniye cinsinden. */
static bool saatNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)args;
    *result = NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
    return true;
}

static bool karekokNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("karekök", args[0])) return false;
    if (AS_NUMBER(args[0]) < 0) return nativeFail("Negatif bir sayının karekökü alınamaz.");
    *result = NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
    return true;
}

static bool mutlakNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("mutlak", args[0])) return false;
    *result = NUMBER_VAL(fabs(AS_NUMBER(args[0])));
    return true;
}

static bool tabanNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("taban", args[0])) return false;
    *result = NUMBER_VAL(floor(AS_NUMBER(args[0])));
    return true;
}

static bool tavanNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("tavan", args[0])) return false;
    *result = NUMBER_VAL(ceil(AS_NUMBER(args[0])));
    return true;
}

/* Basamak sayısı argümanını doğrular (0-15). */
static bool toDigits(const char *name, Value value, int *digits) {
    if (!toInteger(value, digits) || *digits < 0 || *digits > 15) {
        return nativeFail("'%s' için basamak sayısı 0 ile 15 arasında bir tam sayı olmalı.", name);
    }
    return true;
}

/* yuvarla(x) / yuvarla(x, basamak): en yakın tam sayıya ya da verilen ondalık basamağa yuvarlar. */
static bool yuvarlaNative(int argCount, Value *args, Value *result) {
    if (argCount < 1 || argCount > 2) {
        return nativeFail("'yuvarla' fonksiyonu 1 ya da 2 argüman alır, %d verildi.", argCount);
    }
    if (!requireNumber("yuvarla", args[0])) return false;
    if (argCount == 1) {
        *result = NUMBER_VAL(round(AS_NUMBER(args[0])));
        return true;
    }

    int digits = 0;
    if (!toDigits("yuvarla", args[1], &digits)) return false;
    /* Onluk yazım üzerinden yuvarla; ikili kesirlerin yol açtığı sapmaları önler. */
    char text[64];
    snprintf(text, sizeof(text), "%.*f", digits, AS_NUMBER(args[0]));
    *result = NUMBER_VAL(strtod(text, NULL));
    return true;
}

/* biçimle(x, basamak): sayıyı tam olarak verilen sayıda ondalık basamakla metne çevirir. */
static bool bicimleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("biçimle", args[0])) return false;
    int digits = 0;
    if (!toDigits("biçimle", args[1], &digits)) return false;
    if (fabs(AS_NUMBER(args[0])) >= 1e30 || AS_NUMBER(args[0]) != AS_NUMBER(args[0])) {
        return nativeFail("'biçimle' bu büyüklükteki bir sayıyı biçimlendiremez.");
    }
    char text[80];
    int length = snprintf(text, sizeof(text), "%.*f", digits, AS_NUMBER(args[0]));
    *result = OBJ_VAL(copyString(text, length));
    return true;
}

/* aralık(son) / aralık(baş, son) / aralık(baş, son, adım): sayı listesi; son dahil değildir. */
static bool aralikNative(int argCount, Value *args, Value *result) {
    if (argCount < 1 || argCount > 3) {
        return nativeFail("'aralık' fonksiyonu 1, 2 ya da 3 argüman alır, %d verildi.", argCount);
    }
    for (int i = 0; i < argCount; i++) {
        if (!requireNumber("aralık", args[i])) return false;
    }

    double start = argCount == 1 ? 0 : AS_NUMBER(args[0]);
    double end = argCount == 1 ? AS_NUMBER(args[0]) : AS_NUMBER(args[1]);
    double step = argCount == 3 ? AS_NUMBER(args[2]) : 1;
    if (step == 0) return nativeFail("'aralık' fonksiyonunda adım sıfır olamaz.");

    double length = ceil((end - start) / step);
    if (length > MAX_RANGE_LENGTH) {
        return nativeFail("'aralık' en çok %d öğeli liste üretebilir.", MAX_RANGE_LENGTH);
    }

    ObjList *list = newList();
    push(OBJ_VAL(list));
    for (int i = 0; i < length; i++) {
        listAppend(list, NUMBER_VAL(start + i * step));
    }
    pop();

    *result = OBJ_VAL(list);
    return true;
}

/* ---- Listeler ve sözlükler ---- */

/* ekle(liste, öğe): öğeyi listenin sonuna ekler. */
static bool ekleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireList("ekle", args[0])) return false;
    listAppend(AS_LIST(args[0]), args[1]);
    return true;
}

/* araya_ekle(liste, dizin, öğe): öğeyi verilen dizine yerleştirir. */
static bool arayaEkleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireList("araya_ekle", args[0])) return false;
    ObjList *list = AS_LIST(args[0]);
    int index;
    if (!resolveIndex(args[1], list->count, list->count + 1, &index)) return false;
    listInsert(list, index, args[2]);
    return true;
}

/* çıkar(liste): son öğeyi listeden çıkarır ve döndürür. */
static bool cikarNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireList("çıkar", args[0])) return false;
    ObjList *list = AS_LIST(args[0]);
    if (list->count == 0) return nativeFail("Boş listeden öğe çıkarılamaz.");
    *result = listRemove(list, list->count - 1);
    return true;
}

/* sil(liste, dizin) / sil(sözlük, anahtar): öğeyi siler ve döndürür. */
static bool silNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (IS_LIST(args[0])) {
        ObjList *list = AS_LIST(args[0]);
        int index;
        if (!resolveIndex(args[1], list->count, list->count, &index)) return false;
        *result = listRemove(list, index);
        return true;
    }
    if (IS_MAP(args[0])) {
        if (!requireKey(args[1])) return false;
        if (!mapGet(AS_MAP(args[0]), args[1], result)) {
            char *shown = valueToChars(args[1], true, NULL);
            nativeFail("Sözlükte %.150s anahtarı yok.", shown);
            free(shown);
            return false;
        }
        mapDelete(AS_MAP(args[0]), args[1]);
        return true;
    }
    return nativeFail("'sil' fonksiyonu liste ya da sözlük ister; %s verildi.",
                      valueTypeName(args[0]));
}

static int compareKeys(Value a, Value b, bool numbers) {
    if (numbers) {
        double x = AS_NUMBER(a);
        double y = AS_NUMBER(b);
        return (x > y) - (x < y);
    }
    ObjString *x = AS_STRING(a);
    ObjString *y = AS_STRING(b);
    return trCompare(x->chars, x->length, y->chars, y->length);
}

/* Kararlı birleştirmeli sıralama: order, keys içindeki dizinleri tutar. */
static void mergeSort(int *order, int *scratch, int count, const Value *keys, bool numbers) {
    if (count < 2) return;
    int half = count / 2;
    mergeSort(order, scratch, half, keys, numbers);
    mergeSort(order + half, scratch, count - half, keys, numbers);

    int left = 0, right = half, out = 0;
    while (left < half && right < count) {
        if (compareKeys(keys[order[right]], keys[order[left]], numbers) < 0) {
            scratch[out++] = order[right++];
        } else {
            scratch[out++] = order[left++];
        }
    }
    while (left < half) scratch[out++] = order[left++];
    while (right < count) scratch[out++] = order[right++];
    memcpy(order, scratch, sizeof(int) * (size_t)count);
}

/*
 * sırala(liste) / sırala(liste, anahtar): küçükten büyüğe sıralanmış yeni liste.
 * anahtar verilirse her öğe için çağrılır ve öğeler sonuçlarına göre sıralanır.
 * Metinler Türk alfabesine göre sıralanır; eşit öğeler özgün sıralarını korur.
 */
static bool siralaNative(int argCount, Value *args, Value *result) {
    if (argCount < 1 || argCount > 2) {
        return nativeFail("'sırala' fonksiyonu 1 ya da 2 argüman alır, %d verildi.", argCount);
    }
    if (!requireList("sırala", args[0])) return false;
    Value *base = vm.stackTop;

    /* Anahtar fonksiyonu listeyi değiştirebilir; bir kopya üzerinde çalış. */
    ObjList *items = newList();
    push(OBJ_VAL(items));
    for (int i = 0; i < AS_LIST(args[0])->count; i++) {
        listAppend(items, AS_LIST(args[0])->items[i]);
    }

    ObjList *keys = items;
    if (argCount == 2) {
        keys = newList();
        push(OBJ_VAL(keys));
        for (int i = 0; i < items->count; i++) {
            Value key;
            if (!callFunction(args[1], 1, &items->items[i], &key)) return false;
            push(key);
            listAppend(keys, key);
            pop();
        }
    }

    bool allNumbers = true;
    bool allStrings = true;
    for (int i = 0; i < keys->count; i++) {
        if (!IS_NUMBER(keys->items[i])) allNumbers = false;
        if (!IS_STRING(keys->items[i])) allStrings = false;
    }
    if (!allNumbers && !allStrings) {
        vm.stackTop = base;
        return nativeFail(argCount == 2
            ? "'sırala' için anahtar fonksiyonunun sonuçlarının tümü sayı ya da tümü metin olmalı."
            : "'sırala' için listenin tüm öğeleri sayı ya da tüm öğeleri metin olmalı. "
              "Başka türde öğeler için ikinci argüman olarak bir anahtar fonksiyonu verin.");
    }

    int count = items->count;
    int *order = (int *)allocateOrDie(sizeof(int) * (size_t)count * 2);
    for (int i = 0; i < count; i++) order[i] = i;
    mergeSort(order, order + count, count, keys->items, allNumbers);

    ObjList *sorted = newList();
    push(OBJ_VAL(sorted));
    for (int i = 0; i < count; i++) listAppend(sorted, items->items[order[i]]);
    free(order);

    vm.stackTop = base;
    *result = OBJ_VAL(sorted);
    return true;
}

/* eşle(liste, fonksiyon): her öğeye fonksiyonu uygular, sonuçların listesini döndürür. */
static bool esleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireList("eşle", args[0])) return false;
    ObjList *source = AS_LIST(args[0]);
    ObjList *mapped = newList();
    push(OBJ_VAL(mapped));
    for (int i = 0; i < source->count; i++) {
        Value value;
        if (!callFunction(args[1], 1, &source->items[i], &value)) return false;
        push(value);
        listAppend(mapped, value);
        pop();
    }
    pop();
    *result = OBJ_VAL(mapped);
    return true;
}

/* süz(liste, fonksiyon): fonksiyonun doğru döndürdüğü öğelerin listesi. */
static bool suzNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireList("süz", args[0])) return false;
    ObjList *source = AS_LIST(args[0]);
    ObjList *kept = newList();
    push(OBJ_VAL(kept));
    for (int i = 0; i < source->count; i++) {
        Value item = source->items[i];
        Value verdict;
        if (!callFunction(args[1], 1, &item, &verdict)) return false;
        if (!IS_BOOL(verdict)) {
            pop();
            return nativeFail("'süz' için verilen fonksiyon doğru ya da yanlış döndürmeli; %s döndürdü.",
                              valueTypeName(verdict));
        }
        if (AS_BOOL(verdict)) listAppend(kept, item);
    }
    pop();
    *result = OBJ_VAL(kept);
    return true;
}

/* ters(liste) / ters(metin): öğeleri ya da karakterleri ters sırada yeni değer. */
static bool tersNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (IS_LIST(args[0])) {
        ObjList *source = AS_LIST(args[0]);
        ObjList *reversed = newList();
        push(OBJ_VAL(reversed));
        for (int i = source->count - 1; i >= 0; i--) listAppend(reversed, source->items[i]);
        pop();
        *result = OBJ_VAL(reversed);
        return true;
    }
    if (IS_STRING(args[0])) {
        ObjString *source = AS_STRING(args[0]);
        char *buffer = (char *)allocateOrDie((size_t)source->length);
        int offset = 0;
        int end = source->length;
        while (offset < source->length) {
            uint32_t codePoint;
            int size = utf8Decode(source->chars + offset, source->length - offset, &codePoint);
            end -= size;
            memcpy(buffer + end, source->chars + offset, (size_t)size);
            offset += size;
        }
        *result = OBJ_VAL(copyString(buffer, source->length));
        free(buffer);
        return true;
    }
    return nativeFail("'ters' fonksiyonu liste ya da metin ister; %s verildi.",
                      valueTypeName(args[0]));
}

/* anahtarlar(sözlük): anahtarların eklenme sırasıyla listesi. */
static bool anahtarlarNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireMap("anahtarlar", args[0])) return false;
    ObjMap *map = AS_MAP(args[0]);
    ObjList *list = newList();
    push(OBJ_VAL(list));
    for (int i = 0; i < map->used; i++) {
        if (map->entries[i].live) listAppend(list, map->entries[i].key);
    }
    pop();
    *result = OBJ_VAL(list);
    return true;
}

/* değerler(sözlük): değerlerin eklenme sırasıyla listesi. */
static bool degerlerNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireMap("değerler", args[0])) return false;
    ObjMap *map = AS_MAP(args[0]);
    ObjList *list = newList();
    push(OBJ_VAL(list));
    for (int i = 0; i < map->used; i++) {
        if (map->entries[i].live) listAppend(list, map->entries[i].value);
    }
    pop();
    *result = OBJ_VAL(list);
    return true;
}

/* al(sözlük, anahtar, varsayılan): anahtar yoksa hata vermek yerine varsayılanı döndürür. */
static bool alNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireMap("al", args[0])) return false;
    if (!requireKey(args[1])) return false;
    if (!mapGet(AS_MAP(args[0]), args[1], result)) *result = args[2];
    return true;
}

/* ---- Metinler ---- */

/* bul(metin, aranan) / bul(liste, öğe): ilk geçtiği dizin; yoksa -1. */
static bool bulNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (IS_LIST(args[0])) {
        ObjList *list = AS_LIST(args[0]);
        *result = NUMBER_VAL(-1);
        for (int i = 0; i < list->count; i++) {
            if (valuesEqual(list->items[i], args[1])) {
                *result = NUMBER_VAL(i);
                break;
            }
        }
        return true;
    }
    if (IS_STRING(args[0])) {
        if (!requireString("bul", args[1])) return false;
        ObjString *text = AS_STRING(args[0]);
        ObjString *needle = AS_STRING(args[1]);
        int at = findBytes(text->chars, text->length, needle->chars, needle->length, 0);
        *result = NUMBER_VAL(at == -1 ? -1 : utf8Length(text->chars, at));
        return true;
    }
    return nativeFail("'bul' fonksiyonu metin ya da liste ister; %s verildi.",
                      valueTypeName(args[0]));
}

/* böl(metin, ayraç): metni ayraçtan bölerek liste üretir. Boş ayraç karakterlere böler. */
static bool bolNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("böl", args[0]) || !requireString("böl", args[1])) return false;
    ObjString *text = AS_STRING(args[0]);
    ObjString *separator = AS_STRING(args[1]);

    ObjList *parts = newList();
    push(OBJ_VAL(parts));

    int offset = 0;
    if (separator->length == 0) {
        while (offset < text->length) {
            uint32_t codePoint;
            int size = utf8Decode(text->chars + offset, text->length - offset, &codePoint);
            push(OBJ_VAL(copyString(text->chars + offset, size)));
            listAppend(parts, vm.stackTop[-1]);
            pop();
            offset += size;
        }
    } else {
        for (;;) {
            int at = findBytes(text->chars, text->length, separator->chars, separator->length,
                               offset);
            int end = at == -1 ? text->length : at;
            push(OBJ_VAL(copyString(text->chars + offset, end - offset)));
            listAppend(parts, vm.stackTop[-1]);
            pop();
            if (at == -1) break;
            offset = at + separator->length;
        }
    }

    pop();
    *result = OBJ_VAL(parts);
    return true;
}

/* birleştir(liste, ayraç): öğeleri metne çevirip aralarına ayraç koyarak birleştirir. */
static bool birlestirNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireList("birleştir", args[0]) || !requireString("birleştir", args[1])) return false;
    ObjList *list = AS_LIST(args[0]);
    ObjString *separator = AS_STRING(args[1]);

    size_t capacity = 64;
    size_t length = 0;
    char *buffer = (char *)allocateOrDie(capacity);

    for (int i = 0; i < list->count; i++) {
        int itemLength;
        char *item = valueToChars(list->items[i], false, &itemLength);
        size_t needed = length + (size_t)itemLength + (size_t)separator->length + 1;
        if (needed > capacity) {
            while (needed > capacity) capacity *= 2;
            char *grown = (char *)realloc(buffer, capacity);
            if (grown == NULL) {
                free(item);
                free(buffer);
                return nativeFail("Bellek yetersiz.");
            }
            buffer = grown;
        }
        if (i > 0) {
            memcpy(buffer + length, separator->chars, (size_t)separator->length);
            length += (size_t)separator->length;
        }
        memcpy(buffer + length, item, (size_t)itemLength);
        length += (size_t)itemLength;
        free(item);
    }

    *result = OBJ_VAL(copyString(buffer, (int)length));
    free(buffer);
    return true;
}

static bool isBlank(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* kırp(metin): baştaki ve sondaki boşlukları atar. */
static bool kirpNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("kırp", args[0])) return false;
    ObjString *text = AS_STRING(args[0]);
    int start = 0;
    int end = text->length;
    while (start < end && isBlank(text->chars[start])) start++;
    while (end > start && isBlank(text->chars[end - 1])) end--;
    *result = OBJ_VAL(copyString(text->chars + start, end - start));
    return true;
}

/* değiştir(metin, eski, yeni): eski'nin geçtiği her yeri yeni ile değiştirir. */
static bool degistirNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    for (int i = 0; i < 3; i++) {
        if (!requireString("değiştir", args[i])) return false;
    }
    ObjString *text = AS_STRING(args[0]);
    ObjString *from = AS_STRING(args[1]);
    ObjString *to = AS_STRING(args[2]);
    if (from->length == 0) return nativeFail("'değiştir' fonksiyonunda aranan metin boş olamaz.");

    int occurrences = 0;
    for (int at = findBytes(text->chars, text->length, from->chars, from->length, 0); at != -1;
         at = findBytes(text->chars, text->length, from->chars, from->length, at + from->length)) {
        occurrences++;
    }

    size_t size = (size_t)text->length + (size_t)occurrences * (size_t)to->length;
    char *buffer = (char *)allocateOrDie(size);
    int length = 0;
    int offset = 0;
    for (;;) {
        int at = findBytes(text->chars, text->length, from->chars, from->length, offset);
        int end = at == -1 ? text->length : at;
        memcpy(buffer + length, text->chars + offset, (size_t)(end - offset));
        length += end - offset;
        if (at == -1) break;
        memcpy(buffer + length, to->chars, (size_t)to->length);
        length += to->length;
        offset = at + from->length;
    }

    *result = OBJ_VAL(copyString(buffer, length));
    free(buffer);
    return true;
}

static bool mapCase(const char *name, Value value, uint32_t (*convert)(uint32_t), Value *result) {
    if (!requireString(name, value)) return false;
    ObjString *text = AS_STRING(value);

    /* Dönüşüm bayt uzunluğunu değiştirebilir (i -> İ); en kötü durum kadar yer ayır. */
    char *buffer = (char *)allocateOrDie((size_t)text->length * 2 + 4);
    int length = 0;
    int offset = 0;
    while (offset < text->length) {
        uint32_t codePoint;
        int size = utf8Decode(text->chars + offset, text->length - offset, &codePoint);
        if (size == 1 && (unsigned char)text->chars[offset] >= 0x80) {
            /* Geçersiz UTF-8 baytını olduğu gibi aktar. */
            buffer[length++] = text->chars[offset];
        } else {
            length += utf8Encode(convert(codePoint), buffer + length);
        }
        offset += size;
    }

    *result = OBJ_VAL(copyString(buffer, length));
    free(buffer);
    return true;
}

/* Metni verilen genişliğe kadar dolgu karakteriyle tamamlar. */
static bool pad(const char *name, int argCount, Value *args, bool left, Value *result) {
    if (argCount < 2 || argCount > 3) {
        return nativeFail("'%s' fonksiyonu 2 ya da 3 argüman alır, %d verildi.", name, argCount);
    }
    if (!requireString(name, args[0])) return false;
    int width;
    if (!toInteger(args[1], &width) || width < 0 || width > 1000000) {
        return nativeFail("'%s' için genişlik negatif olmayan bir tam sayı olmalı.", name);
    }
    const char *fill = " ";
    int fillLength = 1;
    if (argCount == 3) {
        if (!requireString(name, args[2])) return false;
        fill = AS_CSTRING(args[2]);
        fillLength = AS_STRING(args[2])->length;
        if (utf8Length(fill, fillLength) != 1) {
            return nativeFail("'%s' için dolgu tek bir karakter olmalı.", name);
        }
    }

    ObjString *text = AS_STRING(args[0]);
    int missing = width - utf8Length(text->chars, text->length);
    if (missing <= 0) {
        *result = args[0];
        return true;
    }

    int length = text->length + missing * fillLength;
    char *buffer = (char *)allocateOrDie((size_t)length);
    char *cursor = buffer;
    if (!left) {
        memcpy(cursor, text->chars, (size_t)text->length);
        cursor += text->length;
    }
    for (int i = 0; i < missing; i++) {
        memcpy(cursor, fill, (size_t)fillLength);
        cursor += fillLength;
    }
    if (left) memcpy(cursor, text->chars, (size_t)text->length);

    *result = OBJ_VAL(copyString(buffer, length));
    free(buffer);
    return true;
}

/* sola_doldur(metin, genişlik[, dolgu]): metni sağa yaslar; eksik yer solda doldurulur. */
static bool solaDoldurNative(int argCount, Value *args, Value *result) {
    return pad("sola_doldur", argCount, args, true, result);
}

/* sağa_doldur(metin, genişlik[, dolgu]): metni sola yaslar; eksik yer sağda doldurulur. */
static bool sagaDoldurNative(int argCount, Value *args, Value *result) {
    return pad("sağa_doldur", argCount, args, false, result);
}

/* tekrarla(metin, adet): metni arka arkaya adet kez yazar. */
static bool tekrarlaNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("tekrarla", args[0])) return false;
    int times;
    if (!toInteger(args[1], &times) || times < 0) {
        return nativeFail("'tekrarla' için adet negatif olmayan bir tam sayı olmalı.");
    }
    ObjString *text = AS_STRING(args[0]);
    if ((double)text->length * times > 100000000.0) {
        return nativeFail("'tekrarla' bu kadar uzun bir metin üretemez.");
    }

    int length = text->length * times;
    char *buffer = (char *)allocateOrDie((size_t)length);
    for (int i = 0; i < times; i++) {
        memcpy(buffer + i * text->length, text->chars, (size_t)text->length);
    }
    *result = OBJ_VAL(copyString(buffer, length));
    free(buffer);
    return true;
}

/* büyük_harf(metin): Türkçe kurallarıyla büyük harfe çevirir (i -> İ, ı -> I). */
static bool buyukHarfNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    return mapCase("büyük_harf", args[0], trUpper, result);
}

/* küçük_harf(metin): Türkçe kurallarıyla küçük harfe çevirir (İ -> i, I -> ı). */
static bool kucukHarfNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    return mapCase("küçük_harf", args[0], trLower, result);
}

static bool baslarMiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("başlar_mı", args[0]) || !requireString("başlar_mı", args[1])) return false;
    ObjString *text = AS_STRING(args[0]);
    ObjString *prefix = AS_STRING(args[1]);
    *result = BOOL_VAL(prefix->length <= text->length &&
                       memcmp(text->chars, prefix->chars, (size_t)prefix->length) == 0);
    return true;
}

static bool biterMiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("biter_mi", args[0]) || !requireString("biter_mi", args[1])) return false;
    ObjString *text = AS_STRING(args[0]);
    ObjString *suffix = AS_STRING(args[1]);
    *result = BOOL_VAL(suffix->length <= text->length &&
                       memcmp(text->chars + text->length - suffix->length, suffix->chars,
                              (size_t)suffix->length) == 0);
    return true;
}

void defineBuiltins(void) {
    defineNative("yaz", -1, yazNative);
    defineNative("oku", -1, okuNative);

    defineNative("metin", 1, metinNative);
    defineNative("sayı", 1, sayiNative);
    defineNative("tür", 1, turNative);
    defineNative("uzunluk", 1, uzunlukNative);
    defineNative("örneği_mi", 2, ornegiMiNative);
    defineNative("doğrula", -1, dogrulaNative);
    defineNative("eşit_olmalı", 2, esitOlmaliNative);

    defineNative("saat", 0, saatNative);
    defineNative("karekök", 1, karekokNative);
    defineNative("mutlak", 1, mutlakNative);
    defineNative("taban", 1, tabanNative);
    defineNative("tavan", 1, tavanNative);
    defineNative("yuvarla", -1, yuvarlaNative);
    defineNative("biçimle", 2, bicimleNative);
    defineNative("aralık", -1, aralikNative);

    defineNative("ekle", 2, ekleNative);
    defineNative("araya_ekle", 3, arayaEkleNative);
    defineNative("çıkar", 1, cikarNative);
    defineNative("sil", 2, silNative);
    defineNative("sırala", -1, siralaNative);
    defineNative("eşle", 2, esleNative);
    defineNative("süz", 2, suzNative);
    defineNative("ters", 1, tersNative);
    defineNative("anahtarlar", 1, anahtarlarNative);
    defineNative("değerler", 1, degerlerNative);
    defineNative("al", 3, alNative);

    defineNative("bul", 2, bulNative);
    defineNative("böl", 2, bolNative);
    defineNative("birleştir", 2, birlestirNative);
    defineNative("kırp", 1, kirpNative);
    defineNative("değiştir", 3, degistirNative);
    defineNative("büyük_harf", 1, buyukHarfNative);
    defineNative("küçük_harf", 1, kucukHarfNative);
    defineNative("sola_doldur", -1, solaDoldurNative);
    defineNative("sağa_doldur", -1, sagaDoldurNative);
    defineNative("tekrarla", 2, tekrarlaNative);
    defineNative("başlar_mı", 2, baslarMiNative);
    defineNative("biter_mi", 2, biterMiNative);
}
