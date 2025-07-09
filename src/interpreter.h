#ifndef JUS_INTERPRETER_H
#define JUS_INTERPRETER_H

#include "parser.h"
#include "../include/common.h"

/* Simplified Environment yapısı */
typedef struct SimpleEnvironment {
    char **names;
    Value *values;
    int count;
    int capacity;
} SimpleEnvironment;

/* Interpreter fonksiyon prototipleri */

/**
 * AST düğümlerini yorumla
 * @param nodes AST düğüm dizisi
 * @param nodeCount Düğüm sayısı
 * @param env Çalışma ortamı
 */
void interpret(ASTNode **nodes, int nodeCount, SimpleEnvironment *env);

/**
 * Environment'ı başlat
 * @param env Environment pointer'ı
 */
void env_init(SimpleEnvironment *env);

/**
 * Environment'ı temizle
 * @param env Environment pointer'ı
 */
void env_free(SimpleEnvironment *env);

#endif /* JUS_INTERPRETER_H */