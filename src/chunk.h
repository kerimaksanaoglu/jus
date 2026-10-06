#ifndef JUS_CHUNK_H
#define JUS_CHUNK_H

#include "common.h"
#include "value.h"

/* Bayt kodu komutları. Parantez içindekiler komutu izleyen işlenenlerdir. */
typedef enum {
    OP_CONSTANT,       /* (u16 sabit) */
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_GET_LOCAL,      /* (u8 yuva) */
    OP_SET_LOCAL,      /* (u8 yuva) */
    OP_GET_GLOBAL,     /* (u16 ad sabiti) */
    OP_DEFINE_GLOBAL,  /* (u16 ad sabiti) */
    OP_SET_GLOBAL,     /* (u16 ad sabiti) */
    OP_GET_UPVALUE,    /* (u8 dizin) */
    OP_SET_UPVALUE,    /* (u8 dizin) */
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_NOT,
    OP_NEGATE,
    OP_ASSERT_BOOL,    /* yığının tepesi mantıksal değilse hata */
    OP_ECHO,           /* etkileşimli kipte ifade sonucunu yazar */
    OP_JUMP,           /* (u16 uzaklık) */
    OP_JUMP_IF_FALSE,  /* (u16 uzaklık) koşulu yığından alır */
    OP_AND,            /* (u16 uzaklık) yanlışsa atlar, değilse yığından alır */
    OP_OR,             /* (u16 uzaklık) doğruysa atlar, değilse yığından alır */
    OP_LOOP,           /* (u16 geri uzaklık) */
    OP_CALL,           /* (u8 argüman sayısı) */
    OP_CLOSURE,        /* (u16 fonksiyon sabiti, ardından her üst değer için u8 yerel mi, u8 dizin) */
    OP_CLOSE_UPVALUE,
    OP_RETURN
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t *code;
    int *lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);

#endif
