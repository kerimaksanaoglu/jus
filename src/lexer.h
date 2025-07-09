#ifndef JUS_LEXER_H
#define JUS_LEXER_H

#include "../include/common.h"
#include <stddef.h>

/* Lexer yapısı */
typedef struct {
    const char *src;
    size_t pos;
    int line;
} Lexer;

/* Lexer fonksiyon prototipleri */

/**
 * Lexer'ı başlat
 * @param lexer Lexer yapısı pointer'ı
 * @param source Kaynak kod string'i
 */
void lexer_init(Lexer *lexer, const char *source);

/**
 * Kaynak kodun tamamını tokenize et
 * @param lexer Lexer yapısı pointer'ı
 * @param outCount Çıktı token sayısı
 * @return Token dizisi
 */
Token *lexer_scan_all(Lexer *lexer, int *outCount);

/**
 * Token dizisini bellek temizliği için serbest bırak
 * @param tokens Token dizisi
 * @param count Token sayısı
 */
void token_free(Token *tokens, int count);

#endif /* JUS_LEXER_H */