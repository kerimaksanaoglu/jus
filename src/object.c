#include <stdio.h>
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
    function->upvalueCount = 0;
    function->name = NULL;
    initChunk(&function->chunk);
    return function;
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

/* FNV-1a */
static uint32_t hashString(const char *key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
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

static const char *functionName(ObjFunction *function) {
    return function->name == NULL ? NULL : function->name->chars;
}

ObjString *valueToString(Value value) {
    char buffer[32];
    switch (value.type) {
        case VAL_BOOL:
            return AS_BOOL(value) ? copyString("doğru", (int)strlen("doğru"))
                                  : copyString("yanlış", (int)strlen("yanlış"));
        case VAL_NIL:
            return copyString("boş", (int)strlen("boş"));
        case VAL_NUMBER: {
            int length = formatNumber(AS_NUMBER(value), buffer, sizeof(buffer));
            return copyString(buffer, length);
        }
        case VAL_OBJ:
            break;
    }

    if (IS_STRING(value)) return AS_STRING(value);

    const char *name = NULL;
    if (IS_CLOSURE(value)) name = functionName(AS_CLOSURE(value)->function);
    else if (IS_FUNCTION(value)) name = functionName(AS_FUNCTION(value));
    else if (IS_NATIVE(value)) name = AS_NATIVE(value)->name;

    char text[256];
    int length = snprintf(text, sizeof(text), "<fonksiyon %s>", name == NULL ? "?" : name);
    if (length >= (int)sizeof(text)) length = (int)sizeof(text) - 1;
    return copyString(text, length);
}

static void printQuoted(FILE *out, ObjString *string) {
    fputc('"', out);
    for (int i = 0; i < string->length; i++) {
        char c = string->chars[i];
        switch (c) {
            case '"': fputs("\\\"", out); break;
            case '\\': fputs("\\\\", out); break;
            case '\n': fputs("\\n", out); break;
            case '\t': fputs("\\t", out); break;
            case '\r': fputs("\\r", out); break;
            default: fputc(c, out); break;
        }
    }
    fputc('"', out);
}

void printObject(FILE *out, Value value, bool quoteStrings) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            if (quoteStrings) {
                printQuoted(out, AS_STRING(value));
            } else {
                fwrite(AS_STRING(value)->chars, 1, (size_t)AS_STRING(value)->length, out);
            }
            break;
        case OBJ_CLOSURE: {
            const char *name = functionName(AS_CLOSURE(value)->function);
            fprintf(out, "<fonksiyon %s>", name == NULL ? "?" : name);
            break;
        }
        case OBJ_FUNCTION: {
            const char *name = functionName(AS_FUNCTION(value));
            fprintf(out, "<fonksiyon %s>", name == NULL ? "?" : name);
            break;
        }
        case OBJ_NATIVE:
            fprintf(out, "<fonksiyon %s>", AS_NATIVE(value)->name);
            break;
        case OBJ_UPVALUE:
            fputs("<üst değer>", out);
            break;
    }
}
