#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Türkçe anahtar kelime tanımları */
// Temel Kontrol
#define EGER_KEYWORD     "eğer"
#define DEGILSE_KEYWORD  "değilse"
#define YAZ_KEYWORD      "yaz"
#define GIRDI_KEYWORD    "girdi"
#define DONGU_KEYWORD    "döngü"
#define IKEN_KEYWORD     "iken"
#define FONKSIYON_KEYWORD "fonksiyon"
#define DONUS_KEYWORD    "dönüş"
#define VE_KEYWORD       "ve"
#define VEYA_KEYWORD     "veya"
#define DOGRU_KEYWORD    "doğru"
#define YANLIS_KEYWORD   "yanlış"
#define BOSLUK_KEYWORD   "boşluk"

/* Yeni Türkçe Anahtar Kelimeler - TürkKod 2053 Vizyonu */
// Kontrol Yapıları
#define BASLAT_KEYWORD    "başlat"
#define BITIR_KEYWORD     "bitir"
#define DEVAM_KEYWORD     "devam"
#define KIR_KEYWORD       "kır"
#define DENE_KEYWORD      "dene"
#define YAKALA_KEYWORD    "yakala"
#define FIRLAT_KEYWORD    "fırlat"
#define SEC_KEYWORD       "seç"
#define DURUM_KEYWORD     "durum"
#define VARSAYILAN_KEYWORD "varsayılan"
#define ATLA_KEYWORD      "atla"
#define GIT_KEYWORD       "git"

// Veri Yapıları
#define DIZI_KEYWORD      "dizi"
#define LISTE_KEYWORD     "liste"
#define SOZLUK_KEYWORD    "sözlük"
#define KUME_KEYWORD      "küme"
#define YIGIN_KEYWORD     "yığın"
#define SIRA_KEYWORD      "sıra"
#define NESNE_KEYWORD     "nesne"
#define SINIF_KEYWORD     "sınıf"
#define MIRAS_KEYWORD     "miras"
#define ARABIRIM_KEYWORD  "arabirim"
#define OZELLIK_KEYWORD   "özellik"
#define YONTEM_KEYWORD    "yöntem"

// Döngü Yapıları
#define HER_KEYWORD       "her"
#define KADAR_KEYWORD     "kadar"
#define ICIN_KEYWORD      "için"
#define TEKRARLA_KEYWORD  "tekrarla"
#define ILERI_KEYWORD     "ileri"
#define GERI_KEYWORD      "geri"
#define DURDU_KEYWORD     "durdu"
#define DEVAM_ET_KEYWORD  "devam_et"

// Modüler Programlama
#define ICERI_AL_KEYWORD  "içeri_al"
#define DISARI_VER_KEYWORD "dışarı_ver"
#define MODUL_KEYWORD     "modül"
#define PAKET_KEYWORD     "paket"
#define KUTUPHANE_KEYWORD "kütüphane"
#define ACIK_KEYWORD      "açık"
#define KAPALI_KEYWORD    "kapalı"
#define KORUMALI_KEYWORD  "korumalı"
#define OZEL_KEYWORD      "özel"
#define GENEL_KEYWORD     "genel"

// Mantık İfadeleri
#define BU_KEYWORD        "bu"
#define O_KEYWORD         "o"
#define SU_KEYWORD        "şu"
#define BOYLE_KEYWORD     "böyle"
#define SOYLE_KEYWORD     "şöyle"
#define OYLE_KEYWORD      "öyle"
#define HANGI_KEYWORD     "hangi"
#define NEREDE_KEYWORD    "nerede"
#define NASIL_KEYWORD     "nasıl"
#define NE_ZAMAN_KEYWORD  "ne_zaman"
#define NEDEN_KEYWORD     "neden"

/* Token türleri */
typedef enum {
    TOK_EOF,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_STRING,
    TOK_KEYWORD,
    TOK_COLON,
    TOK_NEWLINE,
    TOK_COMMA,
    TOK_SEMICOLON,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LBRACKET,
    TOK_RBRACKET,
    TOK_ASSIGN,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,
    TOK_MODULO,
    TOK_EQ,
    TOK_NE,
    TOK_LT,
    TOK_LE,
    TOK_GT,
    TOK_GE,
    TOK_AND,
    TOK_OR,
    TOK_NOT,
    TOK_INVALID
} TokenType;

/* Token yapısı */
typedef struct {
    TokenType type;
    char *lexeme;
    int line;
} Token;

/* AST düğüm türleri */
typedef enum {
    AST_PROGRAM,
    AST_VAR_DECL,
    AST_FUNC_DECL,
    AST_PRINT_STMT,
    AST_IF_STMT,
    AST_LOOP_STMT,
    AST_WHILE_STMT,
    AST_RETURN_STMT,
    AST_EXPRESSION_STMT,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_LITERAL_EXPR,
    AST_VARIABLE_EXPR,
    AST_CALL_EXPR,
    AST_ASSIGN_EXPR
} ASTNodeType;

/* AST düğüm yapısı */
typedef struct ASTNode {
    ASTNodeType type;
    union {
        struct {
            char *name;
            struct ASTNode *value;
        } var_decl;
        
        struct {
            char *name;
            char **params;
            int param_count;
            struct ASTNode **body;
            int body_count;
        } func_decl;
        
        struct {
            struct ASTNode *expression;
        } print_stmt;
        
        struct {
            struct ASTNode *condition;
            struct ASTNode **then_branch;
            int then_count;
            struct ASTNode **else_branch;
            int else_count;
        } if_stmt;

        struct {
            struct ASTNode *count_expr;
            struct ASTNode **body;
            int body_count;
        } loop_stmt;
        
        struct {
            struct ASTNode *condition;
            struct ASTNode **body;
            int body_count;
        } while_stmt;
        
        struct {
            struct ASTNode *expression;
        } return_stmt;
        
        struct {
            char *operator;
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_expr;
        
        struct {
            char *operator;
            struct ASTNode *operand;
        } unary_expr;
        
        struct {
            char *value;
        } literal_expr;
        
        struct {
            char *name;
        } variable_expr;
        
        struct {
            char *name;
            struct ASTNode **args;
            int arg_count;
        } call_expr;
        
        struct {
            char *name;
            struct ASTNode *value;
        } assign_expr;
    } data;
} ASTNode;

/* Değişken tablosu için değer türleri */
typedef enum {
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_NULL
} ValueType;

/* Değer yapısı */
typedef struct {
    ValueType type;
    union {
        double number;
        char *string;
        int boolean;
    } data;
} Value;

/* Değişken tablosu girişi */
typedef struct VarEntry {
    char *name;
    Value value;
    struct VarEntry *next;
} VarEntry;

/* Çalışma ortamı yapısı */
typedef struct Environment {
    VarEntry *variables;
    struct Environment *parent;
} Environment;

/* Hata türleri */
typedef enum {
    ERROR_NONE,
    ERROR_LEXER,
    ERROR_PARSER,
    ERROR_RUNTIME,
    ERROR_FILE_NOT_FOUND,
    ERROR_MEMORY
} ErrorType;

/* Hata yapısı */
typedef struct {
    ErrorType type;
    char *message;
    int line;
    int column;
} Error;

/* Genel yardımcı fonksiyon prototipleri */
char *str_duplicate(const char *str);
void free_token(Token *token);
void free_ast_node(ASTNode *node);
void print_error(const Error *error);

#endif /* COMMON_H */