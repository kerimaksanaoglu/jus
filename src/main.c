#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

/* Yardımcı fonksiyonlar - common.h'da tanımlanan fonksiyonların implementasyonu */

/* String'i kopyala */
char *str_duplicate(const char *str) {
    if (!str) return NULL;
    
    int len = strlen(str);
    char *copy = malloc(len + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

/* Token'ı temizle */
void free_token(Token *token) {
    if (token && token->lexeme) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}

/* AST düğümünü temizle */
void free_ast_node(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_VAR_DECL:
            free(node->data.var_decl.name);
            free_ast_node(node->data.var_decl.value);
            break;
            
        case AST_FUNC_DECL:
            free(node->data.func_decl.name);
            for (int i = 0; i < node->data.func_decl.param_count; i++) {
                free(node->data.func_decl.params[i]);
            }
            free(node->data.func_decl.params);
            for (int i = 0; i < node->data.func_decl.body_count; i++) {
                free_ast_node(node->data.func_decl.body[i]);
            }
            free(node->data.func_decl.body);
            break;
            
        case AST_PRINT_STMT:
            free_ast_node(node->data.print_stmt.expression);
            break;
            
        case AST_IF_STMT:
            free_ast_node(node->data.if_stmt.condition);
            for (int i = 0; i < node->data.if_stmt.then_count; i++) {
                free_ast_node(node->data.if_stmt.then_branch[i]);
            }
            free(node->data.if_stmt.then_branch);
            for (int i = 0; i < node->data.if_stmt.else_count; i++) {
                free_ast_node(node->data.if_stmt.else_branch[i]);
            }
            free(node->data.if_stmt.else_branch);
            break;
            
        case AST_WHILE_STMT:
            free_ast_node(node->data.while_stmt.condition);
            for (int i = 0; i < node->data.while_stmt.body_count; i++) {
                free_ast_node(node->data.while_stmt.body[i]);
            }
            free(node->data.while_stmt.body);
            break;
            
        case AST_RETURN_STMT:
            free_ast_node(node->data.return_stmt.expression);
            break;
            
        case AST_BINARY_EXPR:
            free(node->data.binary_expr.operator);
            free_ast_node(node->data.binary_expr.left);
            free_ast_node(node->data.binary_expr.right);
            break;
            
        case AST_UNARY_EXPR:
            free(node->data.unary_expr.operator);
            free_ast_node(node->data.unary_expr.operand);
            break;
            
        case AST_LITERAL_EXPR:
            free(node->data.literal_expr.value);
            break;
            
        case AST_VARIABLE_EXPR:
            free(node->data.variable_expr.name);
            break;
            
        case AST_CALL_EXPR:
            free(node->data.call_expr.name);
            for (int i = 0; i < node->data.call_expr.arg_count; i++) {
                free_ast_node(node->data.call_expr.args[i]);
            }
            free(node->data.call_expr.args);
            break;
            
        case AST_ASSIGN_EXPR:
            free(node->data.assign_expr.name);
            free_ast_node(node->data.assign_expr.value);
            break;
            
        default:
            break;
    }
    
    free(node);
}

/* Environment'ı temizle */
void free_environment(SimpleEnvironment *env) {
    env_free(env);
}

/* Hata yazdır */
void print_error(const Error *error) {
    if (!error) return;
    
    const char *error_type;
    switch (error->type) {
        case ERROR_LEXER:
            error_type = "Lexer Hatası / Lexer Error";
            break;
        case ERROR_PARSER:
            error_type = "Parser Hatası / Parser Error";
            break;
        case ERROR_RUNTIME:
            error_type = "Çalışma Zamanı Hatası / Runtime Error";
            break;
        case ERROR_FILE_NOT_FOUND:
            error_type = "Dosya Bulunamadı / File Not Found";
            break;
        case ERROR_MEMORY:
            error_type = "Bellek Hatası / Memory Error";
            break;
        default:
            error_type = "Bilinmeyen Hata / Unknown Error";
            break;
    }
    
    if (error->line > 0) {
        fprintf(stderr, "%s [Satır/Line %d, Sütun/Column %d]: %s\n", 
                error_type, error->line, error->column, error->message);
    } else {
        fprintf(stderr, "%s: %s\n", error_type, error->message);
    }
}

/* Dosya okuma fonksiyonu */
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }
    
    // Dosya boyutunu öğren
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Bellek ayır ve dosyayı oku
    char *content = malloc(length + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    
    size_t read_length = fread(content, 1, length, file);
    content[read_length] = '\0';
    
    fclose(file);
    return content;
}

/* Kullanım bilgisini yazdır */
void print_usage(const char *program_name) {
    printf("Kullanım / Usage: %s <dosya.jus>\n", program_name);
    printf("Örnek / Example: %s merhaba.jus\n", program_name);
    printf("\n");
    printf("JUS Yorumlayıcısı - Türkçe programlama dili\n");
    printf("JUS Interpreter - Turkish programming language\n");
    printf("\n");
    printf("Desteklenen anahtar kelimeler / Supported keywords:\n");
    printf("  eğer       - if\n");
    printf("  değilse    - else\n");
    printf("  yaz        - print\n");
    printf("  girdi      - input\n");
    printf("  döngü      - loop\n");
    printf("  iken       - while\n");
    printf("  fonksiyon  - function\n");
    printf("  dönüş      - return\n");
    printf("  ve         - and\n");
    printf("  veya       - or\n");
    printf("  doğru      - true\n");
    printf("  yanlış     - false\n");
    printf("  boşluk     - null\n");
}

/* Ana program işlevi */
int main(int argc, char **argv) {
    /* Komut satırı argüman kontrolü */
    if (argc != 2) {
        fprintf(stderr, "Hata: Geçersiz argüman sayısı / Error: Invalid number of arguments\n\n");
        print_usage(argv[0]);
        return 1;
    }
    
    const char *filename = argv[1];
    
    /* Dosyayı oku */
    char *source = read_file(filename);
    if (!source) {
        Error error;
        error.type = ERROR_FILE_NOT_FOUND;
        error.message = malloc(256);
        snprintf(error.message, 256, "Dosya açılamadı: '%s' / Could not open file: '%s'", filename, filename);
        error.line = 0;
        error.column = 0;
        
        print_error(&error);
        free(error.message);
        return 1;
    }
    
    printf("JUS Yorumlayıcısı başlatılıyor... / Starting JUS Interpreter...\n");
    printf("Dosya okunuyor: %s / Reading file: %s\n", filename, filename);
    
    /* Lexical Analysis (Tokenization) */
    printf("\n1. Lexical Analysis (Sözcüksel Analiz)...\n");
    int token_count = 0;
    Lexer lexer;
    lexer_init(&lexer, source);
    Token *tokens = lexer_scan_all(&lexer, &token_count);
    
    if (!tokens) {
        Error error;
        error.type = ERROR_LEXER;
        error.message = str_duplicate("Tokenization başarısız / Tokenization failed");
        error.line = 0;
        error.column = 0;
        
        print_error(&error);
        free(error.message);
        free(source);
        return 1;
    }
    
    printf("   %d token bulundu / Found %d tokens\n", token_count, token_count);
    
    /* Syntax Analysis (Parsing) */
    printf("\n2. Syntax Analysis (Sözdizimi Analizi)...\n");
    int ast_count = 0;
    ASTNode **ast = parse(tokens, token_count, &ast_count);
    
    if (!ast) {
        Error error;
        error.type = ERROR_PARSER;
        error.message = str_duplicate("Parsing başarısız / Parsing failed");
        error.line = 0;
        error.column = 0;
        
        print_error(&error);
        free(error.message);
        
        // Token'ları temizle
        for (int i = 0; i < token_count; i++) {
            free_token(&tokens[i]);
        }
        free(tokens);
        free(source);
        return 1;
    }
    
    printf("   %d AST düğümü oluşturuldu / Created %d AST nodes\n", ast_count, ast_count);
    
    /* Semantic Analysis ve Interpretation */
    printf("\n3. Interpretation (Yorumlama)...\n");
    printf("=====================================\n");
    
    SimpleEnvironment global_env;
    env_init(&global_env);
    interpret(ast, ast_count, &global_env);
    
    printf("=====================================\n");
    printf("Program tamamlandı / Program completed\n");
    
    /* Bellek temizliği */
    printf("\nBellek temizliği / Memory cleanup...\n");
    
    // AST'yi temizle
    for (int i = 0; i < ast_count; i++) {
        free_ast_node(ast[i]);
    }
    free(ast);
    
    // Token'ları temizle
    token_free(tokens, token_count);
    
    // Environment'ı temizle
    env_free(&global_env);
    
    // Kaynak kodu temizle
    free(source);
    
    printf("JUS Yorumlayıcısı sonlandırıldı / JUS Interpreter terminated\n");
    return 0;
}