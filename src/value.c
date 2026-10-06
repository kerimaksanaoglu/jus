#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"

#define MAX_NESTING 32

void initValueArray(ValueArray *array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void writeValueArray(ValueArray *array, Value value) {
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

static bool equalAtDepth(Value a, Value b, int depth) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL: return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ: break;
    }

    /* Metinler tekilleştirildiği için işaretçi karşılaştırması yeterli. */
    if (AS_OBJ(a) == AS_OBJ(b)) return true;
    if (depth > MAX_NESTING) return false;

    if (IS_LIST(a) && IS_LIST(b)) {
        ObjList *x = AS_LIST(a);
        ObjList *y = AS_LIST(b);
        if (x->count != y->count) return false;
        for (int i = 0; i < x->count; i++) {
            if (!equalAtDepth(x->items[i], y->items[i], depth + 1)) return false;
        }
        return true;
    }

    if (IS_MAP(a) && IS_MAP(b)) {
        ObjMap *x = AS_MAP(a);
        ObjMap *y = AS_MAP(b);
        if (x->count != y->count) return false;
        for (int i = 0; i < x->used; i++) {
            if (!x->entries[i].live) continue;
            Value other;
            if (!mapGet(y, x->entries[i].key, &other)) return false;
            if (!equalAtDepth(x->entries[i].value, other, depth + 1)) return false;
        }
        return true;
    }

    return false;
}

bool valuesEqual(Value a, Value b) {
    return equalAtDepth(a, b, 0);
}

const char *valueTypeName(Value value) {
    switch (value.type) {
        case VAL_BOOL: return "mantıksal";
        case VAL_NIL: return "boş";
        case VAL_NUMBER: return "sayı";
        case VAL_OBJ:
            switch (OBJ_TYPE(value)) {
                case OBJ_STRING: return "metin";
                case OBJ_LIST: return "liste";
                case OBJ_MAP: return "sözlük";
                case OBJ_MODULE: return "modül";
                case OBJ_CLASS: return "sınıf";
                case OBJ_INSTANCE: return AS_INSTANCE(value)->klass->name->chars;
                case OBJ_BOUND_METHOD:
                case OBJ_CLOSURE:
                case OBJ_FUNCTION:
                case OBJ_NATIVE: return "fonksiyon";
                case OBJ_UPVALUE: break;
            }
            break;
    }
    return "bilinmeyen";
}

int formatNumber(double number, char *buffer, size_t size) {
    if (isnan(number)) return snprintf(buffer, size, "tanımsız");
    if (isinf(number)) return snprintf(buffer, size, number > 0 ? "sonsuz" : "-sonsuz");
    /* Tam sayı değerleri ondalık kısım olmadan yazılır. */
    if (number == floor(number) && fabs(number) <= 9007199254740992.0) {
        if (number == 0) number = 0; /* -0 yerine 0 */
        return snprintf(buffer, size, "%.0f", number);
    }
    return snprintf(buffer, size, "%.14g", number);
}

/* ---- Değerleri yazıya dökme ---- */

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} TextBuffer;

static void bufferAppend(TextBuffer *buffer, const char *text, size_t length) {
    if (buffer->length + length + 1 > buffer->capacity) {
        size_t capacity = buffer->capacity < 64 ? 64 : buffer->capacity;
        while (buffer->length + length + 1 > capacity) capacity *= 2;
        char *data = (char *)realloc(buffer->data, capacity);
        if (data == NULL) {
            fprintf(stderr, "jus: bellek yetersiz.\n");
            exit(JUS_EXIT_OUT_OF_MEMORY);
        }
        buffer->data = data;
        buffer->capacity = capacity;
    }
    memcpy(buffer->data + buffer->length, text, length);
    buffer->length += length;
    buffer->data[buffer->length] = '\0';
}

static void bufferAppendText(TextBuffer *buffer, const char *text) {
    bufferAppend(buffer, text, strlen(text));
}

static void appendQuoted(TextBuffer *buffer, ObjString *string) {
    bufferAppend(buffer, "\"", 1);
    for (int i = 0; i < string->length; i++) {
        char c = string->chars[i];
        switch (c) {
            case '"': bufferAppend(buffer, "\\\"", 2); break;
            case '\\': bufferAppend(buffer, "\\\\", 2); break;
            case '\n': bufferAppend(buffer, "\\n", 2); break;
            case '\t': bufferAppend(buffer, "\\t", 2); break;
            case '\r': bufferAppend(buffer, "\\r", 2); break;
            default: bufferAppend(buffer, &c, 1); break;
        }
    }
    bufferAppend(buffer, "\"", 1);
}

static void appendFunction(TextBuffer *buffer, const char *name) {
    bufferAppendText(buffer, "<fonksiyon ");
    bufferAppendText(buffer, name == NULL ? "?" : name);
    bufferAppend(buffer, ">", 1);
}

static void appendValue(TextBuffer *buffer, Value value, bool quoteStrings, int depth) {
    switch (value.type) {
        case VAL_BOOL:
            bufferAppendText(buffer, AS_BOOL(value) ? "doğru" : "yanlış");
            return;
        case VAL_NIL:
            bufferAppendText(buffer, "boş");
            return;
        case VAL_NUMBER: {
            char number[32];
            int length = formatNumber(AS_NUMBER(value), number, sizeof(number));
            bufferAppend(buffer, number, (size_t)length);
            return;
        }
        case VAL_OBJ:
            break;
    }

    switch (OBJ_TYPE(value)) {
        case OBJ_STRING:
            if (quoteStrings) {
                appendQuoted(buffer, AS_STRING(value));
            } else {
                bufferAppend(buffer, AS_STRING(value)->chars, (size_t)AS_STRING(value)->length);
            }
            break;
        case OBJ_LIST: {
            ObjList *list = AS_LIST(value);
            if (depth > MAX_NESTING) {
                bufferAppendText(buffer, "[...]");
                break;
            }
            bufferAppend(buffer, "[", 1);
            for (int i = 0; i < list->count; i++) {
                if (i > 0) bufferAppend(buffer, ", ", 2);
                appendValue(buffer, list->items[i], true, depth + 1);
            }
            bufferAppend(buffer, "]", 1);
            break;
        }
        case OBJ_MAP: {
            ObjMap *map = AS_MAP(value);
            if (depth > MAX_NESTING) {
                bufferAppendText(buffer, "{...}");
                break;
            }
            bufferAppend(buffer, "{", 1);
            bool first = true;
            for (int i = 0; i < map->used; i++) {
                if (!map->entries[i].live) continue;
                if (!first) bufferAppend(buffer, ", ", 2);
                first = false;
                appendValue(buffer, map->entries[i].key, true, depth + 1);
                bufferAppend(buffer, ": ", 2);
                appendValue(buffer, map->entries[i].value, true, depth + 1);
            }
            bufferAppend(buffer, "}", 1);
            break;
        }
        case OBJ_CLOSURE: {
            ObjString *name = AS_CLOSURE(value)->function->name;
            appendFunction(buffer, name == NULL ? NULL : name->chars);
            break;
        }
        case OBJ_FUNCTION: {
            ObjString *name = AS_FUNCTION(value)->name;
            appendFunction(buffer, name == NULL ? NULL : name->chars);
            break;
        }
        case OBJ_NATIVE:
            appendFunction(buffer, AS_NATIVE(value)->name);
            break;
        case OBJ_BOUND_METHOD: {
            ObjString *name = AS_BOUND_METHOD(value)->method->function->name;
            appendFunction(buffer, name == NULL ? NULL : name->chars);
            break;
        }
        case OBJ_CLASS:
            bufferAppendText(buffer, "<sınıf ");
            bufferAppendText(buffer, AS_CLASS(value)->name->chars);
            bufferAppend(buffer, ">", 1);
            break;
        case OBJ_INSTANCE:
            bufferAppend(buffer, "<", 1);
            bufferAppendText(buffer, AS_INSTANCE(value)->klass->name->chars);
            bufferAppendText(buffer, " nesnesi>");
            break;
        case OBJ_MODULE:
            bufferAppendText(buffer, "<modül ");
            bufferAppendText(buffer, AS_MODULE(value)->name->chars);
            bufferAppend(buffer, ">", 1);
            break;
        case OBJ_UPVALUE:
            bufferAppendText(buffer, "<üst değer>");
            break;
    }
}

char *valueToChars(Value value, bool quoteStrings, int *length) {
    TextBuffer buffer = {NULL, 0, 0};
    bufferAppend(&buffer, "", 0);
    appendValue(&buffer, value, quoteStrings, 0);
    if (length != NULL) *length = (int)buffer.length;
    return buffer.data;
}

void printValue(FILE *out, Value value, bool quoteStrings) {
    int length;
    char *chars = valueToChars(value, quoteStrings, &length);
    fwrite(chars, 1, (size_t)length, out);
    free(chars);
}
