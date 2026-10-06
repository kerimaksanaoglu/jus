#ifndef JUS_OBJECT_H
#define JUS_OBJECT_H

#include "chunk.h"
#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_CLOSURE(value) isObjType(value, OBJ_CLOSURE)
#define IS_FUNCTION(value) isObjType(value, OBJ_FUNCTION)
#define IS_LIST(value) isObjType(value, OBJ_LIST)
#define IS_MAP(value) isObjType(value, OBJ_MAP)
#define IS_NATIVE(value) isObjType(value, OBJ_NATIVE)
#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_CLOSURE(value) ((ObjClosure *)AS_OBJ(value))
#define AS_FUNCTION(value) ((ObjFunction *)AS_OBJ(value))
#define AS_LIST(value) ((ObjList *)AS_OBJ(value))
#define AS_MAP(value) ((ObjMap *)AS_OBJ(value))
#define AS_NATIVE(value) ((ObjNative *)AS_OBJ(value))
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString *)AS_OBJ(value))->chars)

typedef enum {
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_LIST,
    OBJ_MAP,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_UPVALUE
} ObjType;

struct Obj {
    ObjType type;
    bool isMarked;
    struct Obj *next;
};

typedef struct {
    Obj obj;
    int arity;
    int upvalueCount;
    Chunk chunk;
    ObjString *name; /* ana program için NULL */
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

ObjClosure *newClosure(ObjFunction *function);
ObjFunction *newFunction(void);
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
