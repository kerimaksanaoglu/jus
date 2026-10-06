#ifndef JUS_VM_H
#define JUS_VM_H

#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 1024
#define STACK_MAX (FRAMES_MAX * 64)
#define HANDLERS_MAX 256

typedef struct {
    ObjClosure *closure;
    uint8_t *ip;
    Value *slots;
} CallFrame;

/* Bir 'dene' bloğuna girilirken kaydedilen durum. */
typedef struct {
    int frameCount;
    Value *stackTop;
    uint8_t *ip; /* 'yakala' bloğunun başlangıcı */
} Handler;

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frameCount;

    Handler handlers[HANDLERS_MAX];
    int handlerCount;
    /* Bekleyen hata: 'fırlat' ile atılan değer ya da çalışma zamanı hata iletisi. */
    bool hasThrownValue;
    Value thrown;
    char errorMessage[512];

    Value *stack;
    Value *stackTop;
    Table globals;
    Table strings;
    ObjUpvalue *openUpvalues;

    size_t bytesAllocated;
    size_t nextGC;
    Obj *objects;
    int grayCount;
    int grayCapacity;
    Obj **grayStack;

    const char *sourceName;
    char nativeError[256];
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM(void);
void freeVM(void);
InterpretResult interpret(const char *name, const char *source, bool repl);
void push(Value value);
Value pop(void);

void defineNative(const char *name, int arity, NativeFn function);
/* Yerleşik fonksiyonlar hata iletisini bununla bırakır; her zaman false döndürür. */
bool nativeFail(const char *format, ...);

#endif
