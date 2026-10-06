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
    /* Yerleşik bir fonksiyonun çağırdığı JUS fonksiyonu hata verdi; ileti zaten kayıtlı. */
    bool errorPending;
    bool hasThrownValue;
    Value thrown;
    char errorMessage[512];

    Value *stack;
    Value *stackTop;
    Table builtins; /* her modülden görülen yerleşik fonksiyonlar */
    Table modules;  /* ad ya da dosya yolu -> yüklenmiş modül */
    ObjModule *mainModule;
    ObjString *initString; /* "kur" */
    Table strings;
    ObjUpvalue *openUpvalues;

    size_t bytesAllocated;
    size_t nextGC;
    Obj *objects;
    int grayCount;
    int grayCapacity;
    Obj **grayStack;

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
/* Verilen adla yerleşik modülü döndürür; yoksa oluşturur. */
ObjModule *defineModule(const char *name);
void moduleDefine(ObjModule *module, const char *name, Value value);
void moduleDefineNative(ObjModule *module, const char *name, int arity, NativeFn function);
/* Yerleşik fonksiyonlar hata iletisini bununla bırakır; her zaman false döndürür. */
bool nativeFail(const char *format, ...);

/*
 * Yerleşik bir fonksiyonun içinden bir JUS değerini çağırır. Başarısız olursa
 * false döner; hata zaten kayıtlıdır, çağıran nativeFail kullanmadan doğrudan
 * false döndürmelidir.
 */
bool callFunction(Value callee, int argCount, Value *args, Value *result);

#endif
