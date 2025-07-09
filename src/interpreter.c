#include "interpreter.h"
#include "../include/common.h"
#include "turkiye_features.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    ValueType type;
    union {
        double number;
        char *string;
        int boolean;
    } as;
} RuntimeValue;

/* Forward declarations */
static void visit_node(ASTNode *node, SimpleEnvironment *env);
static void visit_var_decl(ASTNode *node, SimpleEnvironment *env);
static void visit_print_stmt(ASTNode *node, SimpleEnvironment *env);
static void visit_if_stmt(ASTNode *node, SimpleEnvironment *env);
static void visit_loop_stmt(ASTNode *node, SimpleEnvironment *env);
static void visit_func_decl(ASTNode *node, SimpleEnvironment *env);
static void visit_func_call(ASTNode *node, SimpleEnvironment *env);
static void visit_expr_stmt(ASTNode *node, SimpleEnvironment *env);
static void visit_binary_expr(ASTNode *node, SimpleEnvironment *env);

static RuntimeValue evaluate(ASTNode *node, SimpleEnvironment *env);
static char *value_to_string(RuntimeValue value);
static RuntimeValue env_get(SimpleEnvironment *env, const char *name);
static void env_define(SimpleEnvironment *env, const char *name, RuntimeValue value);

/* Visitor function pointer table */
typedef void (*VisitorFunc)(ASTNode *node, SimpleEnvironment *env);
static VisitorFunc visit_table[] = {
    [AST_PROGRAM] = NULL, // Special handling
    [AST_VAR_DECL] = visit_var_decl,
    [AST_FUNC_DECL] = visit_func_decl,
    [AST_PRINT_STMT] = visit_print_stmt,
    [AST_IF_STMT] = visit_if_stmt,
    [AST_LOOP_STMT] = visit_loop_stmt,
    [AST_WHILE_STMT] = NULL, // TODO
    [AST_RETURN_STMT] = NULL, // TODO
    [AST_EXPRESSION_STMT] = visit_expr_stmt,
    [AST_BINARY_EXPR] = visit_binary_expr, // Handle binary expressions
    [AST_UNARY_EXPR] = NULL, // Evaluated by evaluate()
    [AST_LITERAL_EXPR] = NULL, // Evaluated by evaluate()
    [AST_VARIABLE_EXPR] = NULL, // Evaluated by evaluate()
    [AST_CALL_EXPR] = visit_func_call,
    [AST_ASSIGN_EXPR] = visit_expr_stmt // Handle assignment expressions
};

/**
 * Environment'ı başlat
 */
void env_init(SimpleEnvironment *env) {
    env->names = malloc(100 * sizeof(char*));
    env->values = malloc(100 * sizeof(Value));
    env->count = 0;
    env->capacity = 100;
}

/**
 * Environment'ı temizle
 */
void env_free(SimpleEnvironment *env) {
    if (!env) return;
    
    for (int i = 0; i < env->count; i++) {
        if (env->names[i]) {
            free(env->names[i]);
        }
        if (env->values[i].type == VAL_STRING && env->values[i].data.string) {
            free(env->values[i].data.string);
        }
    }
    
    if (env->names) free(env->names);
    if (env->values) free(env->values);
    
    env->names = NULL;
    env->values = NULL;
    env->count = 0;
    env->capacity = 0;
}

/**
 * Environment'a değişken tanımla
 */
static void env_define(SimpleEnvironment *env, const char *name, RuntimeValue value) {
    // Mevcut değişken kontrolü
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->names[i], name) == 0) {
            // Değişken zaten var, güncelle
            if (env->values[i].type == VAL_STRING && env->values[i].data.string) {
                free(env->values[i].data.string);
            }
            
            Value val;
            val.type = value.type;
            if (value.type == VAL_NUMBER) {
                val.data.number = value.as.number;
            } else if (value.type == VAL_STRING) {
                val.data.string = strdup(value.as.string);
            } else if (value.type == VAL_BOOLEAN) {
                val.data.boolean = value.as.boolean;
            }
            env->values[i] = val;
            return;
        }
    }
    
    // Kapasite kontrolü
    if (env->count >= env->capacity) {
        env->capacity *= 2;
        env->names = realloc(env->names, env->capacity * sizeof(char*));
        env->values = realloc(env->values, env->capacity * sizeof(Value));
    }
    
    // Yeni değişken ekle
    env->names[env->count] = strdup(name);
    
    Value val;
    val.type = value.type;
    if (value.type == VAL_NUMBER) {
        val.data.number = value.as.number;
    } else if (value.type == VAL_STRING) {
        val.data.string = strdup(value.as.string);
    } else if (value.type == VAL_BOOLEAN) {
        val.data.boolean = value.as.boolean;
    }
    env->values[env->count] = val;
    env->count++;
}

/**
 * Environment'dan değişken al
 */
static RuntimeValue env_get(SimpleEnvironment *env, const char *name) {
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->names[i], name) == 0) {
            RuntimeValue val;
            val.type = env->values[i].type;
            if (val.type == VAL_NUMBER) {
                val.as.number = env->values[i].data.number;
            } else if (val.type == VAL_STRING) {
                val.as.string = env->values[i].data.string;
            } else if (val.type == VAL_BOOLEAN) {
                val.as.boolean = env->values[i].data.boolean;
            }
            return val;
        }
    }
    
    fprintf(stderr, "Runtime hata: Tanımsız değişken '%s'\n", name);
    exit(EXIT_FAILURE);
}

/**
 * Value'yu string'e dönüştür
 */
static char *value_to_string(RuntimeValue value) {
    char *buffer = malloc(256);
    
    if (value.type == VAL_NUMBER) {
        snprintf(buffer, 256, "%g", value.as.number);
    } else if (value.type == VAL_STRING) {
        snprintf(buffer, 256, "%s", value.as.string);
    } else if (value.type == VAL_BOOLEAN) {
        snprintf(buffer, 256, "%s", value.as.boolean ? "doğru" : "yanlış");
    } else {
        snprintf(buffer, 256, "boşluk");
    }
    return buffer;
}

/**
 * Expression'ı değerlendir
 */
static RuntimeValue evaluate(ASTNode *node, SimpleEnvironment *env) {
    RuntimeValue result;
    result.type = VAL_NULL;

    if (!node) {
        return result;
    }

    switch (node->type) {
        case AST_LITERAL_EXPR: {
            // Türkçe boolean literal'ları kontrol et
            if (strcmp(node->data.literal_expr.value, "doğru") == 0) {
                result.type = VAL_BOOLEAN;
                result.as.boolean = 1;
            } else if (strcmp(node->data.literal_expr.value, "yanlış") == 0) {
                result.type = VAL_BOOLEAN;
                result.as.boolean = 0;
            } else {
                // Sayı mı kontrol et
                char *endptr;
                double num = strtod(node->data.literal_expr.value, &endptr);
                if (*endptr == '\0') {
                    result.type = VAL_NUMBER;
                    result.as.number = num;
                } else {
                    result.type = VAL_STRING;
                    result.as.string = node->data.literal_expr.value;
                }
            }
            break;
        }
        case AST_VARIABLE_EXPR: {
            result = env_get(env, node->data.variable_expr.name);
            break;
        }
        case AST_BINARY_EXPR: {
            RuntimeValue left = evaluate(node->data.binary_expr.left, env);
            RuntimeValue right = evaluate(node->data.binary_expr.right, env);
            
            // Arithmetic operations
            if (strcmp(node->data.binary_expr.operator, "+") == 0) {
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.type = VAL_NUMBER;
                    result.as.number = left.as.number + right.as.number;
                } else {
                    // String concatenation
                    result.type = VAL_STRING;
                    char *left_str = value_to_string(left);
                    char *right_str = value_to_string(right);
                    
                    char *concat_buffer = malloc(strlen(left_str) + strlen(right_str) + 1);
                    strcpy(concat_buffer, left_str);
                    strcat(concat_buffer, right_str);
                    
                    result.as.string = concat_buffer;
                    
                    free(left_str);
                    free(right_str);
                }
            } else if (strcmp(node->data.binary_expr.operator, "-") == 0) {
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.type = VAL_NUMBER;
                    result.as.number = left.as.number - right.as.number;
                } else {
                    fprintf(stderr, "Runtime hata: Çıkarma işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(node->data.binary_expr.operator, "*") == 0) {
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.type = VAL_NUMBER;
                    result.as.number = left.as.number * right.as.number;
                } else {
                    fprintf(stderr, "Runtime hata: Çarpma işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(node->data.binary_expr.operator, "/") == 0) {
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    if (right.as.number == 0) {
                        fprintf(stderr, "Runtime hata: Sıfıra bölme hatası\n");
                        exit(EXIT_FAILURE);
                    }
                    result.type = VAL_NUMBER;
                    result.as.number = left.as.number / right.as.number;
                } else {
                    fprintf(stderr, "Runtime hata: Bölme işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(node->data.binary_expr.operator, "%") == 0) {
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.type = VAL_NUMBER;
                    result.as.number = fmod(left.as.number, right.as.number);
                } else {
                    fprintf(stderr, "Runtime hata: Mod işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            }
            // Comparison operations
            else if (strcmp(node->data.binary_expr.operator, "==") == 0) {
                result.type = VAL_BOOLEAN;
                if (left.type == right.type) {
                    if (left.type == VAL_NUMBER) {
                        result.as.boolean = (left.as.number == right.as.number);
                    } else if (left.type == VAL_STRING) {
                        result.as.boolean = (strcmp(left.as.string, right.as.string) == 0);
                    } else if (left.type == VAL_BOOLEAN) {
                        result.as.boolean = (left.as.boolean == right.as.boolean);
                    }
                } else {
                    result.as.boolean = 0;
                }
            } else if (strcmp(node->data.binary_expr.operator, "!=") == 0) {
                result.type = VAL_BOOLEAN;
                if (left.type == right.type) {
                    if (left.type == VAL_NUMBER) {
                        result.as.boolean = (left.as.number != right.as.number);
                    } else if (left.type == VAL_STRING) {
                        result.as.boolean = (strcmp(left.as.string, right.as.string) != 0);
                    } else if (left.type == VAL_BOOLEAN) {
                        result.as.boolean = (left.as.boolean != right.as.boolean);
                    }
                } else {
                    result.as.boolean = 1;
                }
            } else if (strcmp(node->data.binary_expr.operator, "<") == 0) {
                result.type = VAL_BOOLEAN;
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.as.boolean = (left.as.number < right.as.number);
                } else {
                    fprintf(stderr, "Runtime hata: Karşılaştırma işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(node->data.binary_expr.operator, "<=") == 0) {
                result.type = VAL_BOOLEAN;
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.as.boolean = (left.as.number <= right.as.number);
                } else {
                    fprintf(stderr, "Runtime hata: Karşılaştırma işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(node->data.binary_expr.operator, ">") == 0) {
                result.type = VAL_BOOLEAN;
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.as.boolean = (left.as.number > right.as.number);
                } else {
                    fprintf(stderr, "Runtime hata: Karşılaştırma işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(node->data.binary_expr.operator, ">=") == 0) {
                result.type = VAL_BOOLEAN;
                if (left.type == VAL_NUMBER && right.type == VAL_NUMBER) {
                    result.as.boolean = (left.as.number >= right.as.number);
                } else {
                    fprintf(stderr, "Runtime hata: Karşılaştırma işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            }
            // Logical operations (Türkçe)
            else if (strcmp(node->data.binary_expr.operator, "ve") == 0) {
                result.type = VAL_BOOLEAN;
                int left_bool = (left.type == VAL_NUMBER && left.as.number != 0) ||
                              (left.type == VAL_STRING && strlen(left.as.string) > 0) ||
                              (left.type == VAL_BOOLEAN && left.as.boolean);
                int right_bool = (right.type == VAL_NUMBER && right.as.number != 0) ||
                               (right.type == VAL_STRING && strlen(right.as.string) > 0) ||
                               (right.type == VAL_BOOLEAN && right.as.boolean);
                result.as.boolean = left_bool && right_bool;
            } else if (strcmp(node->data.binary_expr.operator, "veya") == 0) {
                result.type = VAL_BOOLEAN;
                int left_bool = (left.type == VAL_NUMBER && left.as.number != 0) ||
                              (left.type == VAL_STRING && strlen(left.as.string) > 0) ||
                              (left.type == VAL_BOOLEAN && left.as.boolean);
                int right_bool = (right.type == VAL_NUMBER && right.as.number != 0) ||
                               (right.type == VAL_STRING && strlen(right.as.string) > 0) ||
                               (right.type == VAL_BOOLEAN && right.as.boolean);
                result.as.boolean = left_bool || right_bool;
            }
            break;
        }
        case AST_UNARY_EXPR: {
            RuntimeValue operand = evaluate(node->data.unary_expr.operand, env);
            if (strcmp(node->data.unary_expr.operator, "!") == 0) {
                result.type = VAL_BOOLEAN;
                int operand_bool = (operand.type == VAL_NUMBER && operand.as.number != 0) ||
                                  (operand.type == VAL_STRING && strlen(operand.as.string) > 0) ||
                                  (operand.type == VAL_BOOLEAN && operand.as.boolean);
                result.as.boolean = !operand_bool;
            } else if (strcmp(node->data.unary_expr.operator, "-") == 0) {
                if (operand.type == VAL_NUMBER) {
                    result.type = VAL_NUMBER;
                    result.as.number = -operand.as.number;
                } else {
                    fprintf(stderr, "Runtime hata: Negatif işlemi sadece sayılarda yapılabilir\n");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        }
        case AST_ASSIGN_EXPR: {
            RuntimeValue value = evaluate(node->data.assign_expr.value, env);
            env_define(env, node->data.assign_expr.name, value);
            result = value;
            break;
        }
        case AST_CALL_EXPR: {
            const char* func_name = node->data.call_expr.name;
            
            // Türkiye'ye özel built-in fonksiyonlar
            if (strcmp(func_name, "turkiye_sehir_mi") == 0) {
                if (node->data.call_expr.arg_count != 1) {
                    fprintf(stderr, "Runtime hata: turkiye_sehir_mi() 1 parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
                if (arg.type != VAL_STRING) {
                    fprintf(stderr, "Runtime hata: turkiye_sehir_mi() string parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                int func_result = turkiye_sehir_mi(arg.as.string);
                result.type = VAL_BOOLEAN;
                result.as.boolean = func_result;
                break;
            }
            
            if (strcmp(func_name, "tc_kimlik_gecerli") == 0) {
                if (node->data.call_expr.arg_count != 1) {
                    fprintf(stderr, "Runtime hata: tc_kimlik_gecerli() 1 parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
                
                // Number'ı string'e çevir
                char tc_str[20];
                if (arg.type == VAL_NUMBER) {
                    snprintf(tc_str, sizeof(tc_str), "%.0f", arg.as.number);
                } else if (arg.type == VAL_STRING) {
                    strncpy(tc_str, arg.as.string, sizeof(tc_str) - 1);
                    tc_str[sizeof(tc_str) - 1] = '\0';
                } else {
                    fprintf(stderr, "Runtime hata: tc_kimlik_gecerli() sayı veya string parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                
                int func_result = tc_kimlik_gecerli(tc_str);
                result.type = VAL_BOOLEAN;
                result.as.boolean = func_result;
                break;
            }
            
            if (strcmp(func_name, "tl_formatla") == 0) {
                if (node->data.call_expr.arg_count != 1) {
                    fprintf(stderr, "Runtime hata: tl_formatla() 1 parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
                if (arg.type != VAL_NUMBER) {
                    fprintf(stderr, "Runtime hata: tl_formatla() sayı parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                char* func_result = tl_formatla(arg.as.number);
                result.type = VAL_STRING;
                result.as.string = func_result;
                break;
            }
            
            if (strcmp(func_name, "turkiye_saati") == 0) {
                if (node->data.call_expr.arg_count != 0) {
                    fprintf(stderr, "Runtime hata: turkiye_saati() parametre almaz\n");
                    exit(EXIT_FAILURE);
                }
                char* func_result = turkiye_saati();
                result.type = VAL_STRING;
                result.as.string = func_result;
                break;
            }
            
            if (strcmp(func_name, "turkiye_cografya_bilgi") == 0) {
                if (node->data.call_expr.arg_count != 0) {
                    fprintf(stderr, "Runtime hata: turkiye_cografya_bilgi() parametre almaz\n");
                    exit(EXIT_FAILURE);
                }
                char* func_result = turkiye_cografya_bilgi();
                result.type = VAL_STRING;
                result.as.string = func_result;
                break;
            }
            
            if (strcmp(func_name, "turkiye_baskent_mi") == 0) {
                if (node->data.call_expr.arg_count != 1) {
                    fprintf(stderr, "Runtime hata: turkiye_baskent_mi() 1 parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
                if (arg.type != VAL_STRING) {
                    fprintf(stderr, "Runtime hata: turkiye_baskent_mi() string parametre alır\n");
                    exit(EXIT_FAILURE);
                }
                int func_result = turkiye_baskent_mi(arg.as.string);
                result.type = VAL_BOOLEAN;
                result.as.boolean = func_result;
                break;
            }
            
            // Diğer fonksiyonlar için NULL döndür
            result.type = VAL_NULL;
            break;
        }
        default:
            break;
    }
    
    return result;
}

/**
 * Değişken bildirimi ziyaret et
 */
static void visit_var_decl(ASTNode *node, SimpleEnvironment *env) {
    if (!node || !node->data.var_decl.name) {
        fprintf(stderr, "Runtime hata: Geçersiz değişken bildirimi\n");
        exit(EXIT_FAILURE);
    }
    
    RuntimeValue value = evaluate(node->data.var_decl.value, env);
    env_define(env, node->data.var_decl.name, value);
}

/**
 * Print statement ziyaret et
 */
static void visit_print_stmt(ASTNode *node, SimpleEnvironment *env) {
    if (!node || !node->data.print_stmt.expression) {
        fprintf(stderr, "Runtime hata: Geçersiz print statement\n");
        exit(EXIT_FAILURE);
    }
    
    RuntimeValue value = evaluate(node->data.print_stmt.expression, env);
    char *str = value_to_string(value);
    printf("%s\n", str);
    free(str);
}

/**
 * If statement ziyaret et
 */
static void visit_if_stmt(ASTNode *node, SimpleEnvironment *env) {
    if (!node || !node->data.if_stmt.condition) {
        fprintf(stderr, "Runtime hata: Geçersiz if statement\n");
        exit(EXIT_FAILURE);
    }
    
    RuntimeValue condition = evaluate(node->data.if_stmt.condition, env);
    
    int is_true = (condition.type == VAL_NUMBER && condition.as.number != 0) ||
                  (condition.type == VAL_STRING && strlen(condition.as.string) > 0) ||
                  (condition.type == VAL_BOOLEAN && condition.as.boolean);
    
    if (is_true && node->data.if_stmt.then_branch) {
        for (int i = 0; i < node->data.if_stmt.then_count; i++) {
            visit_node(node->data.if_stmt.then_branch[i], env);
        }
    } else if (!is_true && node->data.if_stmt.else_branch) {
        for (int i = 0; i < node->data.if_stmt.else_count; i++) {
            visit_node(node->data.if_stmt.else_branch[i], env);
        }
    }
}

/**
 * Loop statement ziyaret et
 */
static void visit_loop_stmt(ASTNode *node, SimpleEnvironment *env) {
    if (!node) {
        fprintf(stderr, "Runtime hata: Geçersiz loop statement\n");
        exit(EXIT_FAILURE);
    }
    
    RuntimeValue count_val = evaluate(node->data.loop_stmt.count_expr, env);
    if (count_val.type != VAL_NUMBER) {
        fprintf(stderr, "Runtime hata: Döngü sayısı bir numara olmalıdır.\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < count_val.as.number; i++) {
        for (int j = 0; j < node->data.loop_stmt.body_count; j++) {
            visit_node(node->data.loop_stmt.body[j], env);
        }
    }
}

/**
 * Function declaration ziyaret et
 */
static void visit_func_decl(ASTNode *node, SimpleEnvironment *env) {
    (void)env; // Unused
    if (!node || !node->data.func_decl.name) {
        fprintf(stderr, "Runtime hata: Geçersiz fonksiyon bildirimi\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Fonksiyon tanımlandı: %s\n", node->data.func_decl.name);
}

/**
 * Function call ziyaret et
 */
static void visit_func_call(ASTNode *node, SimpleEnvironment *env) {
    if (!node || !node->data.call_expr.name) {
        fprintf(stderr, "Runtime hata: Geçersiz fonksiyon çağrısı\n");
        exit(EXIT_FAILURE);
    }
    
    const char* func_name = node->data.call_expr.name;
    
    // Türkiye'ye özel built-in fonksiyonlar
    if (strcmp(func_name, "turkiye_sehir_mi") == 0) {
        if (node->data.call_expr.arg_count != 1) {
            fprintf(stderr, "Runtime hata: turkiye_sehir_mi() 1 parametre alır\n");
            exit(EXIT_FAILURE);
        }
        RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
        if (arg.type != VAL_STRING) {
            fprintf(stderr, "Runtime hata: turkiye_sehir_mi() string parametre alır\n");
            exit(EXIT_FAILURE);
        }
        int result = turkiye_sehir_mi(arg.as.string);
        printf("%s\n", result ? "doğru" : "yanlış");
        return;
    }
    
    if (strcmp(func_name, "tc_kimlik_gecerli") == 0) {
        if (node->data.call_expr.arg_count != 1) {
            fprintf(stderr, "Runtime hata: tc_kimlik_gecerli() 1 parametre alır\n");
            exit(EXIT_FAILURE);
        }
        RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
        
        // Number'ı string'e çevir
        char tc_str[20];
        if (arg.type == VAL_NUMBER) {
            snprintf(tc_str, sizeof(tc_str), "%.0f", arg.as.number);
        } else if (arg.type == VAL_STRING) {
            strncpy(tc_str, arg.as.string, sizeof(tc_str) - 1);
            tc_str[sizeof(tc_str) - 1] = '\0';
        } else {
            fprintf(stderr, "Runtime hata: tc_kimlik_gecerli() sayı veya string parametre alır\n");
            exit(EXIT_FAILURE);
        }
        
        int result = tc_kimlik_gecerli(tc_str);
        printf("%s\n", result ? "doğru" : "yanlış");
        return;
    }
    
    if (strcmp(func_name, "turkiye_telefon_gecerli") == 0) {
        if (node->data.call_expr.arg_count != 1) {
            fprintf(stderr, "Runtime hata: turkiye_telefon_gecerli() 1 parametre alır\n");
            exit(EXIT_FAILURE);
        }
        RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
        
        // Number'ı string'e çevir
        char phone_str[20];
        if (arg.type == VAL_NUMBER) {
            snprintf(phone_str, sizeof(phone_str), "%.0f", arg.as.number);
        } else if (arg.type == VAL_STRING) {
            strncpy(phone_str, arg.as.string, sizeof(phone_str) - 1);
            phone_str[sizeof(phone_str) - 1] = '\0';
        } else {
            fprintf(stderr, "Runtime hata: turkiye_telefon_gecerli() sayı veya string parametre alır\n");
            exit(EXIT_FAILURE);
        }
        
        int result = turkiye_telefon_gecerli(phone_str);
        printf("%s\n", result ? "doğru" : "yanlış");
        return;
    }
    
    if (strcmp(func_name, "tl_formatla") == 0) {
        if (node->data.call_expr.arg_count != 1) {
            fprintf(stderr, "Runtime hata: tl_formatla() 1 parametre alır\n");
            exit(EXIT_FAILURE);
        }
        RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
        if (arg.type != VAL_NUMBER) {
            fprintf(stderr, "Runtime hata: tl_formatla() sayı parametre alır\n");
            exit(EXIT_FAILURE);
        }
        char* result = tl_formatla(arg.as.number);
        printf("%s\n", result);
        return;
    }
    
    if (strcmp(func_name, "turkiye_saati") == 0) {
        if (node->data.call_expr.arg_count != 0) {
            fprintf(stderr, "Runtime hata: turkiye_saati() parametre almaz\n");
            exit(EXIT_FAILURE);
        }
        char* result = turkiye_saati();
        printf("%s\n", result);
        return;
    }
    
    if (strcmp(func_name, "turkiye_cografya_bilgi") == 0) {
        if (node->data.call_expr.arg_count != 0) {
            fprintf(stderr, "Runtime hata: turkiye_cografya_bilgi() parametre almaz\n");
            exit(EXIT_FAILURE);
        }
        char* result = turkiye_cografya_bilgi();
        printf("%s\n", result);
        return;
    }
    
    if (strcmp(func_name, "turkiye_baskent_mi") == 0) {
        if (node->data.call_expr.arg_count != 1) {
            fprintf(stderr, "Runtime hata: turkiye_baskent_mi() 1 parametre alır\n");
            exit(EXIT_FAILURE);
        }
        RuntimeValue arg = evaluate(node->data.call_expr.args[0], env);
        if (arg.type != VAL_STRING) {
            fprintf(stderr, "Runtime hata: turkiye_baskent_mi() string parametre alır\n");
            exit(EXIT_FAILURE);
        }
        int result = turkiye_baskent_mi(arg.as.string);
        printf("%s\n", result ? "doğru" : "yanlış");
        return;
    }
    
    // Diğer fonksiyonlar
    printf("Tanımsız fonksiyon: %s\n", func_name);
}

/**
 * Expression statement ziyaret et
 */
static void visit_expr_stmt(ASTNode *node, SimpleEnvironment *env) {
    if (!node) return;
    
    // Assignment expression'ları özel handle et
    if (node->type == AST_ASSIGN_EXPR) {
        RuntimeValue value = evaluate(node->data.assign_expr.value, env);
        env_define(env, node->data.assign_expr.name, value);
    } else {
        evaluate(node, env);
    }
}

/**
 * Binary expression ziyaret et
 */
static void visit_binary_expr(ASTNode *node, SimpleEnvironment *env) {
    if (!node) return;
    
    // Binary expression'ı evaluate et ve sonucu görmezden gel
    evaluate(node, env);
}

/**
 * Generic node visitor
 */
static void visit_node(ASTNode *node, SimpleEnvironment *env) {
    if (node && node->type < sizeof(visit_table)/sizeof(visit_table[0])) {
        VisitorFunc visitor = visit_table[node->type];
        if (visitor) {
            visitor(node, env);
        }
    }
}

/**
 * AST düğümlerini yorumla
 */
void interpret(ASTNode **nodes, int nodeCount, SimpleEnvironment *env) {
    if (!nodes || nodeCount == 0 || !env) {
        return;
    }
    
    for (int i = 0; i < nodeCount; i++) {
        visit_node(nodes[i], env);
    }
}