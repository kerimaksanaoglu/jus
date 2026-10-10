#ifndef JUS_SUGGEST_H
#define JUS_SUGGEST_H

#include "common.h"

/*
 * Hata iletilerindeki "şunu mu demek istediniz?" önerileri.
 *
 * Bulunamayan bir ad için, bilinen adlar arasından en yakını seçilir. Yakınlık
 * ölçülürken Türkçe harfler ASCII karşılıklarıyla eş sayılır; böylece Türkçe
 * karakter kullanmadan yazılmış adlar ("sayac", "eger") doğru adla eşleşir.
 */

#define SUGGEST_MAX_NAME 64

typedef struct {
    char folded[SUGGEST_MAX_NAME]; /* aranan adın sadeleştirilmiş hâli */
    int foldedLength;
    const char *target;
    int targetLength;
    const char *best;
    int bestLength;
    int bestScore;
    bool active; /* aranan ad öneri için uygun mu (çok uzun değil) */
} Suggestion;

void suggestionInit(Suggestion *suggestion, const char *target, int length);
/* Bir adayı değerlendirir; aday, aranan adın kendisiyse yok sayılır. */
void suggestionConsider(Suggestion *suggestion, const char *candidate, int length);
/* Dilin anahtar kelimelerini aday olarak değerlendirir. */
void suggestionConsiderKeywords(Suggestion *suggestion);
/* Yeterince yakın bir aday bulunduysa true döner. */
bool suggestionFound(const Suggestion *suggestion);

/*
 * Başka dillerden bilinen bir kelimenin (print, len, if ...) JUS karşılığını
 * döndürür; böyle bir kelime değilse NULL.
 */
const char *foreignEquivalent(const char *name, int length);

/*
 * Öneriyi iletinin sonuna eklenecek bir cümle olarak yazar, örneğin:
 * " 'sayaç' mı demek istediniz?". Öneri yoksa boş metin yazar.
 */
void suggestionSentence(const Suggestion *suggestion, char *out, size_t size);

#endif
