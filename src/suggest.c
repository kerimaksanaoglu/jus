#include <stdio.h>
#include <string.h>

#include "suggest.h"
#include "text.h"

static const char *const keywords[] = {
    "boş", "bu", "değil", "değilse", "değişken", "dene", "devam", "doğru", "dön",
    "eğer", "fırlat", "fonksiyon", "geç", "her", "içinde", "iken", "kır", "kullan",
    "olarak", "sınıf", "üst", "ve", "veya", "yakala", "yanlış",
};

/* Başka dillerden gelen alışkanlıklar: kelime -> JUS karşılığı. */
static const char *const foreign[][2] = {
    {"print", "yaz"}, {"println", "yaz"}, {"printf", "yaz"}, {"echo", "yaz"},
    {"input", "oku"}, {"len", "uzunluk"}, {"length", "uzunluk"},
    {"str", "metin"}, {"string", "metin"}, {"int", "sayı"}, {"float", "sayı"},
    {"number", "sayı"}, {"type", "tür"}, {"range", "aralık"},
    {"append", "ekle"}, {"push", "ekle"}, {"pop", "çıkar"}, {"sort", "sırala"},
    {"sorted", "sırala"}, {"map", "eşle"}, {"filter", "süz"},
    {"true", "doğru"}, {"false", "yanlış"}, {"null", "boş"}, {"none", "boş"},
    {"nil", "boş"}, {"if", "eğer"}, {"else", "değilse"}, {"elif", "değilse eğer"},
    {"while", "iken"}, {"for", "her"}, {"in", "içinde"}, {"def", "fonksiyon"},
    {"function", "fonksiyon"}, {"func", "fonksiyon"}, {"return", "dön"},
    {"and", "ve"}, {"or", "veya"}, {"not", "değil"}, {"break", "kır"},
    {"continue", "devam"}, {"pass", "geç"}, {"class", "sınıf"}, {"self", "bu"},
    {"this", "bu"}, {"super", "üst"}, {"import", "kullan"}, {"try", "dene"},
    {"except", "yakala"}, {"catch", "yakala"}, {"raise", "fırlat"},
    {"throw", "fırlat"}, {"var", "değişken"}, {"let", "değişken"},
};

/*
 * Adı karşılaştırma için sadeleştirir: küçük harfe çevirir ve Türkçe harfleri
 * ASCII karşılıklarına indirger. Yazılan bayt sayısını döndürür; ad arabelleğe
 * sığmazsa -1.
 */
static int foldName(const char *name, int length, char *out, int size) {
    int written = 0;
    int offset = 0;
    while (offset < length) {
        uint32_t point;
        offset += utf8Decode(name + offset, length - offset, &point);
        point = trLower(point);
        switch (point) {
            case 0xE7: point = 'c'; break;  /* ç */
            case 0x11F: point = 'g'; break; /* ğ */
            case 0x131: point = 'i'; break; /* ı */
            case 0xF6: point = 'o'; break;  /* ö */
            case 0x15F: point = 's'; break; /* ş */
            case 0xFC: point = 'u'; break;  /* ü */
            case 0xE2: point = 'a'; break;  /* â */
            case 0xEE: point = 'i'; break;  /* î */
            case 0xFB: point = 'u'; break;  /* û */
            default: break;
        }
        if (written + 4 >= size) return -1;
        written += utf8Encode(point, out + written);
    }
    out[written] = '\0';
    return written;
}

/* Damerau-Levenshtein uzaklığı: ekleme, silme, değiştirme ve komşu iki harfin yer değiştirmesi. */
static int editDistance(const char *a, int aLength, const char *b, int bLength) {
    int rows[3][SUGGEST_MAX_NAME + 1];
    int *previous2 = rows[0], *previous = rows[1], *current = rows[2];

    for (int j = 0; j <= bLength; j++) previous[j] = j;
    for (int i = 1; i <= aLength; i++) {
        current[0] = i;
        for (int j = 1; j <= bLength; j++) {
            int cost = a[i - 1] == b[j - 1] ? 0 : 1;
            int best = previous[j - 1] + cost;
            if (previous[j] + 1 < best) best = previous[j] + 1;
            if (current[j - 1] + 1 < best) best = current[j - 1] + 1;
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1] &&
                previous2[j - 2] + 1 < best) {
                best = previous2[j - 2] + 1;
            }
            current[j] = best;
        }
        int *rotated = previous2;
        previous2 = previous;
        previous = current;
        current = rotated;
    }
    return previous[bLength];
}

void suggestionInit(Suggestion *suggestion, const char *target, int length) {
    suggestion->target = target;
    suggestion->targetLength = length;
    suggestion->best = NULL;
    suggestion->bestLength = 0;
    suggestion->bestScore = 0;
    suggestion->foldedLength = foldName(target, length, suggestion->folded, SUGGEST_MAX_NAME);
    suggestion->active = suggestion->foldedLength > 0;
}

void suggestionConsider(Suggestion *suggestion, const char *candidate, int length) {
    if (!suggestion->active || length == 0) return;
    if (length == suggestion->targetLength && memcmp(candidate, suggestion->target, (size_t)length) == 0) {
        return;
    }

    char folded[SUGGEST_MAX_NAME];
    int foldedLength = foldName(candidate, length, folded, SUGGEST_MAX_NAME);
    if (foldedLength <= 0) return;

    /* Kısa adlarda tek harf farkı çoğu zaman başka bir kelimedir; eşik ada göre büyür. */
    int limit = suggestion->foldedLength <= 3 ? 0 : suggestion->foldedLength <= 6 ? 1 : 2;
    int difference = foldedLength - suggestion->foldedLength;
    if (difference < -limit || difference > limit) return;

    int score = editDistance(suggestion->folded, suggestion->foldedLength, folded, foldedLength);
    if (score > limit) return;
    if (suggestion->best == NULL || score < suggestion->bestScore) {
        suggestion->best = candidate;
        suggestion->bestLength = length;
        suggestion->bestScore = score;
    }
}

void suggestionConsiderKeywords(Suggestion *suggestion) {
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        suggestionConsider(suggestion, keywords[i], (int)strlen(keywords[i]));
    }
}

bool suggestionFound(const Suggestion *suggestion) {
    return suggestion->best != NULL;
}

const char *foreignEquivalent(const char *name, int length) {
    char folded[SUGGEST_MAX_NAME];
    int foldedLength = foldName(name, length, folded, SUGGEST_MAX_NAME);
    if (foldedLength <= 0) return NULL;
    /* "I" Türkçe kurallarla "ı" olur ve "i"ye indirgenir; "IF" gibi yazımlar da eşleşir. */
    for (size_t i = 0; i < sizeof(foreign) / sizeof(foreign[0]); i++) {
        if ((int)strlen(foreign[i][0]) == foldedLength && memcmp(foreign[i][0], folded, (size_t)foldedLength) == 0) {
            return foreign[i][1];
        }
    }
    return NULL;
}

/* Soru ekini ("mı", "mi", "mu", "mü") kelimenin son ünlüsüne göre seçer. */
static const char *questionSuffix(const char *word, int length) {
    const char *suffix = "mi";
    int offset = 0;
    while (offset < length) {
        uint32_t point;
        offset += utf8Decode(word + offset, length - offset, &point);
        switch (trLower(point)) {
            case 'a': case 0x131: case 0xE2: suffix = "mı"; break;
            case 'e': case 'i': case 0xEE: suffix = "mi"; break;
            case 'o': case 'u': case 0xFB: suffix = "mu"; break;
            case 0xF6: case 0xFC: suffix = "mü"; break;
            default: break;
        }
    }
    return suffix;
}

void suggestionSentence(const Suggestion *suggestion, char *out, size_t size) {
    out[0] = '\0';
    const char *equivalent = foreignEquivalent(suggestion->target, suggestion->targetLength);
    if (equivalent != NULL) {
        snprintf(out, size, " JUS'ta bunun karşılığı '%s'.", equivalent);
        return;
    }
    if (suggestion->best == NULL) return;
    snprintf(out, size, " '%.*s' %s demek istediniz?", suggestion->bestLength, suggestion->best,
             questionSuffix(suggestion->best, suggestion->bestLength));
}
