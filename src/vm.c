#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "common.h"
#include "compiler.h"
#include "io.h"
#include "memory.h"
#include "object.h"
#include "stdlib_modules.h"
#include "suggest.h"
#include "text.h"
#include "vm.h"

#define TRACE_LIMIT 10

VM vm;

static void resetStack(void) {
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
    vm.openUpvalues = NULL;
    vm.handlerCount = 0;
    vm.errorPending = false;
    vm.hasThrownValue = false;
    vm.thrown = NIL_VAL;
}

/* Hatayı kaydeder. Yakalanıp yakalanmayacağına handleError karar verir. */
static void runtimeError(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(vm.errorMessage, sizeof(vm.errorMessage), format, args);
    va_end(args);
    vm.hasThrownValue = false;
}

/* Yakalanmayan hatayı çağrı zinciriyle birlikte yazar. */
static void reportError(void) {
    fflush(stdout);

    CallFrame *top = &vm.frames[vm.frameCount - 1];
    size_t instruction = (size_t)(top->ip - top->closure->function->chunk.code - 1);
    ObjModule *topModule = top->closure->function->module;

    if (vm.captureErrors) {
        char *thrown = vm.hasThrownValue ? valueToChars(vm.thrown, false, NULL) : NULL;
        snprintf(vm.capturedError, sizeof(vm.capturedError), "%s:%d: %.500s", topModule->path->chars,
                 top->closure->function->chunk.lines[instruction],
                 thrown != NULL ? thrown : vm.errorMessage);
        free(thrown);
        return;
    }
    fprintf(stderr, "%s:%d: çalışma zamanı hatası: ", topModule->path->chars,
            top->closure->function->chunk.lines[instruction]);
    if (vm.hasThrownValue) {
        printValue(stderr, vm.thrown, false);
    } else {
        fputs(vm.errorMessage, stderr);
    }
    fputs("\n", stderr);

    /* Hata bir fonksiyonun içindeyse çağrı zincirini göster. */
    if (vm.frameCount > 1) {
        int shown = 0;
        for (int i = vm.frameCount - 1; i >= 0; i--) {
            if (shown == TRACE_LIMIT && i > 0) {
                fprintf(stderr, "    ... (%d çağrı daha)\n", i);
                i = 0;
            }
            CallFrame *frame = &vm.frames[i];
            ObjFunction *function = frame->closure->function;
            size_t offset = (size_t)(frame->ip - function->chunk.code - 1);
            int line = function->chunk.lines[offset];
            if (function->name == NULL) {
                fprintf(stderr, "    satır %d, %s", line,
                        function->module == vm.mainModule ? "ana program" : "modülün üst düzeyi");
            } else {
                fprintf(stderr, "    satır %d, '%s' fonksiyonu", line, function->name->chars);
            }
            /* Başka dosyadaki çağrılar için dosya adını da göster. */
            if (function->module != topModule) {
                fprintf(stderr, " (%s)", function->module->path->chars);
            }
            fputc('\n', stderr);
            shown++;
        }
    }
}

bool nativeFail(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(vm.nativeError, sizeof(vm.nativeError), format, args);
    va_end(args);
    return false;
}

void defineNative(const char *name, int arity, NativeFn function) {
    push(OBJ_VAL(copyString(name, (int)strlen(name))));
    push(OBJ_VAL(newNative(name, arity, function)));
    tableSet(&vm.builtins, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
}

ObjModule *defineModule(const char *name) {
    ObjString *key = copyString(name, (int)strlen(name));
    Value existing;
    if (tableGet(&vm.modules, key, &existing)) return AS_MODULE(existing);

    push(OBJ_VAL(key));
    ObjModule *module = newModule(key, key);
    push(OBJ_VAL(module));
    tableSet(&vm.modules, key, OBJ_VAL(module));
    pop();
    pop();
    return module;
}

void moduleDefine(ObjModule *module, const char *name, Value value) {
    push(value);
    push(OBJ_VAL(copyString(name, (int)strlen(name))));
    tableSet(&module->globals, AS_STRING(vm.stackTop[-1]), value);
    pop();
    pop();
}

void moduleDefineNative(ObjModule *module, const char *name, int arity, NativeFn function) {
    moduleDefine(module, name, OBJ_VAL(newNative(name, arity, function)));
}

void initVM(void) {
    vm.stack = (Value *)malloc(sizeof(Value) * STACK_MAX);
    if (vm.stack == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    resetStack();
    vm.objects = NULL;
    vm.bytesAllocated = 0;
    vm.nextGC = 1024 * 1024;

    vm.grayCount = 0;
    vm.grayCapacity = 0;
    vm.grayStack = NULL;

    vm.nativeError[0] = '\0';
    vm.errorMessage[0] = '\0';
    vm.captureErrors = false;
    vm.capturedError[0] = '\0';
    vm.mainModule = NULL;
    vm.initString = NULL;

    initTable(&vm.builtins);
    initTable(&vm.modules);
    initTable(&vm.strings);

    vm.initString = copyString("kur", 3);

    defineBuiltins();
    defineStandardModules();
}

void freeVM(void) {
    freeTable(&vm.builtins);
    freeTable(&vm.modules);
    freeTable(&vm.strings);
    vm.mainModule = NULL;
    vm.initString = NULL;
    freeObjects();
    free(vm.stack);
    vm.stack = NULL;
    vm.stackTop = NULL;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop(void) {
    vm.stackTop--;
    return *vm.stackTop;
}

static Value peek(int distance) {
    return vm.stackTop[-1 - distance];
}

static bool call(ObjClosure *closure, int argCount) {
    if (argCount != closure->function->arity) {
        runtimeError("'%s' fonksiyonu %d argüman bekliyor, %d verildi.",
                     closure->function->name->chars, closure->function->arity, argCount);
        return false;
    }

    /* Her çağrı çerçevesi en çok UINT8_COUNT yığın yuvası kullanır. */
    if (vm.frameCount == FRAMES_MAX || (vm.stackTop - vm.stack) + UINT8_COUNT > STACK_MAX) {
        runtimeError("Yığın taştı; fonksiyon çağrıları çok derine indi (sonsuz özyineleme olabilir).");
        return false;
    }

    CallFrame *frame = &vm.frames[vm.frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frame->slots = vm.stackTop - argCount - 1;
    return true;
}

static bool callValue(Value callee, int argCount) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
            case OBJ_BOUND_METHOD: {
                ObjBoundMethod *bound = AS_BOUND_METHOD(callee);
                vm.stackTop[-argCount - 1] = bound->receiver;
                return call(bound->method, argCount);
            }
            case OBJ_CLASS: {
                ObjClass *klass = AS_CLASS(callee);
                vm.stackTop[-argCount - 1] = OBJ_VAL(newInstance(klass));
                Value initializer;
                if (tableGet(&klass->methods, vm.initString, &initializer)) {
                    return call(AS_CLOSURE(initializer), argCount);
                }
                if (argCount != 0) {
                    runtimeError("'%s' sınıfının 'kur' yöntemi yok; argüman verilemez (%d verildi).",
                                 klass->name->chars, argCount);
                    return false;
                }
                return true;
            }
            case OBJ_CLOSURE:
                return call(AS_CLOSURE(callee), argCount);
            case OBJ_NATIVE: {
                ObjNative *native = AS_NATIVE(callee);
                if (native->arity >= 0 && argCount != native->arity) {
                    runtimeError("'%s' fonksiyonu %d argüman bekliyor, %d verildi.",
                                 native->name, native->arity, argCount);
                    return false;
                }

                Value result = NIL_VAL;
                vm.nativeError[0] = '\0';
                if (!native->function(argCount, vm.stackTop - argCount, &result)) {
                    /* Hata, fonksiyonun çağırdığı JUS kodundan geldiyse iletisi zaten kayıtlıdır. */
                    if (vm.errorPending) {
                        vm.errorPending = false;
                    } else {
                        runtimeError("%s", vm.nativeError);
                    }
                    return false;
                }
                vm.stackTop -= argCount + 1;
                push(result);
                return true;
            }
            default:
                break;
        }
    }
    runtimeError("Yalnızca fonksiyonlar çağrılabilir; bu değerin türü %s.", valueTypeName(callee));
    return false;
}

static ObjUpvalue *captureUpvalue(Value *local) {
    ObjUpvalue *prevUpvalue = NULL;
    ObjUpvalue *upvalue = vm.openUpvalues;
    while (upvalue != NULL && upvalue->location > local) {
        prevUpvalue = upvalue;
        upvalue = upvalue->next;
    }

    if (upvalue != NULL && upvalue->location == local) {
        return upvalue;
    }

    ObjUpvalue *createdUpvalue = newUpvalue(local);
    createdUpvalue->next = upvalue;

    if (prevUpvalue == NULL) {
        vm.openUpvalues = createdUpvalue;
    } else {
        prevUpvalue->next = createdUpvalue;
    }

    return createdUpvalue;
}

static void closeUpvalues(Value *last) {
    while (vm.openUpvalues != NULL && vm.openUpvalues->location >= last) {
        ObjUpvalue *upvalue = vm.openUpvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        vm.openUpvalues = upvalue->next;
    }
}

/*
 * Bekleyen hatayı işler. Etkin bir 'dene' bloğu varsa yığını o bloğun başındaki
 * duruma döndürür, hata değerini yığına koyar ve 'yakala' bloğuna atlar; true
 * döner. Yoksa hatayı yazar ve false döner. stopAt, bu yürütmenin başladığı
 * çağrı derinliğidir (ana program için 0).
 */
static bool handleError(int stopAt) {
    /* Yalnızca bu yürütmenin içinde kurulmuş yakalayıcılar kullanılabilir. */
    bool hasHandler = vm.handlerCount > 0 &&
                      vm.handlers[vm.handlerCount - 1].frameCount > stopAt;
    if (!hasHandler) {
        if (stopAt > 0) {
            /* İç içe yürütme: hatayı, yerleşik fonksiyonu çağıran dış yürütmeye bırak. */
            vm.errorPending = true;
            return false;
        }
        reportError();
        resetStack();
        return false;
    }

    Handler handler = vm.handlers[--vm.handlerCount];
    closeUpvalues(handler.stackTop);
    vm.frameCount = handler.frameCount;
    vm.stackTop = handler.stackTop;
    vm.frames[vm.frameCount - 1].ip = handler.ip;

    if (vm.hasThrownValue) {
        push(vm.thrown);
    } else {
        push(OBJ_VAL(copyString(vm.errorMessage, (int)strlen(vm.errorMessage))));
    }
    vm.hasThrownValue = false;
    vm.thrown = NIL_VAL;
    return true;
}

static void concatenate(void) {
    /* İşlenenler, ayırma sırasında çöp toplayıcıdan korunmak için yığında kalır. */
    ObjString *b = AS_STRING(peek(0));
    ObjString *a = AS_STRING(peek(1));

    int length = a->length + b->length;
    char *chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, (size_t)a->length);
    memcpy(chars + a->length, b->chars, (size_t)b->length);
    chars[length] = '\0';

    ObjString *result = takeString(chars, length);
    pop();
    pop();
    push(OBJ_VAL(result));
}

/* ---- Hata iletilerindeki öneriler ---- */

#define HINT_SIZE 160

static void considerTable(Suggestion *suggestion, Table *table) {
    for (int i = 0; i < table->capacity; i++) {
        ObjString *key = table->entries[i].key;
        if (key != NULL) suggestionConsider(suggestion, key->chars, key->length);
    }
}

/* Tanımsız bir ad için öneri: fonksiyonun yerelleri, modülün adları, yerleşikler, anahtar kelimeler. */
static void hintForName(ObjString *name, char *out) {
    Suggestion suggestion;
    suggestionInit(&suggestion, name->chars, name->length);

    ObjFunction *function = vm.frames[vm.frameCount - 1].closure->function;
    for (int i = 0; i < function->localNames.count; i++) {
        ObjString *local = AS_STRING(function->localNames.values[i]);
        suggestionConsider(&suggestion, local->chars, local->length);
    }
    considerTable(&suggestion, &function->module->globals);
    considerTable(&suggestion, &vm.builtins);
    suggestionConsiderKeywords(&suggestion);
    suggestionSentence(&suggestion, out, HINT_SIZE);
}

/* Bir nesnede bulunamayan üye için öneri: alanlar ve sınıfın yöntemleri. */
static void hintForMember(ObjString *name, Value receiver, ObjClass *klass, char *out) {
    Suggestion suggestion;
    suggestionInit(&suggestion, name->chars, name->length);
    if (IS_INSTANCE(receiver)) considerTable(&suggestion, &AS_INSTANCE(receiver)->fields);
    if (klass != NULL) considerTable(&suggestion, &klass->methods);
    out[0] = '\0';
    if (suggestionFound(&suggestion)) suggestionSentence(&suggestion, out, HINT_SIZE);
}

static void hintForTable(ObjString *name, Table *table, char *out) {
    Suggestion suggestion;
    suggestionInit(&suggestion, name->chars, name->length);
    considerTable(&suggestion, table);
    out[0] = '\0';
    if (suggestionFound(&suggestion)) suggestionSentence(&suggestion, out, HINT_SIZE);
}

/*
 * Yerleşik türlerde (liste, metin, sözlük ...) yöntem çağrısı için öneri. Bu
 * türlerin yöntemi yoktur; başka dillerden gelen l.append(x) ya da Türkçe harf
 * kullanılmadan yazılmış l.uzunlugu() gibi çağrılar yerleşik fonksiyona yönlendirilir.
 */
static void hintForBuiltinMethod(ObjString *name, Value receiver, char *out) {
    out[0] = '\0';
    const char *equivalent = foreignEquivalent(name->chars, name->length);
    if (equivalent != NULL) {
        snprintf(out, HINT_SIZE, " JUS'ta bunun karşılığı '%s' yerleşik fonksiyonudur: %s(%s, ...).", equivalent,
                 equivalent, valueTypeName(receiver));
        return;
    }
    Suggestion suggestion;
    suggestionInit(&suggestion, name->chars, name->length);
    considerTable(&suggestion, &vm.builtins);
    if (suggestionFound(&suggestion)) {
        snprintf(out, HINT_SIZE, " '%.*s' yerleşik fonksiyonunu mu demek istediniz? Yerleşik fonksiyonlar "
                 "%.*s(%s, ...) biçiminde çağrılır.", suggestion.bestLength, suggestion.best,
                 suggestion.bestLength, suggestion.best, valueTypeName(receiver));
        return;
    }
    snprintf(out, HINT_SIZE, " Bu türün yöntemi yoktur; uzunluk(%s) gibi yerleşik fonksiyonlar kullanılır.",
             valueTypeName(receiver));
}

/* Sözlükte bulunamayan metin anahtarı için öneri. */
static void hintForKey(Value key, ObjMap *map, char *out) {
    out[0] = '\0';
    if (!IS_STRING(key)) return;
    Suggestion suggestion;
    suggestionInit(&suggestion, AS_STRING(key)->chars, AS_STRING(key)->length);
    for (int i = 0; i < map->used; i++) {
        if (!map->entries[i].live || !IS_STRING(map->entries[i].key)) continue;
        ObjString *candidate = AS_STRING(map->entries[i].key);
        suggestionConsider(&suggestion, candidate->chars, candidate->length);
    }
    if (suggestionFound(&suggestion)) {
        snprintf(out, HINT_SIZE, " \"%.*s\" anahtarı var; onu mu demek istediniz?", suggestion.bestLength,
                 suggestion.best);
    }
}

static bool integerValue(Value value, int *out) {
    if (!IS_NUMBER(value)) return false;
    double number = AS_NUMBER(value);
    if (number != floor(number) || number < -2147483648.0 || number > 2147483647.0) return false;
    *out = (int)number;
    return true;
}

/* Dizini doğrular; negatif dizinler sondan sayılır. Hata durumunda ileti yazar. */
static bool checkIndex(Value indexValue, int count, const char *kind, int *out) {
    int index;
    if (!integerValue(indexValue, &index)) {
        char *shown = valueToChars(indexValue, true, NULL);
        runtimeError("%s dizini bir tam sayı olmalı; %s verildi.", kind, shown);
        free(shown);
        return false;
    }

    int resolved = index < 0 ? index + count : index;
    if (resolved < 0 || resolved >= count) {
        runtimeError("Dizin sınırların dışında: uzunluk %d, istenen dizin %d.", count, index);
        return false;
    }
    *out = resolved;
    return true;
}

static bool checkKey(Value key) {
    if (isHashable(key)) return true;
    runtimeError("Sözlük anahtarı metin, sayı ya da mantıksal olmalı; %s verildi.",
                 valueTypeName(key));
    return false;
}

/* [kap, dizin] -> değer */
static bool getIndex(void) {
    Value container = peek(1);
    Value index = peek(0);
    Value result;

    if (IS_LIST(container)) {
        ObjList *list = AS_LIST(container);
        int i;
        if (!checkIndex(index, list->count, "Liste", &i)) return false;
        result = list->items[i];
    } else if (IS_STRING(container)) {
        ObjString *string = AS_STRING(container);
        int i;
        if (!checkIndex(index, utf8Length(string->chars, string->length), "Metin", &i)) return false;
        int start = utf8Offset(string->chars, string->length, i);
        uint32_t codePoint;
        int size = utf8Decode(string->chars + start, string->length - start, &codePoint);
        result = OBJ_VAL(copyString(string->chars + start, size));
    } else if (IS_MAP(container)) {
        if (!checkKey(index)) return false;
        if (!mapGet(AS_MAP(container), index, &result)) {
            char *shown = valueToChars(index, true, NULL);
            char hint[HINT_SIZE];
            hintForKey(index, AS_MAP(container), hint);
            runtimeError("Sözlükte %s anahtarı yok.%s", shown, hint);
            free(shown);
            return false;
        }
    } else {
        runtimeError("Yalnızca liste, metin ve sözlük dizinlenebilir; %s verildi.",
                     valueTypeName(container));
        return false;
    }

    pop();
    pop();
    push(result);
    return true;
}

/* [kap, dizin, değer] -> değer */
static bool setIndex(void) {
    Value container = peek(2);
    Value index = peek(1);
    Value value = peek(0);

    if (IS_LIST(container)) {
        ObjList *list = AS_LIST(container);
        int i;
        if (!checkIndex(index, list->count, "Liste", &i)) return false;
        list->items[i] = value;
    } else if (IS_MAP(container)) {
        if (!checkKey(index)) return false;
        mapSet(AS_MAP(container), index, value);
    } else if (IS_STRING(container)) {
        runtimeError("Metinler değiştirilemez; yeni bir metin oluşturun.");
        return false;
    } else {
        runtimeError("Yalnızca liste ve sözlük öğelerine değer atanabilir; %s verildi.",
                     valueTypeName(container));
        return false;
    }

    pop();
    pop();
    pop();
    push(value);
    return true;
}

static bool sliceBound(Value bound, int count, int fallback, int *out) {
    if (IS_NIL(bound)) {
        *out = fallback;
        return true;
    }
    int index;
    if (!integerValue(bound, &index)) {
        runtimeError("Dilim sınırları tam sayı olmalı; %s verildi.", valueTypeName(bound));
        return false;
    }
    if (index < 0) index += count;
    if (index < 0) index = 0;
    if (index > count) index = count;
    *out = index;
    return true;
}

/* [kap, baş, son] -> dilim */
static bool slice(void) {
    Value container = peek(2);
    Value result;

    if (IS_LIST(container)) {
        ObjList *list = AS_LIST(container);
        int start, end;
        if (!sliceBound(peek(1), list->count, 0, &start)) return false;
        if (!sliceBound(peek(0), list->count, list->count, &end)) return false;

        ObjList *sliced = newList();
        result = OBJ_VAL(sliced);
        push(result);
        for (int i = start; i < end; i++) {
            listAppend(sliced, list->items[i]);
        }
        pop();
    } else if (IS_STRING(container)) {
        ObjString *string = AS_STRING(container);
        int count = utf8Length(string->chars, string->length);
        int start, end;
        if (!sliceBound(peek(1), count, 0, &start)) return false;
        if (!sliceBound(peek(0), count, count, &end)) return false;
        if (end < start) end = start;

        int startByte = utf8Offset(string->chars, string->length, start);
        int endByte = utf8Offset(string->chars, string->length, end);
        result = OBJ_VAL(copyString(string->chars + startByte, endByte - startByte));
    } else {
        runtimeError("Yalnızca liste ve metinden dilim alınabilir; %s verildi.",
                     valueTypeName(container));
        return false;
    }

    pop();
    pop();
    pop();
    push(result);
    return true;
}

/* [öğe, kap] -> mantıksal */
static bool contains(void) {
    Value item = peek(1);
    Value container = peek(0);
    bool found = false;

    if (IS_LIST(container)) {
        ObjList *list = AS_LIST(container);
        for (int i = 0; i < list->count && !found; i++) {
            found = valuesEqual(list->items[i], item);
        }
    } else if (IS_STRING(container)) {
        if (!IS_STRING(item)) {
            runtimeError("Bir metnin içinde yalnızca metin aranabilir; %s verildi.",
                         valueTypeName(item));
            return false;
        }
        ObjString *haystack = AS_STRING(container);
        ObjString *needle = AS_STRING(item);
        for (int i = 0; i + needle->length <= haystack->length && !found; i++) {
            found = memcmp(haystack->chars + i, needle->chars, (size_t)needle->length) == 0;
        }
    } else if (IS_MAP(container)) {
        Value ignored;
        found = isHashable(item) && mapGet(AS_MAP(container), item, &ignored);
    } else {
        runtimeError("'içinde' işlecinin sağ tarafı liste, metin ya da sözlük olmalı; %s verildi.",
                     valueTypeName(container));
        return false;
    }

    pop();
    pop();
    push(BOOL_VAL(found));
    return true;
}

/*
 * 'her' döngüsünün bir adımı. Yığının tepesinde [kap, imleç] durur. Sıradaki
 * öğe varsa yığına ekler ve *done yanlış olur; kap bittiyse *done doğru olur.
 */
static bool forNext(bool *done) {
    Value container = peek(1);
    int cursor = (int)AS_NUMBER(peek(0));
    *done = false;

    if (IS_LIST(container)) {
        ObjList *list = AS_LIST(container);
        if (cursor >= list->count) {
            *done = true;
            return true;
        }
        vm.stackTop[-1] = NUMBER_VAL(cursor + 1);
        push(list->items[cursor]);
    } else if (IS_STRING(container)) {
        ObjString *string = AS_STRING(container);
        if (cursor >= string->length) {
            *done = true;
            return true;
        }
        uint32_t codePoint;
        int size = utf8Decode(string->chars + cursor, string->length - cursor, &codePoint);
        vm.stackTop[-1] = NUMBER_VAL(cursor + size);
        push(OBJ_VAL(copyString(string->chars + cursor, size)));
    } else if (IS_MAP(container)) {
        ObjMap *map = AS_MAP(container);
        while (cursor < map->used && !map->entries[cursor].live) cursor++;
        if (cursor >= map->used) {
            *done = true;
            return true;
        }
        vm.stackTop[-1] = NUMBER_VAL(cursor + 1);
        push(map->entries[cursor].key);
    } else {
        runtimeError("'her' döngüsü liste, metin ya da sözlük üzerinde gezinir; %s verildi.",
                     valueTypeName(container));
        return false;
    }
    return true;
}

static int directoryLength(ObjString *path) {
    int length = 0;
    for (int i = 0; i < path->length; i++) {
        if (path->chars[i] == '/' || path->chars[i] == '\\') length = i + 1;
    }
    return length;
}

/* Modülün, içe aktaran dosyanın klasöründeki yolu: <klasör>/<ad>.jus */
static ObjString *localModulePath(ObjModule *importer, ObjString *name) {
    int prefix = directoryLength(importer->path);
    int length = prefix + name->length + 4;
    char *path = (char *)malloc((size_t)length + 1);
    if (path == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    memcpy(path, importer->path->chars, (size_t)prefix);
    memcpy(path + prefix, name->chars, (size_t)name->length);
    memcpy(path + prefix + name->length, ".jus", 5);

    ObjString *result = copyString(path, length);
    free(path);
    return result;
}

/*
 * Modülün paket klasöründeki yolu. Paketler ana programın klasöründeki
 * jus_paketleri/ altında durur: 'kullan ad' -> jus_paketleri/ad/ad.jus,
 * 'kullan "ad/modül"' -> jus_paketleri/ad/modül.jus
 */
static ObjString *packageModulePath(ObjString *name) {
    int prefix = vm.mainModule == NULL ? 0 : directoryLength(vm.mainModule->path);
    bool nested = memchr(name->chars, '/', (size_t)name->length) != NULL;
    size_t size = (size_t)prefix + (size_t)name->length * 2 + 32;
    char *path = (char *)malloc(size);
    if (path == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    int length;
    if (nested) {
        length = snprintf(path, size, "%.*sjus_paketleri/%s.jus", prefix,
                          prefix == 0 ? "" : vm.mainModule->path->chars, name->chars);
    } else {
        length = snprintf(path, size, "%.*sjus_paketleri/%s/%s.jus", prefix,
                          prefix == 0 ? "" : vm.mainModule->path->chars, name->chars, name->chars);
    }

    ObjString *result = copyString(path, length);
    free(path);
    return result;
}

/*
 * Modül adını çözer. Sırasıyla yerleşik modüllere, içe aktaran dosyanın
 * klasörüne ve paket klasörüne bakar. Modülün anahtarı (ad ya da dosya yolu)
 * yığına konur; modül zaten yüklüyse *module doldurulur. Yüklü değilse yığında
 * yerel dosya yolu kalır.
 */
static void resolveModule(ObjModule *importer, ObjString *name, Value *module, bool *found) {
    *found = tableGet(&vm.modules, name, module);
    if (*found) {
        push(OBJ_VAL(name));
        return;
    }

    ObjString *local = localModulePath(importer, name);
    push(OBJ_VAL(local));
    *found = tableGet(&vm.modules, local, module);
    if (*found) return;

    ObjString *package = packageModulePath(name);
    if (tableGet(&vm.modules, package, module)) {
        *found = true;
        vm.stackTop[-1] = OBJ_VAL(package);
    }
}

/*
 * 'kullan' deyimini yürütür. Modül ilk kez yükleniyorsa dosyayı derler ve üst
 * düzey kodunu çağırır; çağrı dönünce yığında sonucu (boş) kalır. Modül zaten
 * yüklüyse doğrudan boş koyar.
 */
static bool importModule(ObjModule *importer, ObjString *name) {
    Value existing;
    bool found;
    resolveModule(importer, name, &existing, &found);
    if (found) {
        pop();
        push(NIL_VAL);
        return true;
    }

    ObjString *path = AS_STRING(peek(0));
    /* JUS ile yazılmış standart kütüphane modülleri yorumlayıcının içinde durur. */
    const char *embedded = embeddedModuleSource(name->chars);
    char *source = NULL;
    if (embedded != NULL) {
        pop();
        push(OBJ_VAL(name));
        path = name;
    } else {
        const char *problem = NULL;
        source = readSource(path->chars, &problem);
        if (source == NULL) {
            /* Dosyanın klasöründe yok; kurulu paketlere bak. */
            ObjString *package = packageModulePath(name);
            push(OBJ_VAL(package));
            const char *packageProblem = NULL;
            source = readSource(package->chars, &packageProblem);
            if (source == NULL) {
                runtimeError("'%s' modülü yüklenemedi: '%s' için %s. Kurulu paketlerde de yok ('%s').",
                             name->chars, path->chars, problem, package->chars);
                return false;
            }
            vm.stackTop[-2] = OBJ_VAL(package);
            pop();
            path = package;
        }
    }

    ObjModule *module = newModule(name, path);
    push(OBJ_VAL(module));
    /* Döngüsel 'kullan' zincirleri sonsuza gitmesin diye çalıştırmadan önce kaydet. */
    tableSet(&vm.modules, path, OBJ_VAL(module));

    ObjFunction *function = compile(path->chars, embedded != NULL ? embedded : source, false, module);
    free(source);
    if (function == NULL) {
        tableDelete(&vm.modules, path);
        runtimeError("'%s' modülü sözdizimi hataları nedeniyle yüklenemedi.", name->chars);
        return false;
    }

    push(OBJ_VAL(function));
    ObjClosure *closure = newClosure(function);
    vm.stackTop -= 3;
    push(OBJ_VAL(closure));
    return call(closure, 0);
}

/* Yığının tepesindeki nesneyi, sınıfın verilen adlı yöntemine bağlı yöntemle değiştirir. */
static bool bindMethod(ObjClass *klass, ObjString *name) {
    Value method;
    if (!tableGet(&klass->methods, name, &method)) {
        char hint[HINT_SIZE];
        hintForMember(name, peek(0), klass, hint);
        runtimeError("'%s' nesnesinde '%s' adında bir alan ya da yöntem yok.%s", klass->name->chars,
                     name->chars, hint);
        return false;
    }

    ObjBoundMethod *bound = newBoundMethod(peek(0), AS_CLOSURE(method));
    pop();
    push(OBJ_VAL(bound));
    return true;
}

/* [nesne] -> üye */
static bool getProperty(ObjString *name) {
    Value object = peek(0);
    if (IS_INSTANCE(object)) {
        ObjInstance *instance = AS_INSTANCE(object);
        Value value;
        if (tableGet(&instance->fields, name, &value)) {
            pop();
            push(value);
            return true;
        }
        return bindMethod(instance->klass, name);
    }

    if (IS_MODULE(object)) {
        Value value;
        if (!tableGet(&AS_MODULE(object)->globals, name, &value)) {
            char hint[HINT_SIZE];
            hintForTable(name, &AS_MODULE(object)->globals, hint);
            runtimeError("'%s' modülünde '%s' adında bir üye yok.%s", AS_MODULE(object)->name->chars,
                         name->chars, hint);
            return false;
        }
        pop();
        push(value);
        return true;
    }

    char hint[HINT_SIZE];
    hintForBuiltinMethod(name, object, hint);
    runtimeError("%s türündeki değerlerin '%s' adında bir üyesi yok.%s", valueTypeName(object),
                 name->chars, hint);
    return false;
}

/* [nesne, değer] -> değer */
static bool setProperty(ObjString *name) {
    if (!IS_INSTANCE(peek(1))) {
        runtimeError("Yalnızca nesnelerin alanlarına değer atanabilir; %s verildi.",
                     valueTypeName(peek(1)));
        return false;
    }

    ObjInstance *instance = AS_INSTANCE(peek(1));
    tableSet(&instance->fields, name, peek(0));
    Value value = pop();
    pop();
    push(value);
    return true;
}

static bool invokeFromClass(ObjClass *klass, ObjString *name, int argCount) {
    Value method;
    if (!tableGet(&klass->methods, name, &method)) {
        char hint[HINT_SIZE];
        hintForMember(name, peek(argCount), klass, hint);
        runtimeError("'%s' nesnesinde '%s' adında bir yöntem yok.%s", klass->name->chars, name->chars,
                     hint);
        return false;
    }
    return call(AS_CLOSURE(method), argCount);
}

/* nesne.ad(...) çağrısı; alıcı, argümanların altında yığında durur. */
static bool invoke(ObjString *name, int argCount) {
    Value receiver = peek(argCount);

    if (IS_INSTANCE(receiver)) {
        ObjInstance *instance = AS_INSTANCE(receiver);
        Value value;
        if (tableGet(&instance->fields, name, &value)) {
            vm.stackTop[-argCount - 1] = value;
            return callValue(value, argCount);
        }
        return invokeFromClass(instance->klass, name, argCount);
    }

    if (IS_MODULE(receiver)) {
        Value value;
        if (!tableGet(&AS_MODULE(receiver)->globals, name, &value)) {
            char hint[HINT_SIZE];
            hintForTable(name, &AS_MODULE(receiver)->globals, hint);
            runtimeError("'%s' modülünde '%s' adında bir üye yok.%s", AS_MODULE(receiver)->name->chars,
                         name->chars, hint);
            return false;
        }
        vm.stackTop[-argCount - 1] = value;
        return callValue(value, argCount);
    }

    char hint[HINT_SIZE];
    hintForBuiltinMethod(name, receiver, hint);
    runtimeError("%s türündeki değerlerin '%s' adında bir yöntemi yok.%s", valueTypeName(receiver),
                 name->chars, hint);
    return false;
}

static void concatenateLists(void) {
    ObjList *b = AS_LIST(peek(0));
    ObjList *a = AS_LIST(peek(1));

    ObjList *result = newList();
    push(OBJ_VAL(result));
    for (int i = 0; i < a->count; i++) listAppend(result, a->items[i]);
    for (int i = 0; i < b->count; i++) listAppend(result, b->items[i]);
    pop();

    pop();
    pop();
    push(OBJ_VAL(result));
}

/*
 * Bayt kodunu yürütür. Çağrı derinliği stopAt'e döndüğünde (ana program için
 * program bittiğinde) durur; iç içe yürütmede sonuç yığının tepesinde kalır.
 * script, bir modülün üst düzey kodunun (sonucu atılır) yürütüldüğünü belirtir.
 */
static InterpretResult run(int stopAt, bool script) {
    CallFrame *frame = &vm.frames[vm.frameCount - 1];

#define READ_BYTE() (*frame->ip++)
#define READ_SHORT() (frame->ip += 2, (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_CONSTANT() (frame->closure->function->chunk.constants.values[READ_SHORT()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
#define BINARY_OP(valueType, op, symbol) \
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtimeError("'%s' işleci iki sayı ister; %s ve %s verildi.", symbol, \
                         valueTypeName(peek(1)), valueTypeName(peek(0))); \
            goto on_error; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
    } while (false)
#define COMPARE_OP(op, symbol) \
    do { \
        if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) { \
            double b = AS_NUMBER(pop()); \
            double a = AS_NUMBER(pop()); \
            push(BOOL_VAL(a op b)); \
        } else if (IS_STRING(peek(0)) && IS_STRING(peek(1))) { \
            ObjString *b = AS_STRING(pop()); \
            ObjString *a = AS_STRING(pop()); \
            push(BOOL_VAL(trCompare(a->chars, a->length, b->chars, b->length) op 0)); \
        } else { \
            runtimeError("'%s' işleci iki sayı ya da iki metin ister; %s ve %s verildi.", symbol, \
                         valueTypeName(peek(1)), valueTypeName(peek(0))); \
            goto on_error; \
        } \
    } while (false)

    for (;;) {
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NIL: push(NIL_VAL); break;
            case OP_TRUE: push(BOOL_VAL(true)); break;
            case OP_FALSE: push(BOOL_VAL(false)); break;
            case OP_POP: pop(); break;
            case OP_GET_LOCAL: {
                uint8_t slot = READ_BYTE();
                push(frame->slots[slot]);
                break;
            }
            case OP_SET_LOCAL: {
                uint8_t slot = READ_BYTE();
                frame->slots[slot] = peek(0);
                break;
            }
            case OP_GET_GLOBAL: {
                ObjString *name = READ_STRING();
                Value value;
                if (!tableGet(&frame->closure->function->module->globals, name, &value) &&
                    !tableGet(&vm.builtins, name, &value)) {
                    char hint[HINT_SIZE];
                    hintForName(name, hint);
                    runtimeError("'%s' adında bir değişken ya da fonksiyon tanımlı değil.%s",
                                 name->chars, hint);
                    goto on_error;
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString *name = READ_STRING();
                tableSet(&frame->closure->function->module->globals, name, peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString *name = READ_STRING();
                Table *globals = &frame->closure->function->module->globals;
                if (tableSet(globals, name, peek(0))) {
                    tableDelete(globals, name);
                    runtimeError("'%s' adında bir değişken tanımlı değil. Yeni değişken için "
                                 "'değişken %s = ...' yazın.", name->chars, name->chars);
                    goto on_error;
                }
                break;
            }
            case OP_GET_UPVALUE: {
                uint8_t slot = READ_BYTE();
                push(*frame->closure->upvalues[slot]->location);
                break;
            }
            case OP_SET_UPVALUE: {
                uint8_t slot = READ_BYTE();
                *frame->closure->upvalues[slot]->location = peek(0);
                break;
            }
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_NOT_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(!valuesEqual(a, b)));
                break;
            }
            case OP_GREATER: COMPARE_OP(>, ">"); break;
            case OP_GREATER_EQUAL: COMPARE_OP(>=, ">="); break;
            case OP_LESS: COMPARE_OP(<, "<"); break;
            case OP_LESS_EQUAL: COMPARE_OP(<=, "<="); break;
            case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                } else if (IS_LIST(peek(0)) && IS_LIST(peek(1))) {
                    concatenateLists();
                } else {
                    runtimeError("'+' işleci iki sayı, iki metin ya da iki liste ister; %s ve %s verildi. "
                                 "Dönüştürmek için metin() ya da sayı() kullanılabilir.",
                                 valueTypeName(peek(1)), valueTypeName(peek(0)));
                    goto on_error;
                }
                break;
            }
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -, "-"); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *, "*"); break;
            case OP_DIVIDE: {
                if (IS_NUMBER(peek(0)) && AS_NUMBER(peek(0)) == 0 && IS_NUMBER(peek(1))) {
                    runtimeError("Sıfıra bölünemez.");
                    goto on_error;
                }
                BINARY_OP(NUMBER_VAL, /, "/");
                break;
            }
            case OP_MODULO: {
                if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
                    runtimeError("'%%' işleci iki sayı ister; %s ve %s verildi.",
                                 valueTypeName(peek(1)), valueTypeName(peek(0)));
                    goto on_error;
                }
                if (AS_NUMBER(peek(0)) == 0) {
                    runtimeError("Sıfıra göre kalan hesaplanamaz.");
                    goto on_error;
                }
                double b = AS_NUMBER(pop());
                double a = AS_NUMBER(pop());
                /* Sonuç bölenin işaretini taşır: -7 % 3 == 2 */
                double result = fmod(a, b);
                if (result != 0 && (result < 0) != (b < 0)) result += b;
                push(NUMBER_VAL(result));
                break;
            }
            case OP_NOT:
                if (!IS_BOOL(peek(0))) {
                    runtimeError("'değil' mantıksal bir değer ister; %s verildi.",
                                 valueTypeName(peek(0)));
                    goto on_error;
                }
                push(BOOL_VAL(!AS_BOOL(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("'-' işleci bir sayı ister; %s verildi.", valueTypeName(peek(0)));
                    goto on_error;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_ASSERT_BOOL:
                if (!IS_BOOL(peek(0))) {
                    runtimeError("'ve' / 'veya' mantıksal değerler ister; %s verildi.",
                                 valueTypeName(peek(0)));
                    goto on_error;
                }
                break;
            case OP_ECHO: {
                Value value = pop();
                if (!IS_NIL(value)) {
                    printValue(stdout, value, true);
                    fputc('\n', stdout);
                }
                break;
            }
            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                frame->ip += offset;
                break;
            }
            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                if (!IS_BOOL(peek(0))) {
                    runtimeError("Koşul mantıksal bir değer (doğru/yanlış) olmalı; %s verildi.",
                                 valueTypeName(peek(0)));
                    goto on_error;
                }
                if (!AS_BOOL(pop())) frame->ip += offset;
                break;
            }
            case OP_AND: {
                uint16_t offset = READ_SHORT();
                if (!IS_BOOL(peek(0))) {
                    runtimeError("'ve' mantıksal değerler ister; %s verildi.",
                                 valueTypeName(peek(0)));
                    goto on_error;
                }
                if (!AS_BOOL(peek(0))) {
                    /* Sonuç yanlış; sağ tarafı ve denetimini atla. */
                    frame->ip += offset;
                } else {
                    pop();
                }
                break;
            }
            case OP_OR: {
                uint16_t offset = READ_SHORT();
                if (!IS_BOOL(peek(0))) {
                    runtimeError("'veya' mantıksal değerler ister; %s verildi.",
                                 valueTypeName(peek(0)));
                    goto on_error;
                }
                if (AS_BOOL(peek(0))) {
                    frame->ip += offset;
                } else {
                    pop();
                }
                break;
            }
            case OP_LOOP: {
                uint16_t offset = READ_SHORT();
                frame->ip -= offset;
                break;
            }
            case OP_CALL: {
                int argCount = READ_BYTE();
                if (!callValue(peek(argCount), argCount)) {
                    goto on_error;
                }
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
            case OP_CLOSURE: {
                ObjFunction *function = AS_FUNCTION(READ_CONSTANT());
                ObjClosure *closure = newClosure(function);
                push(OBJ_VAL(closure));
                for (int i = 0; i < closure->upvalueCount; i++) {
                    uint8_t isLocal = READ_BYTE();
                    uint8_t index = READ_BYTE();
                    if (isLocal) {
                        closure->upvalues[i] = captureUpvalue(frame->slots + index);
                    } else {
                        closure->upvalues[i] = frame->closure->upvalues[index];
                    }
                }
                break;
            }
            case OP_CLOSE_UPVALUE:
                closeUpvalues(vm.stackTop - 1);
                pop();
                break;
            case OP_BUILD_LIST: {
                int count = READ_SHORT();
                ObjList *list = newList();
                push(OBJ_VAL(list));
                for (int i = 0; i < count; i++) {
                    listAppend(list, vm.stackTop[-1 - count + i]);
                }
                vm.stackTop -= count + 1;
                push(OBJ_VAL(list));
                break;
            }
            case OP_BUILD_MAP: {
                int count = READ_SHORT();
                ObjMap *map = newMap();
                push(OBJ_VAL(map));
                Value *pairs = vm.stackTop - 1 - count * 2;
                for (int i = 0; i < count; i++) {
                    if (!checkKey(pairs[i * 2])) goto on_error;
                    mapSet(map, pairs[i * 2], pairs[i * 2 + 1]);
                }
                vm.stackTop -= count * 2 + 1;
                push(OBJ_VAL(map));
                break;
            }
            case OP_GET_INDEX:
                if (!getIndex()) goto on_error;
                break;
            case OP_SET_INDEX:
                if (!setIndex()) goto on_error;
                break;
            case OP_SLICE:
                if (!slice()) goto on_error;
                break;
            case OP_DUP2: {
                Value a = peek(1);
                Value b = peek(0);
                push(a);
                push(b);
                break;
            }
            case OP_IN:
                if (!contains()) goto on_error;
                break;
            case OP_FOR_NEXT: {
                uint16_t offset = READ_SHORT();
                bool done;
                if (!forNext(&done)) goto on_error;
                if (done) frame->ip += offset;
                break;
            }
            case OP_IMPORT: {
                ObjString *name = READ_STRING();
                if (!importModule(frame->closure->function->module, name)) goto on_error;
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
            case OP_MODULE: {
                ObjString *name = READ_STRING();
                Value module = NIL_VAL;
                bool found;
                resolveModule(frame->closure->function->module, name, &module, &found);
                pop();
                push(module);
                break;
            }
            case OP_GET_PROPERTY: {
                ObjString *name = READ_STRING();
                if (!getProperty(name)) goto on_error;
                break;
            }
            case OP_SET_PROPERTY: {
                ObjString *name = READ_STRING();
                if (!setProperty(name)) goto on_error;
                break;
            }
            case OP_INVOKE: {
                ObjString *name = READ_STRING();
                int argCount = READ_BYTE();
                if (!invoke(name, argCount)) goto on_error;
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
            case OP_CLASS:
                push(OBJ_VAL(newClass(READ_STRING())));
                break;
            case OP_INHERIT: {
                Value superclass = peek(1);
                if (!IS_CLASS(superclass)) {
                    runtimeError("Bir sınıf yalnızca başka bir sınıftan türetilebilir; %s verildi.",
                                 valueTypeName(superclass));
                    goto on_error;
                }
                ObjClass *subclass = AS_CLASS(peek(0));
                tableAddAll(&AS_CLASS(superclass)->methods, &subclass->methods);
                subclass->superclass = AS_CLASS(superclass);
                pop();
                break;
            }
            case OP_METHOD: {
                ObjString *name = READ_STRING();
                ObjClass *klass = AS_CLASS(peek(1));
                tableSet(&klass->methods, name, peek(0));
                pop();
                break;
            }
            case OP_GET_SUPER: {
                ObjString *name = READ_STRING();
                ObjClass *superclass = AS_CLASS(pop());
                if (!bindMethod(superclass, name)) goto on_error;
                break;
            }
            case OP_SUPER_INVOKE: {
                ObjString *name = READ_STRING();
                int argCount = READ_BYTE();
                ObjClass *superclass = AS_CLASS(pop());
                if (!invokeFromClass(superclass, name, argCount)) goto on_error;
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
            case OP_DUP:
                push(peek(0));
                break;
            case OP_TRY_BEGIN: {
                uint16_t offset = READ_SHORT();
                if (vm.handlerCount == HANDLERS_MAX) {
                    runtimeError("Çok fazla iç içe 'dene' bloğu var.");
                    goto on_error;
                }
                Handler *handler = &vm.handlers[vm.handlerCount++];
                handler->frameCount = vm.frameCount;
                handler->stackTop = vm.stackTop;
                handler->ip = frame->ip + offset;
                break;
            }
            case OP_TRY_END:
                vm.handlerCount--;
                break;
            case OP_THROW:
                vm.thrown = pop();
                vm.hasThrownValue = true;
                goto on_error;
            case OP_RETURN: {
                Value result = pop();
                closeUpvalues(frame->slots);
                /* Bu çağrının içinde kurulmuş yakalayıcılar artık geçersiz. */
                while (vm.handlerCount > 0 &&
                       vm.handlers[vm.handlerCount - 1].frameCount == vm.frameCount) {
                    vm.handlerCount--;
                }
                vm.frameCount--;
                if (vm.frameCount == 0 && script) {
                    pop();
                    return INTERPRET_OK;
                }

                vm.stackTop = frame->slots;
                push(result);
                if (vm.frameCount == stopAt) return INTERPRET_OK;
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        }
        continue;

    on_error:
        if (!handleError(stopAt)) return INTERPRET_RUNTIME_ERROR;
        frame = &vm.frames[vm.frameCount - 1];
    }

#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_STRING
#undef BINARY_OP
#undef COMPARE_OP
}

InterpretResult interpret(const char *name, const char *source, bool repl) {
    /* Etkileşimli kipte her satır aynı modülün içinde çalışır. */
    if (vm.mainModule == NULL) {
        ObjString *path = copyString(name, (int)strlen(name));
        push(OBJ_VAL(path));
        vm.mainModule = newModule(path, path);
        pop();
    }

    ObjFunction *function = compile(name, source, repl, vm.mainModule);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;

    push(OBJ_VAL(function));
    ObjClosure *closure = newClosure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);

    InterpretResult result = run(0, true);
    fflush(stdout);
    return result;
}

bool callFunction(Value callee, int argCount, Value *args, Value *result) {
    int base = vm.frameCount;
    push(callee);
    for (int i = 0; i < argCount; i++) push(args[i]);

    if (!callValue(callee, argCount)) {
        vm.errorPending = true;
        return false;
    }
    /* Yerleşik fonksiyonlar hemen sonuç verir; JUS fonksiyonları için yürütmeyi sürdür. */
    if (vm.frameCount > base && run(base, false) != INTERPRET_OK) {
        return false;
    }

    *result = pop();
    return true;
}

static int compareNames(const void *a, const void *b) {
    return strcmp((*(ObjString *const *)a)->chars, (*(ObjString *const *)b)->chars);
}

void runTestFile(const char *name, const char *source, int *passed, int *failed) {
    printf("%s\n", name);
    vm.captureErrors = true;
    vm.capturedError[0] = '\0';

    InterpretResult loaded = interpret(name, source, false);
    if (loaded != INTERPRET_OK) {
        if (loaded == INTERPRET_RUNTIME_ERROR) {
            printf("  KALDI  dosya yüklenirken hata: %s\n", vm.capturedError);
        } else {
            printf("  KALDI  dosyada sözdizimi hatası var\n");
        }
        (*failed)++;
        return;
    }

    /* Test fonksiyonlarını topla ve ada göre sırala; tablo sırası belirsizdir. */
    Table *globals = &vm.mainModule->globals;
    ObjString **names = (ObjString **)malloc(sizeof(ObjString *) * (size_t)(globals->capacity + 1));
    if (names == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    int count = 0;
    for (int i = 0; i < globals->capacity; i++) {
        Entry *entry = &globals->entries[i];
        if (entry->key == NULL || !IS_CLOSURE(entry->value)) continue;
        if (strncmp(entry->key->chars, "test_", 5) != 0) continue;
        if (AS_CLOSURE(entry->value)->function->arity != 0) continue;
        names[count++] = entry->key;
    }
    qsort(names, (size_t)count, sizeof(ObjString *), compareNames);

    if (count == 0) printf("  (adı \"test_\" ile başlayan fonksiyon yok)\n");
    for (int i = 0; i < count; i++) {
        Value function;
        Value ignored;
        if (!tableGet(globals, names[i], &function)) continue;
        vm.capturedError[0] = '\0';
        if (callFunction(function, 0, NULL, &ignored)) {
            printf("  geçti  %s\n", names[i]->chars);
            (*passed)++;
        } else {
            vm.errorPending = false;
            printf("  KALDI  %s\n         %s\n", names[i]->chars, vm.capturedError);
            (*failed)++;
        }
        fflush(stdout);
    }
    free(names);
}
