#include "parser.h"
#include "../include/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Parser state */
typedef struct {
    Token *tokens;
    int tokenCount;
    int current;
} Parser;

/* Forward declarations */
static ASTNode *parse_statement(Parser *parser);
static ASTNode *parse_expression(Parser *parser);
static ASTNode *parse_var_decl(Parser *parser);
static ASTNode *parse_print_stmt(Parser *parser);
static ASTNode *parse_if_stmt(Parser *parser);
static ASTNode *parse_loop_stmt(Parser *parser);
static ASTNode *parse_func_decl(Parser *parser);
static ASTNode *parse_func_call(Parser *parser);

/**
 * Mevcut token'ı al
 */
static Token *current_token(Parser *parser) {
    if (parser->current >= parser->tokenCount) {
        return NULL;
    }
    return &parser->tokens[parser->current];
}

/**
 * Sonraki token'a geç
 */
static void advance_token(Parser *parser) {
    if (parser->current < parser->tokenCount) {
        parser->current++;
    }
}

/**
 * Token türünü kontrol et
 */
static int check_token(Parser *parser, TokenType type) {
    Token *token = current_token(parser);
    if (!token) return 0;
    return token->type == type;
}

/**
 * Lexeme kontrolü
 */
static int check_lexeme(Parser *parser, const char *lexeme) {
    Token *token = current_token(parser);
    if (!token) return 0;
    return strcmp(token->lexeme, lexeme) == 0;
}

/**
 * Token tüket ve kontrol et
 */
static void consume_token(Parser *parser, TokenType type, const char *message) {
    if (!check_token(parser, type)) {
        Token *token = current_token(parser);
        fprintf(stderr, "Parser hata (satır %d): %s\n", 
                token ? token->line : 0, message);
        exit(EXIT_FAILURE);
    }
    advance_token(parser);
}

/**
 * AST düğümü oluştur
 */
static ASTNode *create_node(ASTNodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    memset(&node->data, 0, sizeof(node->data));
    return node;
}

/**
 * Newline'ları atla
 */
static void skip_newlines(Parser *parser) {
    while (check_token(parser, TOK_NEWLINE)) {
        advance_token(parser);
    }
}

/**
 * Statement parse et
 */
static ASTNode *parse_statement(Parser *parser) {
    skip_newlines(parser);
    
    Token *token = current_token(parser);
    if (!token) return NULL;
    
    // TODO: implement full statement parsing
    if (token->type == TOK_KEYWORD) {
        if (check_lexeme(parser, "değişken")) {
            return parse_var_decl(parser);
        } else if (check_lexeme(parser, "yaz")) {
            return parse_print_stmt(parser);
        } else if (check_lexeme(parser, "eğer")) {
            return parse_if_stmt(parser);
        } else if (check_lexeme(parser, "tekrar")) {
            return parse_loop_stmt(parser);
        } else if (check_lexeme(parser, "fonksiyon")) {
            return parse_func_decl(parser);
        } else if (check_lexeme(parser, "dönüş")) {
            // Return statement
            advance_token(parser); // "dönüş" kelimesini atla
            ASTNode *node = create_node(AST_RETURN_STMT);
            node->data.return_stmt.expression = parse_expression(parser);
            return node;
        }
    }
    
    if (token->type == TOK_IDENT) {
        // Check if it's a function call or an expression statement
        if (parser->current + 1 < parser->tokenCount &&
            parser->tokens[parser->current + 1].type == TOK_LPAREN) {
            return parse_func_call(parser);
        }
    }
    
    // Türkiye built-in fonksiyonları için özel kontrol
    if (token->type == TOK_KEYWORD) {
        if (strcmp(token->lexeme, "turkiye_cografya_bilgi") == 0 ||
            strcmp(token->lexeme, "turkiye_saati") == 0 ||
            strcmp(token->lexeme, "turkiye_sehir_mi") == 0 ||
            strcmp(token->lexeme, "turkiye_baskent_mi") == 0 ||
            strcmp(token->lexeme, "tc_kimlik_gecerli") == 0 ||
            strcmp(token->lexeme, "turkiye_telefon_gecerli") == 0 ||
            strcmp(token->lexeme, "tl_formatla") == 0 ||
            strcmp(token->lexeme, "turkiye_bayram_kontrol") == 0) {
            return parse_func_call(parser);
        }
    }
    
    return parse_expression(parser);
}

/**
 * Değişken bildirimi parse et
 */
static ASTNode *parse_var_decl(Parser *parser) {
    // TODO: implement variable declaration parsing
    advance_token(parser); // "değişken" kelimesini atla
    
    if (!check_token(parser, TOK_IDENT)) {
        fprintf(stderr, "Parser hata: Değişken adı bekleniyor\n");
        exit(EXIT_FAILURE);
    }
    
    ASTNode *node = create_node(AST_VAR_DECL);
    node->data.var_decl.name = strdup(current_token(parser)->lexeme);
    advance_token(parser);
    
    consume_token(parser, TOK_ASSIGN, "= bekleniyor");
    node->data.var_decl.value = parse_expression(parser);
    
    return node;
}

/**
 * Print statement parse et
 */
static ASTNode *parse_print_stmt(Parser *parser) {
    // TODO: implement print statement parsing
    advance_token(parser); // "yaz" kelimesini atla
    
    ASTNode *node = create_node(AST_PRINT_STMT);
    node->data.print_stmt.expression = parse_expression(parser);
    
    return node;
}

/**
 * If statement parse et
 */
static ASTNode *parse_if_stmt(Parser *parser) {
    advance_token(parser); // "eğer" kelimesini atla
    
    ASTNode *node = create_node(AST_IF_STMT);
    node->data.if_stmt.condition = parse_expression(parser);
    
    consume_token(parser, TOK_COLON, ": bekleniyor");
    skip_newlines(parser);
    
    // Then branch - birden fazla statement olabilir
    int then_capacity = 10;
    node->data.if_stmt.then_branch = malloc(then_capacity * sizeof(ASTNode*));
    node->data.if_stmt.then_count = 0;
    
    // Then branch statement'larını parse et
    while (current_token(parser) &&
           !check_token(parser, TOK_EOF) &&
           !(check_token(parser, TOK_KEYWORD) && check_lexeme(parser, "değilse"))) {
        
        skip_newlines(parser);
        if (check_token(parser, TOK_EOF) ||
            (check_token(parser, TOK_KEYWORD) && check_lexeme(parser, "değilse"))) {
            break;
        }
        
        ASTNode *stmt = parse_statement(parser);
        if (stmt) {
            if (node->data.if_stmt.then_count >= then_capacity) {
                then_capacity *= 2;
                node->data.if_stmt.then_branch = realloc(node->data.if_stmt.then_branch,
                                                        then_capacity * sizeof(ASTNode*));
            }
            node->data.if_stmt.then_branch[node->data.if_stmt.then_count++] = stmt;
        }
        skip_newlines(parser);
    }
    
    // Else branch kontrolü
    if (check_token(parser, TOK_KEYWORD) && check_lexeme(parser, "değilse")) {
        advance_token(parser); // "değilse" kelimesini atla
        consume_token(parser, TOK_COLON, ": bekleniyor");
        skip_newlines(parser);
        
        int else_capacity = 10;
        node->data.if_stmt.else_branch = malloc(else_capacity * sizeof(ASTNode*));
        node->data.if_stmt.else_count = 0;
        
        // Else branch statement'larını parse et
        while (current_token(parser) &&
               !check_token(parser, TOK_EOF)) {
            
            skip_newlines(parser);
            if (check_token(parser, TOK_EOF)) {
                break;
            }
            
            ASTNode *stmt = parse_statement(parser);
            if (stmt) {
                if (node->data.if_stmt.else_count >= else_capacity) {
                    else_capacity *= 2;
                    node->data.if_stmt.else_branch = realloc(node->data.if_stmt.else_branch,
                                                            else_capacity * sizeof(ASTNode*));
                }
                node->data.if_stmt.else_branch[node->data.if_stmt.else_count++] = stmt;
            }
            skip_newlines(parser);
            
            // Basit çözüm: sadece bir statement parse et
            break;
        }
    } else {
        node->data.if_stmt.else_branch = NULL;
        node->data.if_stmt.else_count = 0;
    }
    
    return node;
}

/**
 * Loop statement parse et
 */
static ASTNode *parse_loop_stmt(Parser *parser) {
    // TODO: implement loop statement parsing
    advance_token(parser); // "tekrar" kelimesini atla
    
    ASTNode *node = create_node(AST_LOOP_STMT);
    node->data.loop_stmt.count_expr = parse_expression(parser);

    // "kez" kelimesi isteğe bağlı
    if (check_lexeme(parser, "kez")) {
        advance_token(parser);
    }
    
    consume_token(parser, TOK_COLON, ": bekleniyor");
    
    // TODO: implement block parsing
    node->data.loop_stmt.body = malloc(sizeof(ASTNode*));
    node->data.loop_stmt.body_count = 1;
    node->data.loop_stmt.body[0] = parse_statement(parser);
    
    return node;
}

/**
 * Function declaration parse et
 */
static ASTNode *parse_func_decl(Parser *parser) {
    // TODO: implement function declaration parsing
    advance_token(parser); // "fonksiyon" kelimesini atla
    
    if (!check_token(parser, TOK_IDENT)) {
        fprintf(stderr, "Parser hata: Fonksiyon adı bekleniyor\n");
        exit(EXIT_FAILURE);
    }
    
    ASTNode *node = create_node(AST_FUNC_DECL);
    node->data.func_decl.name = strdup(current_token(parser)->lexeme);
    advance_token(parser);
    
    consume_token(parser, TOK_LPAREN, "( bekleniyor");
    
    // Parameter parsing
    int param_capacity = 10;
    node->data.func_decl.params = malloc(param_capacity * sizeof(char*));
    node->data.func_decl.param_count = 0;
    
    // Parametreleri parse et
    while (!check_token(parser, TOK_RPAREN) && !check_token(parser, TOK_EOF)) {
        if (!check_token(parser, TOK_IDENT)) {
            fprintf(stderr, "Parser hata: Parametre adı bekleniyor\n");
            exit(EXIT_FAILURE);
        }
        
        if (node->data.func_decl.param_count >= param_capacity) {
            param_capacity *= 2;
            node->data.func_decl.params = realloc(node->data.func_decl.params, param_capacity * sizeof(char*));
        }
        
        node->data.func_decl.params[node->data.func_decl.param_count] = strdup(current_token(parser)->lexeme);
        node->data.func_decl.param_count++;
        advance_token(parser);
        
        if (check_token(parser, TOK_COMMA)) {
            advance_token(parser); // ,
        } else if (!check_token(parser, TOK_RPAREN)) {
            fprintf(stderr, "Parser hata: , veya ) bekleniyor\n");
            exit(EXIT_FAILURE);
        }
    }
    
    consume_token(parser, TOK_RPAREN, ") bekleniyor");
    consume_token(parser, TOK_COLON, ": bekleniyor");
    skip_newlines(parser);
    
    // Function body parsing - multiple statements
    int body_capacity = 20;
    node->data.func_decl.body = malloc(body_capacity * sizeof(ASTNode*));
    node->data.func_decl.body_count = 0;
    
    // Parse function body statements
    while (current_token(parser) && !check_token(parser, TOK_EOF)) {
        skip_newlines(parser);
        if (check_token(parser, TOK_EOF)) {
            break;
        }
        
        ASTNode *stmt = parse_statement(parser);
        if (stmt) {
            if (node->data.func_decl.body_count >= body_capacity) {
                body_capacity *= 2;
                node->data.func_decl.body = realloc(node->data.func_decl.body,
                                                   body_capacity * sizeof(ASTNode*));
            }
            node->data.func_decl.body[node->data.func_decl.body_count++] = stmt;
        }
        
        skip_newlines(parser);
        
        // Break after processing one statement for now - simple implementation
        break;
    }
    
    return node;
}

/**
 * Function call parse et
 */
static ASTNode *parse_func_call(Parser *parser) {
    Token *token = current_token(parser);
    if (!token || (token->type != TOK_IDENT && token->type != TOK_KEYWORD)) {
        fprintf(stderr, "Parser hata: Fonksiyon adı bekleniyor\n");
        exit(EXIT_FAILURE);
    }
    
    ASTNode *node = create_node(AST_CALL_EXPR);
    node->data.call_expr.name = strdup(token->lexeme);
    advance_token(parser);
    
    if (check_token(parser, TOK_LPAREN)) {
        advance_token(parser); // (
        
        // Argument parsing
        int arg_capacity = 10;
        node->data.call_expr.args = malloc(arg_capacity * sizeof(ASTNode*));
        node->data.call_expr.arg_count = 0;
        
        // Argümanları parse et
        while (!check_token(parser, TOK_RPAREN) && !check_token(parser, TOK_EOF)) {
            if (node->data.call_expr.arg_count >= arg_capacity) {
                arg_capacity *= 2;
                node->data.call_expr.args = realloc(node->data.call_expr.args,
                                                   arg_capacity * sizeof(ASTNode*));
            }
            
            node->data.call_expr.args[node->data.call_expr.arg_count] = parse_expression(parser);
            node->data.call_expr.arg_count++;
            
            if (check_token(parser, TOK_COMMA)) {
                advance_token(parser); // ,
            } else if (!check_token(parser, TOK_RPAREN)) {
                fprintf(stderr, "Parser hata: , veya ) bekleniyor\n");
                exit(EXIT_FAILURE);
            }
        }
        
        consume_token(parser, TOK_RPAREN, ") bekleniyor");
    } else {
        // Parantez yok, sadece identifier - function call değil
        free(node);
        node = create_node(AST_VARIABLE_EXPR);
        node->data.variable_expr.name = strdup(token->lexeme);
    }
    
    return node;
}

static ASTNode *parse_primary(Parser *parser) {
    Token *token = current_token(parser);
    if (!token) {
        fprintf(stderr, "Parser hata: İfade bekleniyor\n");
        exit(EXIT_FAILURE);
    }

    ASTNode *node = NULL;
    
    // Unary operatörler
    if (token->type == TOK_NOT || token->type == TOK_MINUS) {
        node = create_node(AST_UNARY_EXPR);
        node->data.unary_expr.operator = strdup(token->lexeme);
        advance_token(parser);
        node->data.unary_expr.operand = parse_primary(parser);
    }
    // Parantez içi ifadeler
    else if (token->type == TOK_LPAREN) {
        advance_token(parser); // '(' atla
        node = parse_expression(parser);
        consume_token(parser, TOK_RPAREN, ") bekleniyor");
    }
    // Literal değerler
    else if (token->type == TOK_NUMBER || token->type == TOK_STRING) {
        node = create_node(AST_LITERAL_EXPR);
        node->data.literal_expr.value = strdup(token->lexeme);
        advance_token(parser);
    }
    // Boolean keyword'ler
    else if (token->type == TOK_KEYWORD &&
             (strcmp(token->lexeme, "doğru") == 0 || strcmp(token->lexeme, "yanlış") == 0)) {
        node = create_node(AST_LITERAL_EXPR);
        node->data.literal_expr.value = strdup(token->lexeme);
        advance_token(parser);
    }
    // Identifier veya Türkiye fonksiyonları
    else if (token->type == TOK_IDENT) {
        // Check if it's a function call
        if (parser->current + 1 < parser->tokenCount &&
            parser->tokens[parser->current + 1].type == TOK_LPAREN) {
            // Function call
            node = create_node(AST_CALL_EXPR);
            node->data.call_expr.name = strdup(token->lexeme);
            advance_token(parser);
            
            advance_token(parser); // (
            
            // Argument parsing
            int arg_capacity = 10;
            node->data.call_expr.args = malloc(arg_capacity * sizeof(ASTNode*));
            node->data.call_expr.arg_count = 0;
            
            // Parse arguments
            while (!check_token(parser, TOK_RPAREN) && !check_token(parser, TOK_EOF)) {
                if (node->data.call_expr.arg_count >= arg_capacity) {
                    arg_capacity *= 2;
                    node->data.call_expr.args = realloc(node->data.call_expr.args,
                                                       arg_capacity * sizeof(ASTNode*));
                }
                
                node->data.call_expr.args[node->data.call_expr.arg_count] = parse_expression(parser);
                node->data.call_expr.arg_count++;
                
                if (check_token(parser, TOK_COMMA)) {
                    advance_token(parser); // ,
                } else if (!check_token(parser, TOK_RPAREN)) {
                    fprintf(stderr, "Parser hata: , veya ) bekleniyor\n");
                    exit(EXIT_FAILURE);
                }
            }
            
            consume_token(parser, TOK_RPAREN, ") bekleniyor");
        } else {
            // Simple variable
            node = create_node(AST_VARIABLE_EXPR);
            node->data.variable_expr.name = strdup(token->lexeme);
            advance_token(parser);
        }
    }
    // Türkiye built-in fonksiyonları keyword olarak gelirse
    else if (token->type == TOK_KEYWORD) {
        if (strcmp(token->lexeme, "turkiye_cografya_bilgi") == 0 ||
            strcmp(token->lexeme, "turkiye_saati") == 0 ||
            strcmp(token->lexeme, "turkiye_sehir_mi") == 0 ||
            strcmp(token->lexeme, "turkiye_baskent_mi") == 0 ||
            strcmp(token->lexeme, "tc_kimlik_gecerli") == 0 ||
            strcmp(token->lexeme, "turkiye_telefon_gecerli") == 0 ||
            strcmp(token->lexeme, "tl_formatla") == 0 ||
            strcmp(token->lexeme, "turkiye_bayram_kontrol") == 0) {
            // Function call olarak parse et
            return parse_func_call(parser);
        } else {
            fprintf(stderr, "Parser hata: Beklenmeyen keyword '%s'\n", token->lexeme);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Parser hata: Beklenmeyen token '%s'\n", token->lexeme);
        exit(EXIT_FAILURE);
    }
    return node;
}

static ASTNode *parse_binary_expression(Parser *parser, int precedence) {
    ASTNode *left = parse_primary(parser);

    while (1) {
        Token *op = current_token(parser);
        if (op == NULL || (op->type != TOK_PLUS && op->type != TOK_MINUS &&
                          op->type != TOK_MULTIPLY && op->type != TOK_DIVIDE &&
                          op->type != TOK_MODULO && op->type != TOK_EQ &&
                          op->type != TOK_NE && op->type != TOK_LT &&
                          op->type != TOK_LE && op->type != TOK_GT &&
                          op->type != TOK_GE && op->type != TOK_ASSIGN &&
                          !(op->type == TOK_KEYWORD &&
                            (strcmp(op->lexeme, "ve") == 0 || strcmp(op->lexeme, "veya") == 0)))) {
            break;
        }

        // TODO: Implement proper precedence climbing
        (void)precedence; // Unused for now

        advance_token(parser);
        ASTNode *right = parse_primary(parser);
        
        // Assignment operatörü özel işlem
        if (strcmp(op->lexeme, "=") == 0) {
            ASTNode *new_node = create_node(AST_ASSIGN_EXPR);
            if (left->type == AST_VARIABLE_EXPR) {
                new_node->data.assign_expr.name = strdup(left->data.variable_expr.name);
                new_node->data.assign_expr.value = right;
                free(left); // Sol tarafı temizle
                left = new_node;
            } else {
                fprintf(stderr, "Parser hata: Sadece değişkenlere değer atanabilir\n");
                exit(EXIT_FAILURE);
            }
        } else {
            ASTNode *new_node = create_node(AST_BINARY_EXPR);
            new_node->data.binary_expr.left = left;
            new_node->data.binary_expr.right = right;
            new_node->data.binary_expr.operator = strdup(op->lexeme);
            left = new_node;
        }
    }

    return left;
}

/**
 * Expression parse et
 */
static ASTNode *parse_expression(Parser *parser) {
    return parse_binary_expression(parser, 0);
}

/**
 * Token dizisini AST düğümlerine dönüştür
 */
ASTNode **parse(Token *tokens, int tokenCount, int *outAstCount) {
    if (!tokens || tokenCount == 0 || !outAstCount) {
        *outAstCount = 0;
        return NULL;
    }
    
    Parser parser;
    parser.tokens = tokens;
    parser.tokenCount = tokenCount;
    parser.current = 0;
    
    ASTNode **nodes = malloc(100 * sizeof(ASTNode*));
    int count = 0;
    
    // <program> ::= <statement>*
    while (parser.current < parser.tokenCount && 
           !check_token(&parser, TOK_EOF)) {
        
        skip_newlines(&parser);
        
        if (check_token(&parser, TOK_EOF)) {
            break;
        }
        
        ASTNode *stmt = parse_statement(&parser);
        if (stmt) {
            nodes[count++] = stmt;
        }
        
        skip_newlines(&parser);
    }
    
    *outAstCount = count;
    return nodes;
}

/**
 * AST düğüm dizisini bellek temizliği için serbest bırak
 */
void ast_free(ASTNode **nodes, int count) {
    if (!nodes) return;
    
    for (int i = 0; i < count; i++) {
        if (nodes[i]) {
            // TODO: implement recursive AST node freeing
            free(nodes[i]);
        }
    }
    
    free(nodes);
}