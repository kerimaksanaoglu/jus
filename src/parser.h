#ifndef JUS_PARSER_H
#define JUS_PARSER_H

#include "../include/common.h"

/* Parser fonksiyon prototipleri */

/**
 * Token dizisini AST düğümlerine dönüştür
 * @param tokens Token dizisi
 * @param tokenCount Token sayısı
 * @param outAstCount Çıktı AST düğüm sayısı
 * @return AST düğüm dizisi
 */
ASTNode **parse(Token *tokens, int tokenCount, int *outAstCount);

/**
 * AST düğüm dizisini bellek temizliği için serbest bırak
 * @param nodes AST düğüm dizisi
 * @param count Düğüm sayısı
 */
void ast_free(ASTNode **nodes, int count);

#endif /* JUS_PARSER_H */