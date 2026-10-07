#ifndef JUS_MEMORY_H
#define JUS_MEMORY_H

#include "common.h"
#include "object.h"

#define ALLOCATE(type, count) (type *)reallocate(NULL, 0, sizeof(type) * (size_t)(count))

#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type *)reallocate(pointer, sizeof(type) * (size_t)(oldCount), sizeof(type) * (size_t)(newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (size_t)(oldCount), 0)

/* Tüm dinamik bellek bu fonksiyondan geçer; çöp toplayıcı buradan tetiklenir. */
void *reallocate(void *pointer, size_t oldSize, size_t newSize);
void markObject(Obj *object);
void markValue(Value value);
void collectGarbage(void);
void freeObjects(void);

#endif
