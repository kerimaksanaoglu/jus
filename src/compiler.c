#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "compiler.h"
#include "memory.h"
#include "scanner.h"

/*
 * Tek geçişli derleyici: belirteçleri okurken doğrudan bayt kodu üretir.
 * İfadeler Pratt yöntemiyle (öncelik tırmanma) ayrıştırılır.
 */

#define MAX_LOOP_BREAKS 64

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

/* Düşükten yükseğe işleç öncelikleri. */
typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, /* = */
    PREC_OR,         /* veya */
    PREC_AND,        /* ve */
    PREC_NOT,        /* değil */
    PREC_EQUALITY,   /* == != */
    PREC_COMPARISON, /* < > <= >= */
    PREC_TERM,       /* + - */
    PREC_FACTOR,     /* * / % */
    PREC_UNARY,      /* - */
    PREC_CALL,       /* () */
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)(bool canAssign);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

typedef struct {
    Token name;
    int depth; /* -1: bildirildi ama henüz başlangıç değeri atanmadı */
    bool isCaptured;
} Local;

typedef struct {
    uint8_t index;
    bool isLocal;
} Upvalue;

typedef enum {
    TYPE_FUNCTION,
    TYPE_SCRIPT
} FunctionType;

typedef struct Loop {
    struct Loop *enclosing;
    int start;
    int scopeDepth;
    int breakJumps[MAX_LOOP_BREAKS];
    int breakCount;
} Loop;

typedef struct Compiler {
    struct Compiler *enclosing;
    ObjFunction *function;
    FunctionType type;

    Local locals[UINT8_COUNT];
    int localCount;
    Upvalue upvalues[UINT8_COUNT];
    int scopeDepth;
    Loop *loop;
} Compiler;

static Parser parser;
static Compiler *current = NULL;
static const char *sourceName;
static bool replMode;
/* Son ifade deyimi bir atama mıydı? Etkileşimli kipte atamalar yankılanmaz. */
static bool lastExpressionWasAssignment;

static Chunk *currentChunk(void) {
    return &current->function->chunk;
}

/* ---- Hata bildirimi ---- */

static void errorAt(const Token *token, const char *message) {
    if (parser.panicMode) return;
    parser.panicMode = true;
    parser.hadError = true;

    const char *source = scannerSource();
    const char *lineStart = token->start;
    while (lineStart > source && lineStart[-1] != '\n') lineStart--;
    const char *lineEnd = token->start;
    while (*lineEnd != '\0' && *lineEnd != '\n' && *lineEnd != '\r') lineEnd++;

    /* Sütun, bayt değil karakter (kod noktası) cinsinden sayılır. */
    int column = 1;
    for (const char *p = lineStart; p < token->start; p++) {
        if (((unsigned char)*p & 0xC0) != 0x80) column++;
    }

    fprintf(stderr, "%s:%d:%d: sözdizimi hatası: %s\n", sourceName, token->line, column, message);
    fprintf(stderr, "    %.*s\n    ", (int)(lineEnd - lineStart), lineStart);
    for (const char *p = lineStart; p < token->start; p++) {
        if (((unsigned char)*p & 0xC0) == 0x80) continue;
        fputc(*p == '\t' ? '\t' : ' ', stderr);
    }
    fputs("^\n", stderr);
}

static void error(const char *message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

/* ---- Belirteç akışı ---- */

static void advance(void) {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.message);
    }
}

static void consume(TokenKind type, const char *message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

static bool check(TokenKind type) {
    return parser.current.type == type;
}

static bool match(TokenKind type) {
    if (!check(type)) return false;
    advance();
    return true;
}

/* ---- Bayt kodu üretimi ---- */

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitShort(int value) {
    emitByte((uint8_t)((value >> 8) & 0xff));
    emitByte((uint8_t)(value & 0xff));
}

static void emitLoop(int loopStart) {
    emitByte(OP_LOOP);

    int offset = currentChunk()->count - loopStart + 2;
    if (offset > UINT16_MAX) error("Döngü gövdesi çok büyük.");

    emitShort(offset);
}

static int emitJump(uint8_t instruction) {
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return currentChunk()->count - 2;
}

static void emitReturn(void) {
    emitByte(OP_NIL);
    emitByte(OP_RETURN);
}

static int makeConstant(Value value) {
    /* Aynı sabit (ör. sık kullanılan bir ad) tabloya yalnızca bir kez eklenir. */
    ValueArray *constants = &currentChunk()->constants;
    for (int i = 0; i < constants->count; i++) {
        if (valuesEqual(constants->values[i], value)) return i;
    }

    int constant = addConstant(currentChunk(), value);
    if (constant > UINT16_MAX) {
        error("Tek bir fonksiyonda çok fazla sabit var.");
        return 0;
    }

    return constant;
}

static void emitConstant(Value value) {
    int constant = makeConstant(value);
    emitByte(OP_CONSTANT);
    emitShort(constant);
}

static void patchJump(int offset) {
    /* -2: atlama uzaklığının kendi baytları. */
    int jump = currentChunk()->count - offset - 2;

    if (jump > UINT16_MAX) {
        error("Atlanacak kod bloğu çok büyük.");
    }

    currentChunk()->code[offset] = (uint8_t)((jump >> 8) & 0xff);
    currentChunk()->code[offset + 1] = (uint8_t)(jump & 0xff);
}

static void initCompiler(Compiler *compiler, FunctionType type) {
    compiler->enclosing = current;
    compiler->function = NULL;
    compiler->type = type;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    compiler->loop = NULL;
    compiler->function = newFunction();
    current = compiler;
    if (type != TYPE_SCRIPT) {
        current->function->name = copyString(parser.previous.start, parser.previous.length);
    }

    /* 0 numaralı yuva çağrılan fonksiyonun kendisine ayrılmıştır. */
    Local *local = &current->locals[current->localCount++];
    local->depth = 0;
    local->isCaptured = false;
    local->name.start = "";
    local->name.length = 0;
}

static ObjFunction *endCompiler(void) {
    emitReturn();
    ObjFunction *function = current->function;

    current = current->enclosing;
    return function;
}

static void beginScope(void) {
    current->scopeDepth++;
}

static void endScope(void) {
    current->scopeDepth--;

    while (current->localCount > 0 &&
           current->locals[current->localCount - 1].depth > current->scopeDepth) {
        if (current->locals[current->localCount - 1].isCaptured) {
            emitByte(OP_CLOSE_UPVALUE);
        } else {
            emitByte(OP_POP);
        }
        current->localCount--;
    }
}

/* ---- Değişkenler ---- */

static void expression(void);
static void statement(void);
static void declaration(void);
static const ParseRule *getRule(TokenKind type);
static void parsePrecedence(Precedence precedence);

static int identifierConstant(const Token *name) {
    return makeConstant(OBJ_VAL(copyString(name->start, name->length)));
}

static bool identifiersEqual(const Token *a, const Token *b) {
    if (a->length != b->length) return false;
    return memcmp(a->start, b->start, (size_t)a->length) == 0;
}

static int resolveLocal(Compiler *compiler, const Token *name) {
    for (int i = compiler->localCount - 1; i >= 0; i--) {
        Local *local = &compiler->locals[i];
        if (identifiersEqual(name, &local->name)) {
            if (local->depth == -1) {
                error("Değişken kendi başlangıç değerinin içinde kullanılamaz.");
            }
            return i;
        }
    }

    return -1;
}

static int addUpvalue(Compiler *compiler, uint8_t index, bool isLocal) {
    int upvalueCount = compiler->function->upvalueCount;

    for (int i = 0; i < upvalueCount; i++) {
        Upvalue *upvalue = &compiler->upvalues[i];
        if (upvalue->index == index && upvalue->isLocal == isLocal) {
            return i;
        }
    }

    if (upvalueCount == UINT8_COUNT) {
        error("Bir fonksiyon dış kapsamdan en çok 256 değişken kullanabilir.");
        return 0;
    }

    compiler->upvalues[upvalueCount].isLocal = isLocal;
    compiler->upvalues[upvalueCount].index = index;
    return compiler->function->upvalueCount++;
}

static int resolveUpvalue(Compiler *compiler, const Token *name) {
    if (compiler->enclosing == NULL) return -1;

    int local = resolveLocal(compiler->enclosing, name);
    if (local != -1) {
        compiler->enclosing->locals[local].isCaptured = true;
        return addUpvalue(compiler, (uint8_t)local, true);
    }

    int upvalue = resolveUpvalue(compiler->enclosing, name);
    if (upvalue != -1) {
        return addUpvalue(compiler, (uint8_t)upvalue, false);
    }

    return -1;
}

static void addLocal(Token name) {
    if (current->localCount == UINT8_COUNT) {
        error("Bir fonksiyonda en çok 256 yerel değişken olabilir.");
        return;
    }

    Local *local = &current->locals[current->localCount++];
    local->name = name;
    local->depth = -1;
    local->isCaptured = false;
}

static void declareVariable(void) {
    if (current->scopeDepth == 0) return;

    Token *name = &parser.previous;
    for (int i = current->localCount - 1; i >= 0; i--) {
        Local *local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scopeDepth) {
            break;
        }

        if (identifiersEqual(name, &local->name)) {
            error("Bu kapsamda aynı adla bir değişken zaten tanımlı.");
        }
    }

    addLocal(*name);
}

static int parseVariable(const char *errorMessage) {
    consume(TOKEN_IDENTIFIER, errorMessage);

    declareVariable();
    if (current->scopeDepth > 0) return 0;

    return identifierConstant(&parser.previous);
}

static void markInitialized(void) {
    if (current->scopeDepth == 0) return;
    current->locals[current->localCount - 1].depth = current->scopeDepth;
}

static void defineVariable(int global) {
    if (current->scopeDepth > 0) {
        markInitialized();
        return;
    }

    emitByte(OP_DEFINE_GLOBAL);
    emitShort(global);
}

/* ---- İfadeler ---- */

static uint8_t argumentList(void) {
    int argCount = 0;
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression();
            if (argCount == 255) {
                error("Bir çağrıda en çok 255 argüman olabilir.");
            }
            argCount++;
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Argümanlardan sonra ')' bekleniyor.");
    return (uint8_t)argCount;
}

/* Bileşik atama işleci (+=, -=, *=, /=) varsa tüketir ve karşılık gelen komutu döndürür. */
static bool matchCompoundAssign(uint8_t *op) {
    if (match(TOKEN_PLUS_EQUAL)) *op = OP_ADD;
    else if (match(TOKEN_MINUS_EQUAL)) *op = OP_SUBTRACT;
    else if (match(TOKEN_STAR_EQUAL)) *op = OP_MULTIPLY;
    else if (match(TOKEN_SLASH_EQUAL)) *op = OP_DIVIDE;
    else return false;
    return true;
}

static void and_(bool canAssign) {
    (void)canAssign;
    int endJump = emitJump(OP_AND);

    parsePrecedence(PREC_AND);
    emitByte(OP_ASSERT_BOOL);

    patchJump(endJump);
}

static void or_(bool canAssign) {
    (void)canAssign;
    int endJump = emitJump(OP_OR);

    parsePrecedence(PREC_OR);
    emitByte(OP_ASSERT_BOOL);

    patchJump(endJump);
}

static void binary(bool canAssign) {
    (void)canAssign;
    TokenKind operatorType = parser.previous.type;
    const ParseRule *rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_BANG_EQUAL: emitByte(OP_NOT_EQUAL); break;
        case TOKEN_EQUAL_EQUAL: emitByte(OP_EQUAL); break;
        case TOKEN_GREATER: emitByte(OP_GREATER); break;
        case TOKEN_GREATER_EQUAL: emitByte(OP_GREATER_EQUAL); break;
        case TOKEN_LESS: emitByte(OP_LESS); break;
        case TOKEN_LESS_EQUAL: emitByte(OP_LESS_EQUAL); break;
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        case TOKEN_PERCENT: emitByte(OP_MODULO); break;
        case TOKEN_IN: emitByte(OP_IN); break;
        default: return; /* ulaşılamaz */
    }
}

static void call(bool canAssign) {
    (void)canAssign;
    uint8_t argCount = argumentList();
    emitBytes(OP_CALL, argCount);
}

/* kap[dizin], kap[baş:son] ve dizine atama. */
static void subscript(bool canAssign) {
    bool isSlice = false;
    if (check(TOKEN_COLON)) {
        emitByte(OP_NIL);
    } else {
        expression();
    }
    if (match(TOKEN_COLON)) {
        isSlice = true;
        if (check(TOKEN_RIGHT_BRACKET)) {
            emitByte(OP_NIL);
        } else {
            expression();
        }
    }
    consume(TOKEN_RIGHT_BRACKET, "Dizinden sonra ']' bekleniyor.");

    if (isSlice) {
        emitByte(OP_SLICE);
        return;
    }

    uint8_t compoundOp;
    if (canAssign && match(TOKEN_EQUAL)) {
        expression();
        emitByte(OP_SET_INDEX);
        lastExpressionWasAssignment = true;
    } else if (canAssign && matchCompoundAssign(&compoundOp)) {
        emitByte(OP_DUP2);
        emitByte(OP_GET_INDEX);
        expression();
        emitByte(compoundOp);
        emitByte(OP_SET_INDEX);
        lastExpressionWasAssignment = true;
    } else {
        emitByte(OP_GET_INDEX);
    }
}

static void listLiteral(bool canAssign) {
    (void)canAssign;
    int count = 0;
    while (!check(TOKEN_RIGHT_BRACKET) && !check(TOKEN_EOF)) {
        expression();
        if (count == UINT16_MAX) error("Bir liste yazımında çok fazla öğe var.");
        count++;
        if (!match(TOKEN_COMMA)) break;
    }
    consume(TOKEN_RIGHT_BRACKET, "Liste öğelerinden sonra ']' bekleniyor.");
    emitByte(OP_BUILD_LIST);
    emitShort(count);
}

static void mapLiteral(bool canAssign) {
    (void)canAssign;
    int count = 0;
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
        expression();
        consume(TOKEN_COLON, "Sözlük anahtarından sonra ':' bekleniyor.");
        expression();
        if (count == UINT16_MAX) error("Bir sözlük yazımında çok fazla öğe var.");
        count++;
        if (!match(TOKEN_COMMA)) break;
    }
    consume(TOKEN_RIGHT_BRACE, "Sözlük öğelerinden sonra '}' bekleniyor.");
    emitByte(OP_BUILD_MAP);
    emitShort(count);
}

static void literal(bool canAssign) {
    (void)canAssign;
    switch (parser.previous.type) {
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        default: return; /* ulaşılamaz */
    }
}

static void grouping(bool canAssign) {
    (void)canAssign;
    expression();
    consume(TOKEN_RIGHT_PAREN, "İfadeden sonra ')' bekleniyor.");
}

static void number(bool canAssign) {
    (void)canAssign;
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

static void string(bool canAssign) {
    (void)canAssign;
    /* Tırnakları at, kaçış dizilerini çöz. */
    const char *source = parser.previous.start + 1;
    int sourceLength = parser.previous.length - 2;

    char *buffer = (char *)malloc((size_t)sourceLength + 1);
    if (buffer == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }

    int length = 0;
    for (int i = 0; i < sourceLength; i++) {
        char c = source[i];
        if (c != '\\') {
            buffer[length++] = c;
            continue;
        }

        i++;
        switch (source[i]) {
            case 'n': buffer[length++] = '\n'; break;
            case 't': buffer[length++] = '\t'; break;
            case 'r': buffer[length++] = '\r'; break;
            case '"': buffer[length++] = '"'; break;
            case '\\': buffer[length++] = '\\'; break;
            default:
                error("Geçersiz kaçış dizisi. Kullanılabilenler: \\n \\t \\r \\\" \\\\");
                break;
        }
    }

    emitConstant(OBJ_VAL(copyString(buffer, length)));
    free(buffer);
}

static void namedVariable(Token name, bool canAssign) {
    uint8_t getOp, setOp;
    bool isGlobal = false;
    int arg = resolveLocal(current, &name);
    if (arg != -1) {
        getOp = OP_GET_LOCAL;
        setOp = OP_SET_LOCAL;
    } else if ((arg = resolveUpvalue(current, &name)) != -1) {
        getOp = OP_GET_UPVALUE;
        setOp = OP_SET_UPVALUE;
    } else {
        arg = identifierConstant(&name);
        getOp = OP_GET_GLOBAL;
        setOp = OP_SET_GLOBAL;
        isGlobal = true;
    }

    uint8_t op = getOp;
    uint8_t compoundOp;
    if (canAssign && match(TOKEN_EQUAL)) {
        expression();
        op = setOp;
        lastExpressionWasAssignment = true;
    } else if (canAssign && matchCompoundAssign(&compoundOp)) {
        emitByte(getOp);
        if (isGlobal) emitShort(arg);
        else emitByte((uint8_t)arg);
        expression();
        emitByte(compoundOp);
        op = setOp;
        lastExpressionWasAssignment = true;
    }

    emitByte(op);
    if (isGlobal) {
        emitShort(arg);
    } else {
        emitByte((uint8_t)arg);
    }
}

static void variable(bool canAssign) {
    namedVariable(parser.previous, canAssign);
}

static void not_(bool canAssign) {
    (void)canAssign;
    parsePrecedence(PREC_NOT);
    emitByte(OP_NOT);
}

static void unary(bool canAssign) {
    (void)canAssign;
    parsePrecedence(PREC_UNARY);
    emitByte(OP_NEGATE);
}

static const ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]    = {grouping, call,   PREC_CALL},
    [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACKET]  = {listLiteral, subscript, PREC_CALL},
    [TOKEN_RIGHT_BRACKET] = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]    = {mapLiteral, NULL, PREC_NONE},
    [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_PLUS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STAR_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SLASH_EQUAL]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COLON]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
    [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
    [TOKEN_PERCENT]       = {NULL,     binary, PREC_FACTOR},
    [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]   = {NULL,     binary, PREC_EQUALITY},
    [TOKEN_BANG_EQUAL]    = {NULL,     binary, PREC_EQUALITY},
    [TOKEN_GREATER]       = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS]          = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL]    = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_IDENTIFIER]    = {variable, NULL,   PREC_NONE},
    [TOKEN_STRING]        = {string,   NULL,   PREC_NONE},
    [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
    [TOKEN_AND]           = {NULL,     and_,   PREC_AND},
    [TOKEN_BREAK]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CONTINUE]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FALSE]         = {literal,  NULL,   PREC_NONE},
    [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUNCTION]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IN]            = {NULL,     binary, PREC_COMPARISON},
    [TOKEN_NIL]           = {literal,  NULL,   PREC_NONE},
    [TOKEN_NOT]           = {not_,     NULL,   PREC_NONE},
    [TOKEN_OR]            = {NULL,     or_,    PREC_OR},
    [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_TRUE]          = {literal,  NULL,   PREC_NONE},
    [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NEWLINE]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_INDENT]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DEDENT]        = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("İfade bekleniyor.");
        return;
    }

    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign);

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }

    uint8_t ignored;
    if (canAssign && (match(TOKEN_EQUAL) || matchCompoundAssign(&ignored))) {
        error("Geçersiz atama hedefi; yalnızca değişkenlere ve dizinlere değer atanabilir.");
    }
}

static const ParseRule *getRule(TokenKind type) {
    return &rules[type];
}

static void expression(void) {
    parsePrecedence(PREC_ASSIGNMENT);
}

/* ---- Deyimler ---- */

static void endStatement(void) {
    consume(TOKEN_NEWLINE, "Deyimden sonra satır sonu bekleniyor.");
}

/* ':' satır sonu, girintili deyimler ve girintinin kapanması. */
static void block(void) {
    consume(TOKEN_COLON, "Blok başlatmak için ':' bekleniyor.");
    consume(TOKEN_NEWLINE, "':' işaretinden sonra satır sonu bekleniyor.");
    if (!match(TOKEN_INDENT)) {
        errorAtCurrent("':' işaretinden sonra girintili bir blok bekleniyor.");
        return;
    }

    while (!check(TOKEN_DEDENT) && !check(TOKEN_EOF)) {
        declaration();
    }

    consume(TOKEN_DEDENT, "Blok sonu bekleniyor.");
}

static void scopedBlock(void) {
    beginScope();
    block();
    endScope();
}

static void function(FunctionType type) {
    Compiler compiler;
    initCompiler(&compiler, type);
    beginScope();

    consume(TOKEN_LEFT_PAREN, "Fonksiyon adından sonra '(' bekleniyor.");
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            current->function->arity++;
            if (current->function->arity > 255) {
                errorAtCurrent("Bir fonksiyonun en çok 255 parametresi olabilir.");
            }
            int constant = parseVariable("Parametre adı bekleniyor.");
            defineVariable(constant);
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Parametrelerden sonra ')' bekleniyor.");
    block();

    ObjFunction *function = endCompiler();
    int constant = makeConstant(OBJ_VAL(function));
    emitByte(OP_CLOSURE);
    emitShort(constant);

    for (int i = 0; i < function->upvalueCount; i++) {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0);
        emitByte(compiler.upvalues[i].index);
    }
}

static void funDeclaration(void) {
    int global = parseVariable("Fonksiyon adı bekleniyor.");
    /* Fonksiyon kendi gövdesinde kendini çağırabilsin. */
    markInitialized();
    function(TYPE_FUNCTION);
    defineVariable(global);
}

static void varDeclaration(void) {
    int global = parseVariable("Değişken adı bekleniyor.");

    if (match(TOKEN_EQUAL)) {
        expression();
    } else {
        emitByte(OP_NIL);
    }
    endStatement();

    defineVariable(global);
}

static void expressionStatement(void) {
    lastExpressionWasAssignment = false;
    expression();
    endStatement();

    bool echo = replMode && current->type == TYPE_SCRIPT && current->scopeDepth == 0 &&
                !lastExpressionWasAssignment;
    emitByte(echo ? OP_ECHO : OP_POP);
}

/* 'eğer' belirteci tüketildikten sonra çağrılır. */
static void ifStatement(void) {
    expression();

    int thenJump = emitJump(OP_JUMP_IF_FALSE);
    scopedBlock();

    int elseJump = emitJump(OP_JUMP);
    patchJump(thenJump);

    if (match(TOKEN_ELSE)) {
        if (match(TOKEN_IF)) {
            ifStatement();
        } else {
            scopedBlock();
        }
    }
    patchJump(elseJump);
}

static void whileStatement(void) {
    Loop loop;
    loop.enclosing = current->loop;
    loop.start = currentChunk()->count;
    loop.scopeDepth = current->scopeDepth;
    loop.breakCount = 0;
    current->loop = &loop;

    expression();

    int exitJump = emitJump(OP_JUMP_IF_FALSE);
    scopedBlock();
    emitLoop(loop.start);

    patchJump(exitJump);
    for (int i = 0; i < loop.breakCount; i++) {
        patchJump(loop.breakJumps[i]);
    }

    current->loop = loop.enclosing;
}

/* Kullanıcının adıyla erişemeyeceği, derleyicinin kullandığı yerel değişken. */
static void addHiddenLocal(void) {
    Token name;
    name.type = TOKEN_IDENTIFIER;
    name.start = "";
    name.length = 0;
    name.line = parser.previous.line;
    name.message = NULL;
    addLocal(name);
    markInitialized();
}

/* her ad içinde ifade: blok */
static void forStatement(void) {
    beginScope();
    consume(TOKEN_IDENTIFIER, "'her' sözcüğünden sonra döngü değişkeninin adı bekleniyor.");
    Token name = parser.previous;
    consume(TOKEN_IN, "Döngü değişkeninden sonra 'içinde' bekleniyor.");

    /* Gezilen kap ve imleç, döngü boyunca iki gizli yerel değişkende tutulur. */
    expression();
    addHiddenLocal();
    emitConstant(NUMBER_VAL(0));
    addHiddenLocal();

    Loop loop;
    loop.enclosing = current->loop;
    loop.start = currentChunk()->count;
    loop.scopeDepth = current->scopeDepth;
    loop.breakCount = 0;
    current->loop = &loop;

    int exitJump = emitJump(OP_FOR_NEXT);

    beginScope();
    addLocal(name);
    markInitialized();
    block();
    endScope();
    emitLoop(loop.start);

    patchJump(exitJump);
    for (int i = 0; i < loop.breakCount; i++) {
        patchJump(loop.breakJumps[i]);
    }

    current->loop = loop.enclosing;
    endScope();
}

/* Döngüden çıkarken ya da başa dönerken döngü içindeki yerelleri yığından at. */
static void discardLoopLocals(void) {
    for (int i = current->localCount - 1;
         i >= 0 && current->locals[i].depth > current->loop->scopeDepth; i--) {
        emitByte(current->locals[i].isCaptured ? OP_CLOSE_UPVALUE : OP_POP);
    }
}

static void breakStatement(void) {
    if (current->loop == NULL) {
        error("'kır' yalnızca bir döngünün içinde kullanılabilir.");
        endStatement();
        return;
    }
    endStatement();

    discardLoopLocals();
    if (current->loop->breakCount == MAX_LOOP_BREAKS) {
        error("Bir döngüde çok fazla 'kır' deyimi var.");
        return;
    }
    current->loop->breakJumps[current->loop->breakCount++] = emitJump(OP_JUMP);
}

static void continueStatement(void) {
    if (current->loop == NULL) {
        error("'devam' yalnızca bir döngünün içinde kullanılabilir.");
        endStatement();
        return;
    }
    endStatement();

    discardLoopLocals();
    emitLoop(current->loop->start);
}

static void returnStatement(void) {
    if (current->type == TYPE_SCRIPT) {
        error("'dön' yalnızca bir fonksiyonun içinde kullanılabilir.");
    }

    if (match(TOKEN_NEWLINE)) {
        emitReturn();
    } else {
        expression();
        endStatement();
        emitByte(OP_RETURN);
    }
}

/* Hatadan sonra bir sonraki satırın başına kadar ilerle. */
static void synchronize(void) {
    parser.panicMode = false;

    while (parser.current.type != TOKEN_EOF) {
        if (parser.previous.type == TOKEN_NEWLINE) return;
        advance();
    }
}

static void declaration(void) {
    if (match(TOKEN_FUNCTION)) {
        funDeclaration();
    } else if (match(TOKEN_VAR)) {
        varDeclaration();
    } else {
        statement();
    }

    if (parser.panicMode) synchronize();
}

static void statement(void) {
    if (match(TOKEN_IF)) {
        ifStatement();
    } else if (match(TOKEN_WHILE)) {
        whileStatement();
    } else if (match(TOKEN_FOR)) {
        forStatement();
    } else if (match(TOKEN_RETURN)) {
        returnStatement();
    } else if (match(TOKEN_BREAK)) {
        breakStatement();
    } else if (match(TOKEN_CONTINUE)) {
        continueStatement();
    } else if (match(TOKEN_ELSE)) {
        error("'değilse' kendisinden önce bir 'eğer' bloğu olmadan kullanılamaz.");
    } else if (match(TOKEN_INDENT)) {
        error("Beklenmeyen girinti; bu satır bir bloğun içinde değil.");
        while (!check(TOKEN_DEDENT) && !check(TOKEN_EOF)) {
            declaration();
        }
        match(TOKEN_DEDENT);
    } else {
        expressionStatement();
    }
}

ObjFunction *compile(const char *name, const char *source, bool repl) {
    initScanner(source);
    Compiler compiler;
    sourceName = name;
    replMode = repl;
    lastExpressionWasAssignment = false;
    parser.hadError = false;
    parser.panicMode = false;
    initCompiler(&compiler, TYPE_SCRIPT);

    advance();

    while (!match(TOKEN_EOF)) {
        declaration();
    }

    ObjFunction *function = endCompiler();
    return parser.hadError ? NULL : function;
}

void markCompilerRoots(void) {
    Compiler *compiler = current;
    while (compiler != NULL) {
        markObject((Obj *)compiler->function);
        compiler = compiler->enclosing;
    }
}
