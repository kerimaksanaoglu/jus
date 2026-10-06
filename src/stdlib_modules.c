#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "io.h"
#include "object.h"
#include "stdlib_modules.h"
#include "value.h"
#include "vm.h"

/*
 * Standart kütüphane. Her modül 'kullan ad' ile yüklenir ve üyelerine
 * 'ad.üye' biçiminde erişilir.
 */

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

static void defineString(ObjModule *module, const char *name, const char *text) {
    push(OBJ_VAL(copyString(text, (int)strlen(text))));
    moduleDefine(module, name, vm.stackTop[-1]);
    pop();
}

/* ---- matematik ---- */

static bool usNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.üs", args[0]) || !requireNumber("matematik.üs", args[1])) return false;
    *result = NUMBER_VAL(pow(AS_NUMBER(args[0]), AS_NUMBER(args[1])));
    return true;
}

static bool karekokNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.karekök", args[0])) return false;
    if (AS_NUMBER(args[0]) < 0) return nativeFail("Negatif bir sayının karekökü alınamaz.");
    *result = NUMBER_VAL(sqrt(AS_NUMBER(args[0])));
    return true;
}

static bool sinNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.sin", args[0])) return false;
    *result = NUMBER_VAL(sin(AS_NUMBER(args[0])));
    return true;
}

static bool cosNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.cos", args[0])) return false;
    *result = NUMBER_VAL(cos(AS_NUMBER(args[0])));
    return true;
}

static bool tanNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.tan", args[0])) return false;
    *result = NUMBER_VAL(tan(AS_NUMBER(args[0])));
    return true;
}

static bool lnNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.ln", args[0])) return false;
    if (AS_NUMBER(args[0]) <= 0) return nativeFail("Logaritma yalnızca pozitif sayılar için tanımlıdır.");
    *result = NUMBER_VAL(log(AS_NUMBER(args[0])));
    return true;
}

static bool log10Native(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("matematik.log10", args[0])) return false;
    if (AS_NUMBER(args[0]) <= 0) return nativeFail("Logaritma yalnızca pozitif sayılar için tanımlıdır.");
    *result = NUMBER_VAL(log10(AS_NUMBER(args[0])));
    return true;
}

/* Sayıları ya tek bir liste olarak ya da ayrı argümanlar olarak kabul eder. */
static bool numberArguments(const char *name, int argCount, Value *args, Value **items, int *count) {
    if (argCount == 1 && IS_LIST(args[0])) {
        *items = AS_LIST(args[0])->items;
        *count = AS_LIST(args[0])->count;
    } else {
        *items = args;
        *count = argCount;
    }
    for (int i = 0; i < *count; i++) {
        if (!requireNumber(name, (*items)[i])) return false;
    }
    return true;
}

static bool enKucukNative(int argCount, Value *args, Value *result) {
    Value *items;
    int count;
    if (!numberArguments("matematik.en_küçük", argCount, args, &items, &count)) return false;
    if (count == 0) return nativeFail("'matematik.en_küçük' en az bir sayı ister.");
    double best = AS_NUMBER(items[0]);
    for (int i = 1; i < count; i++) {
        if (AS_NUMBER(items[i]) < best) best = AS_NUMBER(items[i]);
    }
    *result = NUMBER_VAL(best);
    return true;
}

static bool enBuyukNative(int argCount, Value *args, Value *result) {
    Value *items;
    int count;
    if (!numberArguments("matematik.en_büyük", argCount, args, &items, &count)) return false;
    if (count == 0) return nativeFail("'matematik.en_büyük' en az bir sayı ister.");
    double best = AS_NUMBER(items[0]);
    for (int i = 1; i < count; i++) {
        if (AS_NUMBER(items[i]) > best) best = AS_NUMBER(items[i]);
    }
    *result = NUMBER_VAL(best);
    return true;
}

static bool toplamNative(int argCount, Value *args, Value *result) {
    Value *items;
    int count;
    if (!numberArguments("matematik.toplam", argCount, args, &items, &count)) return false;
    double sum = 0;
    for (int i = 0; i < count; i++) sum += AS_NUMBER(items[i]);
    *result = NUMBER_VAL(sum);
    return true;
}

/* ---- rastgele ---- */

static uint64_t randomState = 0x9E3779B97F4A7C15ull;

/* xorshift64*: her platformda aynı tohumla aynı diziyi üretir. */
static uint64_t nextRandom(void) {
    randomState ^= randomState >> 12;
    randomState ^= randomState << 25;
    randomState ^= randomState >> 27;
    return randomState * 2685821657736338717ull;
}

static double randomUnit(void) {
    return (double)(nextRandom() >> 11) / 9007199254740992.0;
}

static void seedRandom(uint64_t seed) {
    randomState = seed * 0x9E3779B97F4A7C15ull + 0x1234567ull;
    if (randomState == 0) randomState = 0x9E3779B97F4A7C15ull;
    for (int i = 0; i < 8; i++) nextRandom();
}

/* rastgele.sayı(): 0 ile 1 arasında (1 hariç) ondalıklı sayı. */
static bool rastgeleSayiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)args;
    *result = NUMBER_VAL(randomUnit());
    return true;
}

/* rastgele.tam(alt, üst): alt ve üst dahil tam sayı. */
static bool rastgeleTamNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireNumber("rastgele.tam", args[0]) || !requireNumber("rastgele.tam", args[1])) return false;
    double low = ceil(AS_NUMBER(args[0]));
    double high = floor(AS_NUMBER(args[1]));
    if (high < low) return nativeFail("'rastgele.tam' için üst sınır alt sınırdan küçük olamaz.");
    *result = NUMBER_VAL(low + floor(randomUnit() * (high - low + 1)));
    return true;
}

/* rastgele.seç(liste): listeden rastgele bir öğe. */
static bool rastgeleSecNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireList("rastgele.seç", args[0])) return false;
    ObjList *list = AS_LIST(args[0]);
    if (list->count == 0) return nativeFail("Boş listeden öğe seçilemez.");
    *result = list->items[(int)(randomUnit() * list->count)];
    return true;
}

/* rastgele.karıştır(liste): listenin öğelerini yerinde karıştırır. */
static bool rastgeleKaristirNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireList("rastgele.karıştır", args[0])) return false;
    ObjList *list = AS_LIST(args[0]);
    for (int i = list->count - 1; i > 0; i--) {
        int j = (int)(randomUnit() * (i + 1));
        Value swap = list->items[i];
        list->items[i] = list->items[j];
        list->items[j] = swap;
    }
    return true;
}

/* rastgele.tohum(sayı): aynı tohum aynı rastgele diziyi üretir. */
static bool rastgeleTohumNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireNumber("rastgele.tohum", args[0])) return false;
    seedRandom((uint64_t)(int64_t)AS_NUMBER(args[0]));
    return true;
}

/* ---- zaman ---- */

/* zaman.şimdi(): 1 Ocak 1970'ten bu yana geçen saniye. */
static bool simdiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)args;
#ifdef _WIN32
    FILETIME fileTime;
    GetSystemTimeAsFileTime(&fileTime);
    uint64_t ticks = ((uint64_t)fileTime.dwHighDateTime << 32) | fileTime.dwLowDateTime;
    /* FILETIME 1601'den beri 100 nanosaniyelik adımları sayar. */
    *result = NUMBER_VAL((double)ticks / 1e7 - 11644473600.0);
#else
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    *result = NUMBER_VAL((double)now.tv_sec + (double)now.tv_nsec / 1e9);
#endif
    return true;
}

/* zaman.bekle(saniye): programı verilen süre kadar durdurur. */
static bool bekleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireNumber("zaman.bekle", args[0])) return false;
    double seconds = AS_NUMBER(args[0]);
    if (seconds < 0) return nativeFail("'zaman.bekle' için süre negatif olamaz.");
    fflush(stdout);
#ifdef _WIN32
    Sleep((DWORD)(seconds * 1000));
#else
    struct timespec duration;
    duration.tv_sec = (time_t)seconds;
    duration.tv_nsec = (long)((seconds - (double)duration.tv_sec) * 1e9);
    nanosleep(&duration, NULL);
#endif
    return true;
}

static void setField(ObjMap *map, const char *key, double number) {
    push(OBJ_VAL(copyString(key, (int)strlen(key))));
    mapSet(map, vm.stackTop[-1], NUMBER_VAL(number));
    pop();
}

/* zaman.tarih(): yerel tarih ve saat, sözlük olarak. */
static bool tarihNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)args;
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    if (local == NULL) return nativeFail("Sistem saati okunamadı.");

    ObjMap *map = newMap();
    push(OBJ_VAL(map));
    setField(map, "yıl", local->tm_year + 1900);
    setField(map, "ay", local->tm_mon + 1);
    setField(map, "gün", local->tm_mday);
    setField(map, "saat", local->tm_hour);
    setField(map, "dakika", local->tm_min);
    setField(map, "saniye", local->tm_sec);
    /* 1: Pazartesi ... 7: Pazar */
    setField(map, "haftanın_günü", local->tm_wday == 0 ? 7 : local->tm_wday);
    pop();

    *result = OBJ_VAL(map);
    return true;
}

/* ---- dosya ---- */

/* dosya.oku(yol): dosyanın tüm içeriği, metin olarak. */
static bool dosyaOkuNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("dosya.oku", args[0])) return false;
    const char *problem = NULL;
    char *content = readSource(AS_CSTRING(args[0]), &problem);
    if (content == NULL) {
        return nativeFail("'%.200s' okunamadı: %s.", AS_CSTRING(args[0]), problem);
    }
    *result = OBJ_VAL(copyString(content, (int)strlen(content)));
    free(content);
    return true;
}

static bool writeFile(const char *name, const char *mode, Value *args) {
    if (!requireString(name, args[0]) || !requireString(name, args[1])) return false;
    FILE *file = openFile(AS_CSTRING(args[0]), mode);
    if (file == NULL) return nativeFail("'%.200s' yazmak için açılamadı.", AS_CSTRING(args[0]));
    ObjString *content = AS_STRING(args[1]);
    size_t written = fwrite(content->chars, 1, (size_t)content->length, file);
    bool closed = fclose(file) == 0;
    if (written != (size_t)content->length || !closed) {
        return nativeFail("'%.200s' dosyasına yazılamadı.", AS_CSTRING(args[0]));
    }
    return true;
}

/* dosya.yaz(yol, içerik): dosyayı içerikle oluşturur; varsa üzerine yazar. */
static bool dosyaYazNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    return writeFile("dosya.yaz", "wb", args);
}

/* dosya.ekle(yol, içerik): içeriği dosyanın sonuna ekler. */
static bool dosyaEkleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    return writeFile("dosya.ekle", "ab", args);
}

/* dosya.satırlar(yol): dosyanın satırları, liste olarak (satır sonları atılmış). */
static bool dosyaSatirlarNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("dosya.satırlar", args[0])) return false;
    const char *problem = NULL;
    char *content = readSource(AS_CSTRING(args[0]), &problem);
    if (content == NULL) {
        return nativeFail("'%.200s' okunamadı: %s.", AS_CSTRING(args[0]), problem);
    }

    ObjList *lines = newList();
    push(OBJ_VAL(lines));
    const char *start = content;
    while (*start != '\0') {
        const char *end = strchr(start, '\n');
        const char *next = end == NULL ? start + strlen(start) : end + 1;
        if (end == NULL) end = next;
        int length = (int)(end - start);
        if (length > 0 && start[length - 1] == '\r') length--;
        push(OBJ_VAL(copyString(start, length)));
        listAppend(lines, vm.stackTop[-1]);
        pop();
        start = next;
    }
    pop();
    free(content);

    *result = OBJ_VAL(lines);
    return true;
}

/* dosya.var_mı(yol) */
static bool dosyaVarMiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("dosya.var_mı", args[0])) return false;
    FILE *file = openFile(AS_CSTRING(args[0]), "rb");
    if (file != NULL) fclose(file);
    *result = BOOL_VAL(file != NULL);
    return true;
}

/* dosya.sil(yol) */
static bool dosyaSilNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireString("dosya.sil", args[0])) return false;
    if (!removeFile(AS_CSTRING(args[0]))) {
        return nativeFail("'%.200s' silinemedi.", AS_CSTRING(args[0]));
    }
    return true;
}

/* dosya.klasör_oluştur(yol) */
static bool klasorOlusturNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireString("dosya.klasör_oluştur", args[0])) return false;
    if (!makeDirectory(AS_CSTRING(args[0]))) {
        return nativeFail("'%.200s' klasörü oluşturulamadı.", AS_CSTRING(args[0]));
    }
    return true;
}

/* dosya.klasör_sil(yol): boş bir klasörü siler. */
static bool klasorSilNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    if (!requireString("dosya.klasör_sil", args[0])) return false;
    if (!removeDirectory(AS_CSTRING(args[0]))) {
        return nativeFail("'%.200s' klasörü silinemedi; klasör boş olmalıdır.", AS_CSTRING(args[0]));
    }
    return true;
}

static void appendName(const char *name, void *context) {
    ObjList *list = (ObjList *)context;
    push(OBJ_VAL(copyString(name, (int)strlen(name))));
    listAppend(list, vm.stackTop[-1]);
    pop();
}

/* dosya.listele(yol): klasördeki dosya ve klasör adları; sıra belirsizdir. */
static bool listeleNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("dosya.listele", args[0])) return false;
    ObjList *list = newList();
    push(OBJ_VAL(list));
    bool listed = listDirectory(AS_CSTRING(args[0]), appendName, list);
    pop();
    if (!listed) return nativeFail("'%.200s' klasörü okunamadı.", AS_CSTRING(args[0]));
    *result = OBJ_VAL(list);
    return true;
}

/* ---- sistem ---- */

/* sistem.hata_yaz(...): yaz gibi, ama standart hata çıktısına yazar. */
static bool hataYazNative(int argCount, Value *args, Value *result) {
    (void)result;
    fflush(stdout);
    for (int i = 0; i < argCount; i++) {
        if (i > 0) fputc(' ', stderr);
        printValue(stderr, args[i], false);
    }
    fputc('\n', stderr);
    return true;
}

/* sistem.çık(kod): programı verilen çıkış koduyla sonlandırır. */
static bool cikNative(int argCount, Value *args, Value *result) {
    (void)result;
    if (argCount > 1) return nativeFail("'sistem.çık' en çok 1 argüman alır, %d verildi.", argCount);
    int code = 0;
    if (argCount == 1) {
        if (!requireNumber("sistem.çık", args[0])) return false;
        code = (int)AS_NUMBER(args[0]);
    }
    fflush(stdout);
    exit(code);
}

/* sistem.ortam(ad): ortam değişkeninin değeri; tanımlı değilse boş. */
static bool ortamNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!requireString("sistem.ortam", args[0])) return false;
    const char *value = getenv(AS_CSTRING(args[0]));
    *result = value == NULL ? NIL_VAL : OBJ_VAL(copyString(value, (int)strlen(value)));
    return true;
}



void defineStandardModules(void) {
    ObjModule *matematik = defineModule("matematik");
    moduleDefine(matematik, "pi", NUMBER_VAL(3.14159265358979323846));
    moduleDefine(matematik, "e", NUMBER_VAL(2.71828182845904523536));
    moduleDefineNative(matematik, "üs", 2, usNative);
    moduleDefineNative(matematik, "karekök", 1, karekokNative);
    moduleDefineNative(matematik, "sin", 1, sinNative);
    moduleDefineNative(matematik, "cos", 1, cosNative);
    moduleDefineNative(matematik, "tan", 1, tanNative);
    moduleDefineNative(matematik, "ln", 1, lnNative);
    moduleDefineNative(matematik, "log10", 1, log10Native);
    moduleDefineNative(matematik, "en_küçük", -1, enKucukNative);
    moduleDefineNative(matematik, "en_büyük", -1, enBuyukNative);
    moduleDefineNative(matematik, "toplam", -1, toplamNative);

    seedRandom((uint64_t)time(NULL) ^ ((uint64_t)clock() << 20));
    ObjModule *rastgele = defineModule("rastgele");
    moduleDefineNative(rastgele, "sayı", 0, rastgeleSayiNative);
    moduleDefineNative(rastgele, "tam", 2, rastgeleTamNative);
    moduleDefineNative(rastgele, "seç", 1, rastgeleSecNative);
    moduleDefineNative(rastgele, "karıştır", 1, rastgeleKaristirNative);
    moduleDefineNative(rastgele, "tohum", 1, rastgeleTohumNative);

    ObjModule *zaman = defineModule("zaman");
    moduleDefineNative(zaman, "şimdi", 0, simdiNative);
    moduleDefineNative(zaman, "bekle", 1, bekleNative);
    moduleDefineNative(zaman, "tarih", 0, tarihNative);

    ObjModule *dosya = defineModule("dosya");
    moduleDefineNative(dosya, "oku", 1, dosyaOkuNative);
    moduleDefineNative(dosya, "yaz", 2, dosyaYazNative);
    moduleDefineNative(dosya, "ekle", 2, dosyaEkleNative);
    moduleDefineNative(dosya, "satırlar", 1, dosyaSatirlarNative);
    moduleDefineNative(dosya, "var_mı", 1, dosyaVarMiNative);
    moduleDefineNative(dosya, "sil", 1, dosyaSilNative);
    moduleDefineNative(dosya, "klasör_oluştur", 1, klasorOlusturNative);
    moduleDefineNative(dosya, "klasör_sil", 1, klasorSilNative);
    moduleDefineNative(dosya, "listele", 1, listeleNative);

    ObjModule *sistem = defineModule("sistem");
    moduleDefineNative(sistem, "çık", -1, cikNative);
    moduleDefineNative(sistem, "ortam", 1, ortamNative);
    moduleDefineNative(sistem, "hata_yaz", -1, hataYazNative);
    defineString(sistem, "betik", "");
    defineString(sistem, "betik_klasörü", "");
#if defined(_WIN32)
    defineString(sistem, "platform", "windows");
#elif defined(__APPLE__)
    defineString(sistem, "platform", "macos");
#else
    defineString(sistem, "platform", "linux");
#endif
    setScriptArguments(0, NULL);

    defineDataModules();
    defineNetworkModule();
}

void setScriptPath(const char *path) {
    ObjModule *sistem = defineModule("sistem");
    defineString(sistem, "betik", path);

    /* Klasör, son yol ayracına kadar olan kısımdır; ayraç yoksa çalışma klasörü ("."). */
    int length = 0;
    for (int i = 0; path[i] != '\0'; i++) {
        if (path[i] == '/' || path[i] == '\\') length = i;
    }
    if (length == 0) {
        defineString(sistem, "betik_klasörü", path[0] == '/' ? "/" : ".");
    } else {
        push(OBJ_VAL(copyString(path, length)));
        moduleDefine(sistem, "betik_klasörü", vm.stackTop[-1]);
        pop();
    }
}

void setScriptArguments(int count, char **arguments) {
    ObjModule *sistem = defineModule("sistem");
    ObjList *list = newList();
    push(OBJ_VAL(list));
    for (int i = 0; i < count; i++) {
        push(OBJ_VAL(copyString(arguments[i], (int)strlen(arguments[i]))));
        listAppend(list, vm.stackTop[-1]);
        pop();
    }
    moduleDefine(sistem, "argümanlar", OBJ_VAL(list));
    pop();
}
