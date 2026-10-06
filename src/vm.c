#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtins.h"
#include "common.h"
#include "compiler.h"
#include "memory.h"
#include "object.h"
#include "vm.h"

#define TRACE_LIMIT 10

VM vm;

static void resetStack(void) {
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
    vm.openUpvalues = NULL;
}

static void runtimeError(const char *format, ...) {
    fflush(stdout);

    CallFrame *top = &vm.frames[vm.frameCount - 1];
    size_t instruction = (size_t)(top->ip - top->closure->function->chunk.code - 1);
    fprintf(stderr, "%s:%d: çalışma zamanı hatası: ", vm.sourceName,
            top->closure->function->chunk.lines[instruction]);

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
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
                fprintf(stderr, "    satır %d, ana program\n", line);
            } else {
                fprintf(stderr, "    satır %d, '%s' fonksiyonu\n", line, function->name->chars);
            }
            shown++;
        }
    }

    resetStack();
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
    tableSet(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
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

    vm.sourceName = "";
    vm.nativeError[0] = '\0';

    initTable(&vm.globals);
    initTable(&vm.strings);

    defineBuiltins();
}

void freeVM(void) {
    freeTable(&vm.globals);
    freeTable(&vm.strings);
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
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
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
                if (!tableGet(&vm.globals, name, &value)) {
                    runtimeError("'%s' adında bir değişken ya da fonksiyon tanımlı değil.",
                                 name->chars);
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(value);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                ObjString *name = READ_STRING();
                tableSet(&vm.globals, name, peek(0));
                pop();
                break;
            }
            case OP_SET_GLOBAL: {
                ObjString *name = READ_STRING();
                if (tableSet(&vm.globals, name, peek(0))) {
                    tableDelete(&vm.globals, name);
                    runtimeError("'%s' adında bir değişken tanımlı değil. Yeni değişken için "
                                 "'değişken %s = ...' yazın.", name->chars, name->chars);
                    return INTERPRET_RUNTIME_ERROR;
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
            case OP_GREATER: BINARY_OP(BOOL_VAL, >, ">"); break;
            case OP_GREATER_EQUAL: BINARY_OP(BOOL_VAL, >=, ">="); break;
            case OP_LESS: BINARY_OP(BOOL_VAL, <, "<"); break;
            case OP_LESS_EQUAL: BINARY_OP(BOOL_VAL, <=, "<="); break;
            case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                } else {
                    runtimeError("'+' işleci iki sayı ya da iki metin ister; %s ve %s verildi. "
                                 "Dönüştürmek için metin() ya da sayı() kullanılabilir.",
                                 valueTypeName(peek(1)), valueTypeName(peek(0)));
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -, "-"); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *, "*"); break;
            case OP_DIVIDE: {
                if (IS_NUMBER(peek(0)) && AS_NUMBER(peek(0)) == 0 && IS_NUMBER(peek(1))) {
                    runtimeError("Sıfıra bölünemez.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                BINARY_OP(NUMBER_VAL, /, "/");
                break;
            }
            case OP_MODULO: {
                if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) {
                    runtimeError("'%%' işleci iki sayı ister; %s ve %s verildi.",
                                 valueTypeName(peek(1)), valueTypeName(peek(0)));
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (AS_NUMBER(peek(0)) == 0) {
                    runtimeError("Sıfıra göre kalan hesaplanamaz.");
                    return INTERPRET_RUNTIME_ERROR;
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
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(BOOL_VAL(!AS_BOOL(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("'-' işleci bir sayı ister; %s verildi.", valueTypeName(peek(0)));
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_ASSERT_BOOL:
                if (!IS_BOOL(peek(0))) {
                    runtimeError("'ve' / 'veya' mantıksal değerler ister; %s verildi.",
                                 valueTypeName(peek(0)));
                    return INTERPRET_RUNTIME_ERROR;
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
                    return INTERPRET_RUNTIME_ERROR;
                }
                if (!AS_BOOL(pop())) frame->ip += offset;
                break;
            }
            case OP_AND: {
                uint16_t offset = READ_SHORT();
                if (!IS_BOOL(peek(0))) {
                    runtimeError("'ve' mantıksal değerler ister; %s verildi.",
                                 valueTypeName(peek(0)));
                    return INTERPRET_RUNTIME_ERROR;
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
                    return INTERPRET_RUNTIME_ERROR;
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
                    return INTERPRET_RUNTIME_ERROR;
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
            case OP_RETURN: {
                Value result = pop();
                closeUpvalues(frame->slots);
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
    }

#undef READ_BYTE
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_STRING
#undef BINARY_OP
}

InterpretResult interpret(const char *name, const char *source, bool repl) {
    vm.sourceName = name;

    ObjFunction *function = compile(name, source, repl);
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
