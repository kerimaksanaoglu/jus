#ifndef JUS_TABLE_H
#define JUS_TABLE_H

#include "common.h"
#include "value.h"

/* Anahtarı tekilleştirilmiş metin olan açık adreslemeli karma tablo. */

typedef struct {
    ObjString *key;
    Value value;
} Entry;

typedef struct {
    int count; /* silinmiş yuvalar dahil */
    int capacity;
    Entry *entries;
} Table;

void initTable(Table *table);
void freeTable(Table *table);
bool tableGet(Table *table, ObjString *key, Value *value);
/* Anahtar yeni eklendiyse true döndürür. */
bool tableSet(Table *table, ObjString *key, Value value);
bool tableDelete(Table *table, ObjString *key);
void tableAddAll(Table *from, Table *to);
ObjString *tableFindString(Table *table, const char *chars, int length, uint32_t hash);
void tableRemoveWhite(Table *table);
void markTable(Table *table);

#endif
