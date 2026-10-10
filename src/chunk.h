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
    OP_BIT_AND,        /* & */
    OP_BIT_OR,         /* | */
    OP_BIT_XOR,        /* ^ */
    OP_BIT_NOT,        /* ~ */
    OP_SHIFT_LEFT,     /* << */
    OP_SHIFT_RIGHT,    /* >> */
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
    OP_BUILD_LIST,     /* (u16 öğe sayısı) */
    OP_BUILD_MAP,      /* (u16 çift sayısı) */
    OP_GET_INDEX,      /* [kap, dizin] -> değer */
    OP_SET_INDEX,      /* [kap, dizin, değer] -> değer */
    OP_SLICE,          /* [kap, baş, son] -> dilim; verilmeyen uç boş */
    OP_DUP2,           /* yığının tepesindeki iki değeri çoğaltır */
    OP_IN,             /* [öğe, kap] -> mantıksal */
    OP_FOR_NEXT,       /* (u16 uzaklık) [kap, imleç] -> sıradaki öğeyi ekler ya da atlar */
    OP_TRY_BEGIN,      /* (u16 uzaklık) hata yakalayıcı kurar; uzaklık 'yakala' bloğunu gösterir */
    OP_TRY_END,        /* en son kurulan yakalayıcıyı kaldırır */
    OP_THROW,          /* yığının tepesindeki değeri hata olarak fırlatır */
    OP_IMPORT,         /* (u16 ad sabiti) modülü yükler; gerekiyorsa üst düzey kodunu çağırır */
    OP_MODULE,         /* (u16 ad sabiti) yüklenmiş modülü yığına koyar */
    OP_GET_PROPERTY,   /* (u16 ad sabiti) [nesne] -> üye */
    OP_SET_PROPERTY,   /* (u16 ad sabiti) [nesne, değer] -> değer */
    OP_INVOKE,         /* (u16 ad sabiti, u8 argüman sayısı) nesne.ad(...) çağrısı */
    OP_CLASS,          /* (u16 ad sabiti) yeni sınıf oluşturur */
    OP_INHERIT,        /* [üst sınıf, sınıf] -> [üst sınıf]; yöntemleri devralır */
    OP_METHOD,         /* (u16 ad sabiti) [sınıf, fonksiyon] -> [sınıf] */
    OP_GET_SUPER,      /* (u16 ad sabiti) [bu, üst sınıf] -> bağlı yöntem */
    OP_SUPER_INVOKE,   /* (u16 ad sabiti, u8 argüman sayısı) üst.ad(...) çağrısı */
    OP_DUP,            /* yığının tepesindeki değeri çoğaltır */
    OP_ARG_GIVEN,      /* (u8 parametre sırası, u16 uzaklık) argüman verildiyse atlar; verilmediyse varsayılan değer kodu izler */
    OP_BUILD_STRING,   /* (u16 parça sayısı) parçaları metne çevirip birleştirir (biçimli metin) */
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
