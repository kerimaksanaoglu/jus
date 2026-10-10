#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type) {
    Obj *object = (Obj *)reallocate(NULL, 0, size);
    object->type = type;
    object->isMarked = false;

    object->next = vm.objects;
    vm.objects = object;
    return object;
}

ObjBoundMethod *newBoundMethod(Value receiver, ObjClosure *method) {
    ObjBoundMethod *bound = ALLOCATE_OBJ(ObjBoundMethod, OBJ_BOUND_METHOD);
    bound->receiver = receiver;
    bound->method = method;
    return bound;
}

ObjClass *newClass(ObjString *name) {
    ObjClass *klass = ALLOCATE_OBJ(ObjClass, OBJ_CLASS);
    klass->name = name;
    klass->superclass = NULL;
    initTable(&klass->methods);
    return klass;
}

ObjInstance *newInstance(ObjClass *klass) {
    ObjInstance *instance = ALLOCATE_OBJ(ObjInstance, OBJ_INSTANCE);
    instance->klass = klass;
    initTable(&instance->fields);
    return instance;
}

ObjClosure *newClosure(ObjFunction *function) {
    ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue *, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++) {
        upvalues[i] = NULL;
    }

    ObjClosure *closure = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}

ObjFunction *newFunction(void) {
    ObjFunction *function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->optionalCount = 0;
    function->hasRest = false;
    function->upvalueCount = 0;
    function->name = NULL;
    function->module = NULL;
    initValueArray(&function->localNames);
    initChunk(&function->chunk);
    return function;
}

ObjModule *newModule(ObjString *name, ObjString *path) {
    ObjModule *module = ALLOCATE_OBJ(ObjModule, OBJ_MODULE);
    module->name = name;
    module->path = path;
    initTable(&module->globals);
    return module;
}

ObjNative *newNative(const char *name, int arity, NativeFn function) {
    ObjNative *native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
    native->function = function;
    native->arity = arity;
    native->name = name;
    return native;
}

static ObjString *allocateString(char *chars, int length, uint32_t hash) {
    ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    /* Tablo büyürken çöp toplayıcı çalışabilir; metni yığında koru. */
    push(OBJ_VAL(string));
    tableSet(&vm.strings, string, NIL_VAL);
    pop();

    return string;
}

/*
 * FNV-1a. Uzun metinlerde baytların tümü değil, eşit aralıklı en çok ~64 tanesi
 * karılır; böylece büyük metinler üretmek (ör. döngüde birleştirme) metnin
 * uzunluğuyla orantılı bir karma maliyeti getirmez. Uzunluk da karmaya katılır.
 */
static uint32_t hashString(const char *key, int length) {
    uint32_t hash = 2166136261u ^ (uint32_t)length;
    int step = (length >> 6) + 1;
    for (int i = length; i >= step; i -= step) {
        hash ^= (uint8_t)key[i - 1];
        hash *= 16777619;
    }
    return hash;
}

ObjString *takeString(char *chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocateString(chars, length, hash);
}

ObjString *copyString(const char *chars, int length) {
    uint32_t hash = hashString(chars, length);
    ObjString *interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;

    char *heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, hash);
}

ObjUpvalue *newUpvalue(Value *slot) {
    ObjUpvalue *upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

ObjString *valueToString(Value value) {
    if (IS_STRING(value)) return AS_STRING(value);

    int length;
    char *chars = valueToChars(value, false, &length);
    ObjString *string = copyString(chars, length);
    free(chars);
    return string;
}

/* ---- Liste ---- */

ObjList *newList(void) {
    ObjList *list = ALLOCATE_OBJ(ObjList, OBJ_LIST);
    list->count = 0;
    list->capacity = 0;
    list->items = NULL;
    return list;
}

static void listReserve(ObjList *list) {
    if (list->capacity < list->count + 1) {
        int oldCapacity = list->capacity;
        int capacity = GROW_CAPACITY(oldCapacity);
        /* Önce ayır, sonra kapasiteyi güncelle: ayırma sırasında çöp toplayıcı listeyi gezebilir. */
        list->items = GROW_ARRAY(Value, list->items, oldCapacity, capacity);
        list->capacity = capacity;
    }
}

void listAppend(ObjList *list, Value value) {
    listReserve(list);
    list->items[list->count++] = value;
}

void listInsert(ObjList *list, int index, Value value) {
    listReserve(list);
    memmove(&list->items[index + 1], &list->items[index],
            sizeof(Value) * (size_t)(list->count - index));
    list->items[index] = value;
    list->count++;
}

Value listRemove(ObjList *list, int index) {
    Value removed = list->items[index];
    memmove(&list->items[index], &list->items[index + 1],
            sizeof(Value) * (size_t)(list->count - index - 1));
    list->count--;
    return removed;
}

/* ---- Sözlük ---- */

#define MAP_EMPTY (-1)
#define MAP_DELETED (-2)

ObjMap *newMap(void) {
    ObjMap *map = ALLOCATE_OBJ(ObjMap, OBJ_MAP);
    map->count = 0;
    map->used = 0;
    map->capacity = 0;
    map->entries = NULL;
    map->indices = NULL;
    return map;
}

bool isHashable(Value value) {
    if (IS_STRING(value) || IS_BOOL(value)) return true;
    return IS_NUMBER(value) && AS_NUMBER(value) == AS_NUMBER(value);
}

static uint32_t hashValue(Value value) {
    if (IS_STRING(value)) return AS_STRING(value)->hash;
    if (IS_BOOL(value)) return AS_BOOL(value) ? 3 : 5;

    double number = AS_NUMBER(value);
    if (number == 0) number = 0; /* 0 ile -0 aynı anahtardır */
    uint64_t bits;
    memcpy(&bits, &number, sizeof(bits));
    return (uint32_t)(bits ^ (bits >> 32)) * 2654435761u;
}

/*
 * Anahtarı arar. Bulursa girdi numarasını döndürür; bulamazsa -1 döndürür ve
 * *slot, anahtarın eklenebileceği indices yuvasını gösterir.
 */
static int mapFind(ObjMap *map, Value key, uint32_t hash, int *slot) {
    *slot = -1;
    if (map->capacity == 0) return -1;

    uint32_t mask = (uint32_t)map->capacity - 1;
    uint32_t index = hash & mask;
    int tombstone = -1;
    for (;;) {
        int entry = map->indices[index];
        if (entry == MAP_EMPTY) {
            *slot = tombstone != -1 ? tombstone : (int)index;
            return -1;
        }
        if (entry == MAP_DELETED) {
            if (tombstone == -1) tombstone = (int)index;
        } else if (map->entries[entry].hash == hash && valuesEqual(map->entries[entry].key, key)) {
            *slot = (int)index;
            return entry;
        }
        index = (index + 1) & mask;
    }
}

/* Tabloyu büyütür ve silinmiş girdileri atarak sıkıştırır. */
static void mapRebuild(ObjMap *map) {
    int capacity = 8;
    while (capacity * 2 < (map->count + 1) * 3) capacity *= 2;

    MapEntry *entries = ALLOCATE(MapEntry, capacity);
    int *indices = ALLOCATE(int, capacity);
    for (int i = 0; i < capacity; i++) indices[i] = MAP_EMPTY;

    uint32_t mask = (uint32_t)capacity - 1;
    int used = 0;
    for (int i = 0; i < map->used; i++) {
        if (!map->entries[i].live) continue;
        entries[used] = map->entries[i];
        uint32_t index = entries[used].hash & mask;
        while (indices[index] != MAP_EMPTY) index = (index + 1) & mask;
        indices[index] = used;
        used++;
    }

    FREE_ARRAY(MapEntry, map->entries, map->capacity);
    FREE_ARRAY(int, map->indices, map->capacity);
    map->entries = entries;
    map->indices = indices;
    map->capacity = capacity;
    map->used = used;
}

bool mapGet(ObjMap *map, Value key, Value *value) {
    int slot;
    int entry = mapFind(map, key, hashValue(key), &slot);
    if (entry == -1) return false;
    *value = map->entries[entry].value;
    return true;
}

void mapSet(ObjMap *map, Value key, Value value) {
    uint32_t hash = hashValue(key);
    int slot;
    int entry = mapFind(map, key, hash, &slot);
    if (entry != -1) {
        map->entries[entry].value = value;
        return;
    }

    /* Doluluk (silinmişler dahil) üçte ikiyi geçmesin; böylece her zaman boş yuva kalır. */
    if ((map->used + 1) * 3 > map->capacity * 2) {
        mapRebuild(map);
        mapFind(map, key, hash, &slot);
    }

    MapEntry *added = &map->entries[map->used];
    added->key = key;
    added->value = value;
    added->hash = hash;
    added->live = true;
    map->indices[slot] = map->used;
    map->used++;
    map->count++;
}

bool mapDelete(ObjMap *map, Value key) {
    int slot;
    int entry = mapFind(map, key, hashValue(key), &slot);
    if (entry == -1) return false;

    map->entries[entry].live = false;
    map->entries[entry].key = NIL_VAL;
    map->entries[entry].value = NIL_VAL;
    map->indices[slot] = MAP_DELETED;
    map->count--;
    return true;
}
