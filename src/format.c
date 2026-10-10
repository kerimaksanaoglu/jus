/*
 * JUS kaynak kodu biçimlendiricisi.
 *
 * Çalışma düzeni: kaynak önce sözcük çözümleyiciyle doğrulanır (hatalı kaynak
 * biçimlendirilmez). Ardından satır satır, yorumları da gören hafif bir
 * sözcük ayrıştırmasıyla yeniden yazılır. Belirteçlerin kendisi (ad, sayı,
 * metin, yorum içeriği) değiştirilmez; yalnızca aralarındaki boşluklar,
 * girintiler ve boş satırlar yeniden düzenlenir.
 */

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "format.h"
#include "scanner.h"

#define INDENT_WIDTH 4
#define MAX_BLANK_LINES 2
#define MAX_INDENT_LEVELS 256

/* ---- Büyüyen tampon ---- */

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
    bool failed;
} Buffer;

static void bufferReserve(Buffer *b, size_t extra) {
    if (b->failed) return;
    if (b->length + extra + 1 <= b->capacity) return;
    size_t capacity = b->capacity == 0 ? 256 : b->capacity;
    while (capacity < b->length + extra + 1) capacity *= 2;
    char *data = (char *)realloc(b->data, capacity);
    if (data == NULL) {
        b->failed = true;
        return;
    }
    b->data = data;
    b->capacity = capacity;
}

static void bufferAppend(Buffer *b, const char *text, size_t length) {
    bufferReserve(b, length);
    if (b->failed) return;
    if (length > 0) memcpy(b->data + b->length, text, length);
    b->length += length;
    b->data[b->length] = '\0';
}

static void bufferChar(Buffer *b, char c) {
    bufferAppend(b, &c, 1);
}

static void bufferRepeat(Buffer *b, char c, int count) {
    for (int i = 0; i < count; i++) bufferChar(b, c);
}

/* ---- Çıktı satırları ---- */

typedef struct {
    size_t offset;  /* metin tamponundaki başlangıç */
    size_t length;
    int indent;     /* girinti, boşluk sayısı */
    bool blank;
    bool pending;   /* girintisi sonraki kod satırına göre belirlenecek yorum */
} Line;

typedef struct {
    Line *items;
    int count;
    int capacity;
    bool failed;
} LineList;

static Line *lineAdd(LineList *list) {
    if (list->failed) return NULL;
    if (list->count == list->capacity) {
        int capacity = list->capacity == 0 ? 64 : list->capacity * 2;
        Line *items = (Line *)realloc(list->items, sizeof(Line) * (size_t)capacity);
        if (items == NULL) {
            list->failed = true;
            return NULL;
        }
        list->items = items;
        list->capacity = capacity;
    }
    Line *line = &list->items[list->count++];
    memset(line, 0, sizeof(Line));
    return line;
}

/* ---- Belirteç ayrıştırma ---- */

typedef struct {
    const char *text;
    TokenKind type;
} Keyword;

static const Keyword keywords[] = {
    {"ve", TOKEN_AND},
    {"olarak", TOKEN_AS},
    {"kullan", TOKEN_IMPORT},
    {"kır", TOKEN_BREAK},
    {"geç", TOKEN_PASS},
    {"sınıf", TOKEN_CLASS},
    {"üst", TOKEN_SUPER},
    {"bu", TOKEN_THIS},
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

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

/* ASCII harfler, alt çizgi ve tüm UTF-8 çok baytlı karakterler ad içinde geçebilir. */
static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
           (unsigned char)c >= 0x80;
}

static TokenKind identifierKind(const char *start, size_t length) {
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strlen(keywords[i].text) == length && memcmp(start, keywords[i].text, length) == 0) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

/* Satırın p konumundaki belirteci okur; uzunluğunu ve türünü verir. */
static TokenKind readToken(const char *p, const char *end, size_t *length) {
    const char *start = p;
    char c = *p++;
    if (isAlpha(c)) {
        while (p < end && (isAlpha(*p) || isDigit(*p))) p++;
        /* f"..." biçimli metin tek bir belirteçtir. */
        if (p - start == 1 && c == 'f' && p < end && *p == '"') {
            p++;
            while (p < end && *p != '"') {
                if (*p == '\\' && p + 1 < end) p++;
                p++;
            }
            if (p < end) p++;
            *length = (size_t)(p - start);
            return TOKEN_STRING;
        }
        *length = (size_t)(p - start);
        return identifierKind(start, *length);
    }
    if (isDigit(c)) {
        while (p < end && isDigit(*p)) p++;
        if (p + 1 < end && *p == '.' && isDigit(p[1])) {
            p++;
            while (p < end && isDigit(*p)) p++;
        }
        *length = (size_t)(p - start);
        return TOKEN_NUMBER;
    }
    if (c == '"') {
        while (p < end && *p != '"') {
            if (*p == '\\' && p + 1 < end) p++;
            p++;
        }
        if (p < end) p++;
        *length = (size_t)(p - start);
        return TOKEN_STRING;
    }

    bool eq = p < end && *p == '=';
    TokenKind kind = TOKEN_ERROR;
    switch (c) {
        case '(': kind = TOKEN_LEFT_PAREN; break;
        case ')': kind = TOKEN_RIGHT_PAREN; break;
        case '[': kind = TOKEN_LEFT_BRACKET; break;
        case ']': kind = TOKEN_RIGHT_BRACKET; break;
        case '{': kind = TOKEN_LEFT_BRACE; break;
        case '}': kind = TOKEN_RIGHT_BRACE; break;
        case ',': kind = TOKEN_COMMA; break;
        case '.': kind = TOKEN_DOT; break;
        case ':': kind = TOKEN_COLON; break;
        case '%': kind = TOKEN_PERCENT; break;
        case '&': kind = TOKEN_AMPERSAND; break;
        case '|': kind = TOKEN_PIPE; break;
        case '^': kind = TOKEN_CARET; break;
        case '~': kind = TOKEN_TILDE; break;
        case '-': kind = eq ? TOKEN_MINUS_EQUAL : TOKEN_MINUS; break;
        case '+': kind = eq ? TOKEN_PLUS_EQUAL : TOKEN_PLUS; break;
        case '/': kind = eq ? TOKEN_SLASH_EQUAL : TOKEN_SLASH; break;
        case '*': kind = eq ? TOKEN_STAR_EQUAL : TOKEN_STAR; break;
        case '=': kind = eq ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL; break;
        case '<': kind = (p < end && *p == '<') ? TOKEN_SHIFT_LEFT : eq ? TOKEN_LESS_EQUAL : TOKEN_LESS; break;
        case '>': kind = (p < end && *p == '>') ? TOKEN_SHIFT_RIGHT : eq ? TOKEN_GREATER_EQUAL : TOKEN_GREATER; break;
        case '!': kind = eq ? TOKEN_BANG_EQUAL : TOKEN_ERROR; break;
        default: break;
    }
    bool twoChar = kind == TOKEN_MINUS_EQUAL || kind == TOKEN_PLUS_EQUAL ||
                   kind == TOKEN_SLASH_EQUAL || kind == TOKEN_STAR_EQUAL ||
                   kind == TOKEN_EQUAL_EQUAL || kind == TOKEN_LESS_EQUAL ||
                   kind == TOKEN_GREATER_EQUAL || kind == TOKEN_BANG_EQUAL ||
                   kind == TOKEN_SHIFT_LEFT || kind == TOKEN_SHIFT_RIGHT;
    if (twoChar) p++;
    *length = (size_t)(p - start);
    return kind;
}

static bool isOpener(TokenKind k) {
    return k == TOKEN_LEFT_PAREN || k == TOKEN_LEFT_BRACKET || k == TOKEN_LEFT_BRACE;
}

static bool isCloser(TokenKind k) {
    return k == TOKEN_RIGHT_PAREN || k == TOKEN_RIGHT_BRACKET || k == TOKEN_RIGHT_BRACE;
}

/* Bir değeri bitiren belirteç mi? Ardından gelen '-' bu durumda ikilidir. */
static bool endsValue(TokenKind k) {
    switch (k) {
        case TOKEN_IDENTIFIER: case TOKEN_NUMBER: case TOKEN_STRING:
        case TOKEN_RIGHT_PAREN: case TOKEN_RIGHT_BRACKET: case TOKEN_RIGHT_BRACE:
        case TOKEN_TRUE: case TOKEN_FALSE: case TOKEN_NIL: case TOKEN_THIS:
            return true;
        default:
            return false;
    }
}

/* Ardından '(' ya da '[' geldiğinde çağrı/dizinleme sayılan belirteç mi? */
static bool isCallee(TokenKind k) {
    switch (k) {
        case TOKEN_IDENTIFIER: case TOKEN_STRING: case TOKEN_THIS: case TOKEN_SUPER:
        case TOKEN_RIGHT_PAREN: case TOKEN_RIGHT_BRACKET: case TOKEN_RIGHT_BRACE:
            return true;
        default:
            return false;
    }
}

/* ---- Biçimlendirici ---- */

typedef struct {
    bool hasPrev;
    TokenKind prev;
    bool prevUnary;   /* önceki belirteç tekli eksi */
    char prevColon;   /* önceki belirteç ':' ise türü: 'B' blok, 'D' sözlük, 'S' dilim */
    char *brackets;   /* açık parantezlerin yığını */
    int depth;
    int bracketCap;
    int stmtLevel;    /* içinde bulunulan deyimin blok düzeyi */
} State;

typedef struct {
    Buffer text;      /* tüm satırların girintisiz metinleri */
    LineList lines;
    State st;
    int indentStack[MAX_INDENT_LEVELS];
    int indentCount;
    int pendingFrom;  /* girintisi bekleyen ilk yorum satırının dizini, yoksa -1 */
    bool failed;
} Formatter;

/* Belirteçten önce boşluk gerekir mi? (Satırın ilk belirteci için çağrılmaz.) */
static bool wantSpace(const State *st, TokenKind cur) {
    if (!st->hasPrev) return false;
    if (cur == TOKEN_COMMA || cur == TOKEN_DOT || cur == TOKEN_COLON || isCloser(cur)) {
        /* "1 .5" gibi tutarsız yazımlarda sayı ile nokta birleşmesin. */
        return cur == TOKEN_DOT && st->prev == TOKEN_NUMBER;
    }
    if (isOpener(st->prev)) return false;
    if (st->prev == TOKEN_DOT) return cur == TOKEN_NUMBER;
    if (st->prevUnary) return false;
    if (st->prev == TOKEN_COLON) return st->prevColon != 'S';
    if (cur == TOKEN_LEFT_PAREN || cur == TOKEN_LEFT_BRACKET) return !isCallee(st->prev);
    return true;
}

/* Yorumu ('#' ile başlayan aralık) standart yazıma çevirir; sondaki boşluklar atılır. */
static void appendComment(Buffer *out, const char *start, const char *end) {
    while (end > start && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r')) end--;
    bufferChar(out, '#');
    start++;
    if (start < end && *start != ' ' && *start != '!' && *start != '#') bufferChar(out, ' ');
    bufferAppend(out, start, (size_t)(end - start));
}

static bool pushBracket(State *st, char c) {
    if (st->depth == st->bracketCap) {
        int capacity = st->bracketCap == 0 ? 32 : st->bracketCap * 2;
        char *brackets = (char *)realloc(st->brackets, (size_t)capacity);
        if (brackets == NULL) return false;
        st->brackets = brackets;
        st->bracketCap = capacity;
    }
    st->brackets[st->depth++] = c;
    return true;
}

static void resolvePending(Formatter *f, int level) {
    if (f->pendingFrom < 0) return;
    for (int i = f->pendingFrom; i < f->lines.count; i++) {
        Line *line = &f->lines.items[i];
        if (line->pending) {
            line->indent = level * INDENT_WIDTH;
            line->pending = false;
        }
    }
    f->pendingFrom = -1;
}

/* Bir fiziksel satırı işler: [p, end) satır sonu hariç satırdır. */
static void formatLine(Formatter *f, const char *p, const char *end) {
    State *st = &f->st;
    int leading = 0;
    while (p < end && (*p == ' ' || *p == '\t' || *p == '\r')) {
        if (*p != '\r') leading++;
        p++;
    }

    Line *line = lineAdd(&f->lines);
    if (line == NULL) {
        f->failed = true;
        return;
    }
    line->offset = f->text.length;

    if (p == end) {
        line->blank = true;
        return;
    }

    if (*p == '#') {
        appendComment(&f->text, p, end);
        line->length = f->text.length - line->offset;
        if (st->depth > 0) {
            line->indent = (st->stmtLevel + st->depth) * INDENT_WIDTH;
        } else {
            line->pending = true;
            if (f->pendingFrom < 0) f->pendingFrom = f->lines.count - 1;
        }
        return;
    }

    /* Kod satırı. Parantez dışındaysa yeni bir deyim başlar. */
    bool newStatement = st->depth == 0;
    if (newStatement) {
        while (f->indentCount > 1 && f->indentStack[f->indentCount - 1] > leading) {
            f->indentCount--;
        }
        if (f->indentStack[f->indentCount - 1] < leading && f->indentCount < MAX_INDENT_LEVELS) {
            f->indentStack[f->indentCount++] = leading;
        }
        st->stmtLevel = f->indentCount - 1;
        st->hasPrev = false;
        st->prevUnary = false;
        st->prevColon = 0;
        line->indent = st->stmtLevel * INDENT_WIDTH;
        resolvePending(f, st->stmtLevel);
    }

    bool first = true;
    while (p < end) {
        if (*p == ' ' || *p == '\t' || *p == '\r') {
            p++;
            continue;
        }
        if (*p == '#') {
            bufferAppend(&f->text, "  ", 2);
            appendComment(&f->text, p, end);
            break;
        }

        size_t length;
        TokenKind kind = readToken(p, end, &length);
        if (first && !newStatement) {
            /* Devam satırı: yalnızca kapatan parantezle başlıyorsa bir düzey dışarıda. */
            int depth = st->depth - (isCloser(kind) && st->depth > 0 ? 1 : 0);
            line->indent = (st->stmtLevel + depth) * INDENT_WIDTH;
        }
        if (!first && wantSpace(st, kind)) bufferChar(&f->text, ' ');

        /* Belirtecin kaynak yazımı olduğu gibi korunur. */
        bufferAppend(&f->text, p, length);
        first = false;

        bool unary = false;
        char colon = 0;
        /* Tekli eksi ve parametre listesindeki '*ad' işaretinden sonra boşluk konmaz. */
        if (kind == TOKEN_MINUS || kind == TOKEN_STAR) unary = !(st->hasPrev && endsValue(st->prev));
        if (kind == TOKEN_TILDE) unary = true;
        if (kind == TOKEN_COLON) {
            colon = st->depth == 0 ? 'B' : (st->brackets[st->depth - 1] == '[' ? 'S' : 'D');
        }
        if (isOpener(kind)) {
            if (!pushBracket(st, *p)) {
                f->failed = true;
                return;
            }
        } else if (isCloser(kind) && st->depth > 0) {
            st->depth--;
        }
        st->hasPrev = true;
        st->prev = kind;
        st->prevUnary = unary;
        st->prevColon = colon;
        p += length;
    }
    line->length = f->text.length - line->offset;
}

static char *failWith(const char **problem, int *line, const char *message, int number) {
    if (problem != NULL) *problem = message;
    if (line != NULL) *line = number;
    return NULL;
}

/* Satırları birleştirir: baştaki boş satırlar atılır, ardışık boşlar sıkıştırılır. */
static char *assemble(const Formatter *f) {
    Buffer out;
    memset(&out, 0, sizeof(out));
    bufferAppend(&out, "", 0);
    int blanks = 0;
    bool any = false;
    for (int i = 0; i < f->lines.count; i++) {
        const Line *l = &f->lines.items[i];
        if (l->blank) {
            if (any) blanks++;
            continue;
        }
        bufferRepeat(&out, '\n', blanks > MAX_BLANK_LINES ? MAX_BLANK_LINES : blanks);
        blanks = 0;
        bufferRepeat(&out, ' ', l->indent);
        bufferAppend(&out, f->text.data + l->offset, l->length);
        bufferChar(&out, '\n');
        any = true;
    }
    if (out.failed) {
        free(out.data);
        return NULL;
    }
    return out.data;
}

char *formatSource(const char *source, const char **problem, int *line) {
    /* BOM atılır, CRLF satır sonları LF yapılır. */
    size_t sourceLength = strlen(source);
    if (sourceLength >= 3 && (unsigned char)source[0] == 0xEF &&
        (unsigned char)source[1] == 0xBB && (unsigned char)source[2] == 0xBF) {
        source += 3;
        sourceLength -= 3;
    }
    char *norm = (char *)malloc(sourceLength + 1);
    if (norm == NULL) return failWith(problem, line, "Bellek yetersiz.", 0);
    size_t n = 0;
    for (size_t i = 0; i < sourceLength; i++) {
        if (source[i] == '\r' && (i + 1 == sourceLength || source[i + 1] == '\n')) continue;
        norm[n++] = source[i];
    }
    norm[n] = '\0';

    /* Sözcük düzeyinde hata varsa biçimlendirme yapılmaz. */
    initScanner(norm);
    for (;;) {
        Token token = scanToken();
        if (token.type == TOKEN_ERROR) {
            free(norm);
            return failWith(problem, line, token.message, token.line);
        }
        if (token.type == TOKEN_EOF) break;
    }

    Formatter f;
    memset(&f, 0, sizeof(f));
    f.indentCount = 1;
    f.pendingFrom = -1;

    const char *p = norm;
    const char *stop = norm + n;
    while (p < stop && !f.failed) {
        const char *eol = (const char *)memchr(p, '\n', (size_t)(stop - p));
        const char *end = eol == NULL ? stop : eol;
        formatLine(&f, p, end);
        p = eol == NULL ? stop : eol + 1;
    }
    free(norm);
    resolvePending(&f, 0);

    char *result = NULL;
    if (!f.failed && !f.text.failed && !f.lines.failed) result = assemble(&f);
    free(f.text.data);
    free(f.lines.items);
    free(f.st.brackets);
    if (result == NULL) return failWith(problem, line, "Bellek yetersiz.", 0);
    return result;
}
