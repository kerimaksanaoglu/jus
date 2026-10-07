#ifndef JUS_TEXT_H
#define JUS_TEXT_H

#include "common.h"

/*
 * UTF-8 ve Türkçe metin yardımcıları. Uzunluklar ve dizinler bayt değil,
 * karakter (Unicode kod noktası) cinsindendir.
 */

/* Bir kod noktası çözer, tükettiği bayt sayısını döndürür. Geçersiz bayt tek başına sayılır. */
int utf8Decode(const char *text, int remaining, uint32_t *codePoint);
/* Kod noktasını yazar (en çok 4 bayt), yazılan bayt sayısını döndürür. */
int utf8Encode(uint32_t codePoint, char *out);
int utf8Length(const char *text, int byteLength);
/* charIndex'inci karakterin bayt konumu; charIndex karakter sayısına eşitse byteLength. */
int utf8Offset(const char *text, int byteLength, int charIndex);

/* Türkçe kurallarıyla harf dönüşümü: i <-> İ, ı <-> I. */
uint32_t trLower(uint32_t codePoint);
uint32_t trUpper(uint32_t codePoint);

/* Türk alfabesi sırasına göre karşılaştırır: negatif, sıfır ya da pozitif. */
int trCompare(const char *a, int aLength, const char *b, int bLength);

#endif
