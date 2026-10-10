#ifndef JUS_OBJECT_H
#define JUS_OBJECT_H

#include "chunk.h"
#include "common.h"
#include "table.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_BOUND_METHOD(value) isObjType(value, OBJ_BOUND_METHOD)
#define IS_BYTES(value) isObjType(value, OBJ_BYTES)
#define IS_CLASS(value) isObjType(value, OBJ_CLASS)
#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
#define IS_INSTANCE(value) isObjType(value, OBJ_INSTANCE)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_LIST(value) isObjType(value, OBJ_LIST)
#define IS_MAP(value) isObjType(value, OBJ_MAP)
#define IS_MODULE(value) isObjType(value, OBJ_MODULE)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_BOUND_METHOD(value) ((ObjBoundMethod *)AS_OBJ(value))
#define AS_BYTES(value) ((ObjBytes *)AS_OBJ(value))
#define AS_CLASS(value) ((ObjClass *)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure *)AS_OBJ(value))
#define AS_INSTANCE(value) ((ObjInstance *)AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_LIST(value) ((ObjList *)AS_OBJ(value))
#define AS_MAP(value) ((ObjMap *)AS_OBJ(value))
#define AS_MODULE(value) ((ObjModule *)AS_OBJ(value))
#define AS_NATIVE(value) ((ObjNative *)AS_OBJ(value))
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum {
    OBJ_BOUND_METHOD,
    OBJ_BYTES,
    OBJ_CLASS,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_INSTANCE,
    OBJ_LIST,
    OBJ_MAP,
    OBJ_MODULE,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_UPVALUE
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
    struct Obj *next;
};

/* Bir kaynak dosyası ya da yerleşik modül; üst düzey adlarını tutar. */
typedef struct ObjModule {
    Obj obj;
    ObjString *name;
    ObjString *path; /* hata iletilerinde gösterilen dosya yolu */
    Table globals;
} ObjModule;

typedef struct {
    Obj obj;
    int arity;         /* zorunlu parametre sayısı */
    int optionalCount; /* varsayılan değeri olan parametre sayısı */
    bool hasRest;      /* son parametre '*ad' biçiminde: kalan argümanları liste olarak alır */
    int upvalueCount;
    Chunk chunk;
    ObjString *name; /* modülün üst düzey kodu için NULL */
    ObjModule *module; /* fonksiyonun tanımlandığı modül */
    /* Fonksiyonda ve onu saran fonksiyonlarda tanımlı yerel adlar; hata önerileri için. */
    ValueArray localNames;
} ObjFunction;

/*
 * Yerleşik fonksiyon. Başarılıysa sonucu *result'a yazıp true döndürür;
 * hata durumunda nativeFail() ile ileti bırakıp false döndürür.
 */
typedef bool (*NativeFn)(int argCount, Value *args, Value *result);

typedef struct {
    Obj obj;
    NativeFn function;
    int arity; /* -1: değişken sayıda argüman */
    const char *name;
} ObjNative;

struct ObjString {
    Obj obj;
    int length; /* bayt cinsinden */
    char *chars;
    uint32_t hash;
};

typedef struct {
    Obj obj;
    int count;
    int capacity;
    Value *items;
} ObjList;

/* Bayt dizisi (baytlar türü); öğeler 0-255 arası değerlerdir. */
typedef struct {
    Obj obj;
    int count;
    int capacity;
    uint8_t *data;
} ObjBytes;

typedef struct {
    Value key;
    Value value;
    uint32_t hash;
    bool live; /* silinen girdiler yerinde kalır, live yanlış olur */
} MapEntry;

/*
 * Ekleme sırasını koruyan sözlük. Girdiler eklenme sırasıyla entries dizisinde
 * durur; indices, karma değerinden girdi numarasına giden açık adreslemeli tablodur.
 */
typedef struct {
    Obj obj;
    int count; /* canlı girdi sayısı */
    int used;  /* entries içinde kullanılan yuva sayısı (silinmişler dahil) */
    int capacity; /* hem entries hem indices kapasitesi; 0 ya da ikinin kuvveti */
    MapEntry *entries;
    int *indices; /* -1: boş, -2: silinmiş, diğer: girdi numarası */
} ObjMap;

typedef struct ObjUpvalue {
    Obj obj;
    Value *location;
    Value closed;
    struct ObjUpvalue *next;
} ObjUpvalue;

typedef struct {
    Obj obj;
    ObjFunction *function;
    ObjUpvalue **upvalues;
    int upvalueCount;
} ObjClosure;

typedef struct ObjClass {
    Obj obj;
    ObjString *name;
    struct ObjClass *superclass; /* yoksa NULL */
    Table methods; /* üst sınıftan devralınanlar dahil */
} ObjClass;

typedef struct {
    Obj obj;
    ObjClass *klass;
    Table fields;
} ObjInstance;

/* Bir nesneye bağlanmış yöntem: nesne.yöntem ifadesinin değeri. */
typedef struct {
    Obj obj;
    Value receiver;
    ObjClosure *method;
} ObjBoundMethod;

/* Verilen boyutta ham nesne ayırır ve nesne listesine ekler (bytes.c gibi başka dosyalar için). */
Obj *allocateObject(size_t size, ObjType type);
ObjBoundMethod *newBoundMethod(Value receiver, ObjClosure *method);
ObjClass *newClass(ObjString *name);
ObjInstance *newInstance(ObjClass *klass);
ObjClosure *newClosure(ObjFunction *function);
ObjFunction *newFunction(void);
/* name ve path çağıran tarafından çöp toplayıcıdan korunmalıdır. */
ObjModule *newModule(ObjString *name, ObjString *path);
ObjNative *newNative(const char *name, int arity, NativeFn function);
ObjString *takeString(char *chars, int length);
ObjString *copyString(const char *chars, int length);
ObjUpvalue *newUpvalue(Value *slot);

/*
 * Liste ve sözlük işlemleri bellek ayırabilir. Çağıran, hem kabın hem de eklenen
 * değerlerin çöp toplayıcıdan korunduğundan (ör. yığında durduğundan) emin olmalıdır.
 */
ObjList *newList(void);
void listAppend(ObjList *list, Value value);
void listInsert(ObjList *list, int index, Value value);
Value listRemove(ObjList *list, int index);

ObjMap *newMap(void);
/* Sözlük anahtarı olabilen türler: metin, sayı, mantıksal. */
bool isHashable(Value value);
bool mapGet(ObjMap *map, Value key, Value *value);
void mapSet(ObjMap *map, Value key, Value value);
bool mapDelete(ObjMap *map, Value key);

/* Herhangi bir değeri metne çevirir (metin() için). */
ObjString *valueToString(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
