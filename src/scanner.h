#ifndef JUS_SCANNER_H
#define JUS_SCANNER_H

typedef enum {
    /* Noktalama ve işleçler */
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACKET, TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE, TOKEN_COMMA, TOKEN_COLON, TOKEN_DOT,
    TOKEN_PLUS_EQUAL, TOKEN_MINUS_EQUAL, TOKEN_STAR_EQUAL, TOKEN_SLASH_EQUAL,
    TOKEN_MINUS, TOKEN_PLUS, TOKEN_SLASH, TOKEN_STAR, TOKEN_PERCENT,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL, TOKEN_BANG_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL,
    /* Değerler */
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    /* Anahtar kelimeler */
    TOKEN_AND,      /* ve */
    TOKEN_AS,       /* olarak */
    TOKEN_BREAK,    /* kır */
    TOKEN_CATCH,    /* yakala */
    TOKEN_CONTINUE, /* devam */
    TOKEN_ELSE,     /* değilse */
    TOKEN_FALSE,    /* yanlış */
    TOKEN_FOR,      /* her */
    TOKEN_FUNCTION, /* fonksiyon */
    TOKEN_IF,       /* eğer */
    TOKEN_IMPORT,   /* kullan */
    TOKEN_IN,       /* içinde */
    TOKEN_NIL,      /* boş */
    TOKEN_NOT,      /* değil */
    TOKEN_OR,       /* veya */
    TOKEN_RETURN,   /* dön */
    TOKEN_THROW,    /* fırlat */
    TOKEN_TRUE,     /* doğru */
    TOKEN_TRY,      /* dene */
    TOKEN_VAR,      /* değişken */
    TOKEN_WHILE,    /* iken */
    /* Satır ve blok yapısı */
    TOKEN_NEWLINE, TOKEN_INDENT, TOKEN_DEDENT,

    TOKEN_ERROR, TOKEN_EOF
} TokenKind;

typedef struct {
    TokenKind type;
    const char *start; /* kaynak metindeki konum */
    int length;
    int line;
    const char *message; /* yalnızca TOKEN_ERROR için */
} Token;

void initScanner(const char *source);
Token scanToken(void);
const char *scannerSource(void);

#endif
