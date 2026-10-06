#include <string.h>

#include "common.h"
#include "scanner.h"

#define MAX_INDENT_LEVELS 64

typedef struct {
    const char *source;
    const char *start;
    const char *current;
    int line;
    bool atLineStart;
    int parenDepth; /* (), [] ve {} içinde satır sonları ve girinti yok sayılır */
    int indentStack[MAX_INDENT_LEVELS];
    int indentCount;
    int pendingDedents;
    char indentChar; /* dosyada kullanılan girinti karakteri; henüz görülmediyse 0 */
} Scanner;

static Scanner scanner;

typedef struct {
    const char *text;
    TokenKind type;
} Keyword;

static const Keyword keywords[] = {
    {"ve", TOKEN_AND},
    {"kır", TOKEN_BREAK},
    {"yakala", TOKEN_CATCH},
    {"fırlat", TOKEN_THROW},
    {"dene", TOKEN_TRY},
    {"devam", TOKEN_CONTINUE},
    {"değilse", TOKEN_ELSE},
    {"yanlış", TOKEN_FALSE},
    {"her", TOKEN_FOR},
    {"fonksiyon", TOKEN_FUNCTION},
    {"eğer", TOKEN_IF},
    {"içinde", TOKEN_IN},
    {"boş", TOKEN_NIL},
    {"değil", TOKEN_NOT},
    {"veya", TOKEN_OR},
    {"dön", TOKEN_RETURN},
    {"doğru", TOKEN_TRUE},
    {"değişken", TOKEN_VAR},
    {"iken", TOKEN_WHILE},
};

void initScanner(const char *source) {
    scanner.source = source;
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.atLineStart = true;
    scanner.parenDepth = 0;
    scanner.indentStack[0] = 0;
    scanner.indentCount = 1;
    scanner.pendingDedents = 0;
    scanner.indentChar = 0;
}

const char *scannerSource(void) {
    return scanner.source;
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

/* ASCII harfler, alt çizgi ve tüm UTF-8 çok baytlı karakterler ad içinde geçebilir. */
static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
           (unsigned char)c >= 0x80;
}

static bool isAtEnd(void) {
    return *scanner.current == '\0';
}

static char advance(void) {
    scanner.current++;
    return scanner.current[-1];
}

static char peek(void) {
    return *scanner.current;
}

static char peekNext(void) {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

static Token makeToken(TokenKind type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    token.message = NULL;
    return token;
}

/* Kaynakta karşılığı olmayan belirteçler (INDENT, DEDENT, dosya sonu satır sonu). */
static Token syntheticToken(TokenKind type) {
    scanner.start = scanner.current;
    return makeToken(type);
}

static Token errorToken(const char *message) {
    Token token = makeToken(TOKEN_ERROR);
    token.message = message;
    return token;
}

/*
 * Satır başındaki girintiyi ölçer. Boş satırlar ve yalnızca yorum içeren
 * satırlar atlanır. Girinti düzeyi değiştiyse *out doldurulur ve true döner.
 */
static bool scanIndentation(Token *out) {
    for (;;) {
        int indent = 0;
        bool sawSpace = false;
        bool sawTab = false;
        while (peek() == ' ' || peek() == '\t') {
            if (peek() == ' ') sawSpace = true;
            else sawTab = true;
            indent++;
            scanner.current++;
        }

        if (peek() == '\r') scanner.current++;
        if (peek() == '\n') {
            scanner.current++;
            scanner.line++;
            continue;
        }
        if (peek() == '#') {
            while (peek() != '\n' && !isAtEnd()) scanner.current++;
            continue;
        }
        if (isAtEnd()) return false;

        scanner.atLineStart = false;
        scanner.start = scanner.current;

        if (sawSpace && sawTab) {
            *out = errorToken("Girintide boşluk ve sekme birlikte kullanılamaz.");
            return true;
        }
        if (indent > 0) {
            char used = sawTab ? '\t' : ' ';
            if (scanner.indentChar == 0) {
                scanner.indentChar = used;
            } else if (scanner.indentChar != used) {
                *out = errorToken("Girinti dosya boyunca ya yalnızca boşlukla ya da "
                                  "yalnızca sekmeyle yapılmalıdır.");
                return true;
            }
        }

        int top = scanner.indentStack[scanner.indentCount - 1];
        if (indent > top) {
            if (scanner.indentCount == MAX_INDENT_LEVELS) {
                *out = errorToken("Çok fazla iç içe blok var.");
                return true;
            }
            scanner.indentStack[scanner.indentCount++] = indent;
            *out = makeToken(TOKEN_INDENT);
            return true;
        }
        if (indent < top) {
            int dedents = 0;
            while (scanner.indentCount > 1 &&
                   scanner.indentStack[scanner.indentCount - 1] > indent) {
                scanner.indentCount--;
                dedents++;
            }
            if (scanner.indentStack[scanner.indentCount - 1] != indent) {
                *out = errorToken("Girinti, önceki blok düzeylerinden hiçbiriyle eşleşmiyor.");
                return true;
            }
            scanner.pendingDedents = dedents - 1;
            *out = makeToken(TOKEN_DEDENT);
            return true;
        }
        return false;
    }
}

static void skipWhitespace(void) {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                if (scanner.parenDepth == 0) return;
                scanner.line++;
                advance();
                break;
            case '#':
                while (peek() != '\n' && !isAtEnd()) advance();
                break;
            default:
                return;
        }
    }
}

static TokenKind identifierType(void) {
    size_t length = (size_t)(scanner.current - scanner.start);
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strlen(keywords[i].text) == length &&
            memcmp(scanner.start, keywords[i].text, length) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier(void) {
    while (isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

static Token number(void) {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static Token string(void) {
    while (peek() != '"' && peek() != '\n' && !isAtEnd()) {
        if (peek() == '\\' && peekNext() != '\n' && peekNext() != '\0') advance();
        advance();
    }

    if (peek() != '"') {
        return errorToken("Metin kapatılmamış; kapanış tırnağı (\") eksik.");
    }

    advance();
    return makeToken(TOKEN_STRING);
}

Token scanToken(void) {
    if (scanner.pendingDedents > 0) {
        scanner.pendingDedents--;
        return syntheticToken(TOKEN_DEDENT);
    }

    if (scanner.atLineStart && scanner.parenDepth == 0) {
        Token token;
        if (scanIndentation(&token)) return token;
    }

    skipWhitespace();
    scanner.start = scanner.current;

    if (isAtEnd()) {
        /* Dosya sonunda son satırı kapat, açık blokları kapat. */
        if (!scanner.atLineStart) {
            scanner.atLineStart = true;
            return syntheticToken(TOKEN_NEWLINE);
        }
        if (scanner.indentCount > 1) {
            scanner.indentCount--;
            return syntheticToken(TOKEN_DEDENT);
        }
        return syntheticToken(TOKEN_EOF);
    }

    char c = advance();
    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '\n': {
            Token token = makeToken(TOKEN_NEWLINE);
            scanner.line++;
            scanner.atLineStart = true;
            return token;
        }
        case '(':
            scanner.parenDepth++;
            return makeToken(TOKEN_LEFT_PAREN);
        case ')':
            if (scanner.parenDepth > 0) scanner.parenDepth--;
            return makeToken(TOKEN_RIGHT_PAREN);
        case '[':
            scanner.parenDepth++;
            return makeToken(TOKEN_LEFT_BRACKET);
        case ']':
            if (scanner.parenDepth > 0) scanner.parenDepth--;
            return makeToken(TOKEN_RIGHT_BRACKET);
        case '{':
            scanner.parenDepth++;
            return makeToken(TOKEN_LEFT_BRACE);
        case '}':
            if (scanner.parenDepth > 0) scanner.parenDepth--;
            return makeToken(TOKEN_RIGHT_BRACE);
        case ',': return makeToken(TOKEN_COMMA);
        case ':': return makeToken(TOKEN_COLON);
        case '-': return makeToken(match('=') ? TOKEN_MINUS_EQUAL : TOKEN_MINUS);
        case '+': return makeToken(match('=') ? TOKEN_PLUS_EQUAL : TOKEN_PLUS);
        case '/': return makeToken(match('=') ? TOKEN_SLASH_EQUAL : TOKEN_SLASH);
        case '*': return makeToken(match('=') ? TOKEN_STAR_EQUAL : TOKEN_STAR);
        case '%': return makeToken(TOKEN_PERCENT);
        case '!':
            if (match('=')) return makeToken(TOKEN_BANG_EQUAL);
            return errorToken("'!' tek başına kullanılamaz; olumsuzlama için 'değil' yazın.");
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    return errorToken("Beklenmeyen karakter.");
}
