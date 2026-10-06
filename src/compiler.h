#ifndef JUS_COMPILER_H
#define JUS_COMPILER_H

#include "object.h"

/*
 * Kaynağı bayt koduna derler. Hata varsa iletileri stderr'e yazıp NULL döndürür.
 * repl doğruysa üst düzey ifadelerin sonucu ekrana yazılır. Derlenen tüm
 * fonksiyonlar verilen modüle bağlanır.
 */
ObjFunction *compile(const char *name, const char *source, bool repl, ObjModule *module);
void markCompilerRoots(void);

#endif
