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
    TOKEN_AMPERSAND, TOKEN_PIPE, TOKEN_CARET, TOKEN_TILDE, TOKEN_SHIFT_LEFT, TOKEN_SHIFT_RIGHT,
    /* Değerler */
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_FSTRING, TOKEN_NUMBER,
    /* Anahtar kelimeler */
    TOKEN_AND,      /* ve */
    TOKEN_AS,       /* olarak */
    TOKEN_BREAK,    /* kır */
    TOKEN_CATCH,    /* yakala */
    TOKEN_CLASS,    /* sınıf */
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
    TOKEN_PASS,     /* geç */
    TOKEN_RETURN,   /* dön */
    TOKEN_SUPER,    /* üst */
    TOKEN_THIS,     /* bu */
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

#define SCANNER_MAX_INDENT_LEVELS 64

/* Tarayıcının tüm durumu; biçimli metinlerdeki ifadeler için kaydedilip geri yüklenir. */
typedef struct {
    const char *source;
    const char *start;
    const char *current;
    const char *end; /* taranan aralığın sonu; normal taramada kaynağın sonu */
    int line;
    bool atLineStart;
    int parenDepth; /* (), [] ve {} içinde satır sonları ve girinti yok sayılır */
    int indentStack[SCANNER_MAX_INDENT_LEVELS];
    int indentCount;
    int pendingDedents;
    char indentChar; /* dosyada kullanılan girinti karakteri; henüz görülmediyse 0 */
} ScannerState;

void initScanner(const char *source);
/*
 * Kaynağın [start, end) aralığını tek bir ifade olarak tarar (biçimli metin
 * içindeki ifadeler için). Hata konumları yine tüm kaynağa göre hesaplanır.
 */
void initScannerRange(const char *source, const char *start, const char *end, int line);
void scannerSave(ScannerState *out);
void scannerRestore(const ScannerState *saved);
Token scanToken(void);
const char *scannerSource(void);

#endif
