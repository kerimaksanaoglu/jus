#ifndef JUS_BYTES_H
#define JUS_BYTES_H

#include "object.h"

/*
 * baytlar türü: değiştirilebilir bayt dizisi. Öğeler 0-255 arası tam
 * değerlerdir; dizinleme, dilimleme, 'her' ve '+' metinlerdeki kurallarla çalışır.
 */

ObjBytes *newBytes(int count);
void bytesAppend(ObjBytes *bytes, uint8_t value);
/* Değer 0-255 arasında tam sayı mı? Değilse nativeFail ile ileti bırakır. */
bool bytesElement(const char *what, Value value, uint8_t *out);
/* UTF-8 geçerliliği: geçersizse false. */
bool validUtf8(const uint8_t *data, int length);

/* baytlar(...) yerleşik fonksiyonu ve 'bayt' modülü. */
void defineBytesBuiltins(void);
void defineBytesModule(void);

#endif
