#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "memory.h"
#include "vm.h"

#define GC_HEAP_GROW_FACTOR 2

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
    vm.bytesAllocated += newSize - oldSize;
    if (newSize > oldSize) {
#ifdef JUS_DEBUG_STRESS_GC
        collectGarbage();
#else
        if (vm.bytesAllocated > vm.nextGC) {
            collectGarbage();
        }
#endif
    }

    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);
    if (result == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    return result;
}

void markObject(Obj *object) {
    if (object == NULL) return;
    if (object->isMarked) return;

    object->isMarked = true;

    if (vm.grayCapacity < vm.grayCount + 1) {
        vm.grayCapacity = GROW_CAPACITY(vm.grayCapacity);
        /* Gri yığın çöp toplayıcının kendi belleğidir; reallocate kullanılmaz. */
        Obj **grayStack = (Obj **)realloc(vm.grayStack, sizeof(Obj *) * (size_t)vm.grayCapacity);
        if (grayStack == NULL) {
            fprintf(stderr, "jus: bellek yetersiz.\n");
            exit(JUS_EXIT_OUT_OF_MEMORY);
        }
        vm.grayStack = grayStack;
    }

    vm.grayStack[vm.grayCount++] = object;
}

void markValue(Value value) {
    if (IS_OBJ(value)) markObject(AS_OBJ(value));
}

static void markArray(ValueArray *array) {
    for (int i = 0; i < array->count; i++) {
        markValue(array->values[i]);
    }
}

static void blackenObject(Obj *object) {
    switch (object->type) {
        case OBJ_BOUND_METHOD: {
            ObjBoundMethod *bound = (ObjBoundMethod *)object;
            markValue(bound->receiver);
            markObject((Obj *)bound->method);
            break;
        }
        case OBJ_CLASS: {
            ObjClass *klass = (ObjClass *)object;
            markObject((Obj *)klass->name);
            markObject((Obj *)klass->superclass);
            markTable(&klass->methods);
            break;
        }
        case OBJ_INSTANCE: {
            ObjInstance *instance = (ObjInstance *)object;
            markObject((Obj *)instance->klass);
            markTable(&instance->fields);
            break;
        }
        case OBJ_CLOSURE: {
            ObjClosure *closure = (ObjClosure *)object;
            markObject((Obj *)closure->function);
            for (int i = 0; i < closure->upvalueCount; i++) {
                markObject((Obj *)closure->upvalues[i]);
            }
            break;
        }
        case OBJ_FUNCTION: {
            ObjFunction *function = (ObjFunction *)object;
            markObject((Obj *)function->name);
            markObject((Obj *)function->module);
            markArray(&function->chunk.constants);
            break;
        }
        case OBJ_MODULE: {
            ObjModule *module = (ObjModule *)object;
            markObject((Obj *)module->name);
            markObject((Obj *)module->path);
            markTable(&module->globals);
            break;
        }
        case OBJ_LIST: {
            ObjList *list = (ObjList *)object;
            for (int i = 0; i < list->count; i++) {
                markValue(list->items[i]);
            }
            break;
        }
        case OBJ_MAP: {
            ObjMap *map = (ObjMap *)object;
            for (int i = 0; i < map->used; i++) {
                markValue(map->entries[i].key);
                markValue(map->entries[i].value);
            }
            break;
        }
        case OBJ_UPVALUE:
            markValue(((ObjUpvalue *)object)->closed);
            break;
        case OBJ_NATIVE:
        case OBJ_STRING:
            break;
    }
}

static void freeObject(Obj *object) {
    switch (object->type) {
        case OBJ_BOUND_METHOD:
            FREE(ObjBoundMethod, object);
            break;
        case OBJ_CLASS: {
            ObjClass *klass = (ObjClass *)object;
            freeTable(&klass->methods);
            FREE(ObjClass, object);
            break;
        }
        case OBJ_INSTANCE: {
            ObjInstance *instance = (ObjInstance *)object;
            freeTable(&instance->fields);
            FREE(ObjInstance, object);
            break;
        }
        case OBJ_CLOSURE: {
            ObjClosure *closure = (ObjClosure *)object;
            FREE_ARRAY(ObjUpvalue *, closure->upvalues, closure->upvalueCount);
            FREE(ObjClosure, object);
            break;
        }
        case OBJ_FUNCTION: {
            ObjFunction *function = (ObjFunction *)object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
            break;
        }
        case OBJ_LIST: {
            ObjList *list = (ObjList *)object;
            FREE_ARRAY(Value, list->items, list->capacity);
            FREE(ObjList, object);
            break;
        }
        case OBJ_MAP: {
            ObjMap *map = (ObjMap *)object;
            FREE_ARRAY(MapEntry, map->entries, map->capacity);
            FREE_ARRAY(int, map->indices, map->capacity);
            FREE(ObjMap, object);
            break;
        }
        case OBJ_MODULE: {
            ObjModule *module = (ObjModule *)object;
            freeTable(&module->globals);
            FREE(ObjModule, object);
            break;
        }
        case OBJ_NATIVE:
            FREE(ObjNative, object);
            break;
        case OBJ_STRING: {
            ObjString *string = (ObjString *)object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
        case OBJ_UPVALUE:
            FREE(ObjUpvalue, object);
            break;
    }
}

static void markRoots(void) {
    for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
        markValue(*slot);
    }

    for (int i = 0; i < vm.frameCount; i++) {
        markObject((Obj *)vm.frames[i].closure);
    }

    for (ObjUpvalue *upvalue = vm.openUpvalues; upvalue != NULL; upvalue = upvalue->next) {
        markObject((Obj *)upvalue);
    }

    markValue(vm.thrown);
    markTable(&vm.builtins);
    markTable(&vm.modules);
    markObject((Obj *)vm.mainModule);
    markObject((Obj *)vm.initString);
    markCompilerRoots();
}

static void traceReferences(void) {
    while (vm.grayCount > 0) {
        Obj *object = vm.grayStack[--vm.grayCount];
        blackenObject(object);
    }
}

static void sweep(void) {
    Obj *previous = NULL;
    Obj *object = vm.objects;
    while (object != NULL) {
        if (object->isMarked) {
            object->isMarked = false;
            previous = object;
            object = object->next;
        } else {
            Obj *unreached = object;
            object = object->next;
            if (previous != NULL) {
                previous->next = object;
            } else {
                vm.objects = object;
            }

            freeObject(unreached);
        }
    }
}

void collectGarbage(void) {
    markRoots();
    traceReferences();
    /* Metin tablosu zayıf başvuru tutar: işaretlenmemiş metinleri çıkar. */
    tableRemoveWhite(&vm.strings);
    sweep();

    vm.nextGC = vm.bytesAllocated * GC_HEAP_GROW_FACTOR;
}

void freeObjects(void) {
    Obj *object = vm.objects;
    while (object != NULL) {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }

    free(vm.grayStack);
    vm.grayStack = NULL;
    vm.grayCount = 0;
    vm.grayCapacity = 0;
}
