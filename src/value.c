#include <math.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"

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

bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL: return true;
        case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
        /* Metinler tekilleştirildiği için işaretçi karşılaştırması yeterli. */
        case VAL_OBJ: return AS_OBJ(a) == AS_OBJ(b);
    }
    return false;
}

const char *valueTypeName(Value value) {
    switch (value.type) {
        case VAL_BOOL: return "mantıksal";
        case VAL_NIL: return "boş";
        case VAL_NUMBER: return "sayı";
        case VAL_OBJ:
            switch (OBJ_TYPE(value)) {
                case OBJ_STRING: return "metin";
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
    if (number == floor(number) && fabs(number) < 1e15) {
        if (number == 0) number = 0; /* -0 yerine 0 */
        return snprintf(buffer, size, "%.0f", number);
    }
    return snprintf(buffer, size, "%.14g", number);
}

void printValue(FILE *out, Value value, bool quoteStrings) {
    switch (value.type) {
        case VAL_BOOL:
            fputs(AS_BOOL(value) ? "doğru" : "yanlış", out);
            break;
        case VAL_NIL:
            fputs("boş", out);
            break;
        case VAL_NUMBER: {
            char buffer[32];
            formatNumber(AS_NUMBER(value), buffer, sizeof(buffer));
            fputs(buffer, out);
            break;
        }
        case VAL_OBJ:
            printObject(out, value, quoteStrings);
            break;
    }
}
