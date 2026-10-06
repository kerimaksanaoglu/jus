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
#include "text.h"
#include "vm.h"

#define TRACE_LIMIT 10

VM vm;

static void resetStack(void) {
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
    vm.openUpvalues = NULL;
    vm.handlerCount = 0;
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
    vm.mainModule = NULL;

    initTable(&vm.builtins);
    initTable(&vm.modules);
    initTable(&vm.strings);

    defineBuiltins();
    defineStandardModules();
}

void freeVM(void) {
    freeTable(&vm.builtins);
    freeTable(&vm.modules);
    freeTable(&vm.strings);
    vm.mainModule = NULL;
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
                    runtimeError("%s", vm.nativeError);
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
 * döner. Yoksa hatayı yazar ve false döner.
 */
static bool handleError(void) {
    if (vm.handlerCount == 0) {
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
            runtimeError("Sözlükte %s anahtarı yok.", shown);
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

/*
 * Modül adını çözer: önce yerleşik modüllere bakar, sonra içe aktaran dosyanın
 * klasöründe '<ad>.jus' dosyasını arar. Anahtar (ad ya da dosya yolu) yığına
 * konur; modül zaten yüklüyse *module doldurulur.
 */
static void resolveModule(ObjModule *importer, ObjString *name, Value *module, bool *found) {
    *found = tableGet(&vm.modules, name, module);
    if (*found) {
        push(OBJ_VAL(name));
        return;
    }

    const char *importerPath = importer->path->chars;
    int directoryLength = 0;
    for (int i = 0; i < importer->path->length; i++) {
        if (importerPath[i] == '/' || importerPath[i] == '\\') directoryLength = i + 1;
    }

    int length = directoryLength + name->length + 4;
    char *path = (char *)malloc((size_t)length + 1);
    if (path == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    memcpy(path, importerPath, (size_t)directoryLength);
    memcpy(path + directoryLength, name->chars, (size_t)name->length);
    memcpy(path + directoryLength + name->length, ".jus", 5);

    ObjString *key = copyString(path, length);
    free(path);
    push(OBJ_VAL(key));
    *found = tableGet(&vm.modules, key, module);
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
    const char *problem = NULL;
    char *source = readSource(path->chars, &problem);
    if (source == NULL) {
        runtimeError("'%s' modülü yüklenemedi: '%s' için %s.", name->chars, path->chars, problem);
        return false;
    }

    ObjModule *module = newModule(name, path);
    push(OBJ_VAL(module));
    /* Döngüsel 'kullan' zincirleri sonsuza gitmesin diye çalıştırmadan önce kaydet. */
    tableSet(&vm.modules, path, OBJ_VAL(module));

    ObjFunction *function = compile(path->chars, source, false, module);
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

/* [nesne] -> üye */
static bool getProperty(ObjString *name) {
    Value object = peek(0);
    if (IS_MODULE(object)) {
        Value value;
        if (!tableGet(&AS_MODULE(object)->globals, name, &value)) {
            runtimeError("'%s' modülünde '%s' adında bir üye yok.", AS_MODULE(object)->name->chars,
                         name->chars);
            return false;
        }
        pop();
        push(value);
        return true;
    }

    runtimeError("%s türündeki değerlerin '%s' adında bir üyesi yok.", valueTypeName(object),
                 name->chars);
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

static InterpretResult run(void) {
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
                    runtimeError("'%s' adında bir değişken ya da fonksiyon tanımlı değil.",
                                 name->chars);
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
                if (vm.frameCount == 0) {
                    pop();
                    return INTERPRET_OK;
                }

                vm.stackTop = frame->slots;
                push(result);
                frame = &vm.frames[vm.frameCount - 1];
                break;
            }
        }
        continue;

    on_error:
        if (!handleError()) return INTERPRET_RUNTIME_ERROR;
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

    InterpretResult result = run();
    fflush(stdout);
    return result;
}
