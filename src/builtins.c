#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "builtins.h"
#include "object.h"
#include "value.h"
#include "vm.h"

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
        if (!IS_STRING(args[0])) {
            return nativeFail("'oku' fonksiyonunun istem argümanı metin olmalı; %s verildi.",
                              valueTypeName(args[0]));
        }
        printValue(stdout, args[0], false);
    }
    fflush(stdout);

    size_t capacity = 128;
    size_t length = 0;
    char *buffer = (char *)malloc(capacity);
    if (buffer == NULL) return nativeFail("Bellek yetersiz.");

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

/* uzunluk(metin): karakter (Unicode kod noktası) sayısı. */
static bool uzunlukNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'uzunluk' fonksiyonu metin ister; %s verildi.", valueTypeName(args[0]));
    }
    ObjString *string = AS_STRING(args[0]);
    int count = 0;
    for (int i = 0; i < string->length; i++) {
        if (((unsigned char)string->chars[i] & 0xC0) != 0x80) count++;
    }
    *result = NUMBER_VAL(count);
    return true;
}

/* tür(değer): değerin tür adını metin olarak döndürür. */
static bool turNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    const char *name = valueTypeName(args[0]);
    *result = OBJ_VAL(copyString(name, (int)strlen(name)));
    return true;
}

/* saat(): programın kullandığı işlemci süresi, saniye cinsinden. */
static bool saatNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)args;
    *result = NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
    return true;
}

static bool requireNumber(const char *name, Value value) {
    if (IS_NUMBER(value)) return true;
    return nativeFail("'%s' fonksiyonu sayı ister; %s verildi.", name, valueTypeName(value));
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

static bool yuvarlaNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("yuvarla", args[0])) return false;
    *result = NUMBER_VAL(round(AS_NUMBER(args[0])));
    return true;
}

void defineBuiltins(void) {
    defineNative("yaz", -1, yazNative);
    defineNative("oku", -1, okuNative);
    defineNative("metin", 1, metinNative);
    defineNative("sayı", 1, sayiNative);
    defineNative("uzunluk", 1, uzunlukNative);
    defineNative("tür", 1, turNative);
    defineNative("saat", 0, saatNative);
    defineNative("karekök", 1, karekokNative);
    defineNative("mutlak", 1, mutlakNative);
    defineNative("taban", 1, tabanNative);
    defineNative("tavan", 1, tavanNative);
    defineNative("yuvarla", 1, yuvarlaNative);
}
