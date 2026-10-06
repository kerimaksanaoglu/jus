#include <string.h>

#include "text.h"

int utf8Decode(const char *text, int remaining, uint32_t *codePoint) {
    unsigned char first = (unsigned char)text[0];
    if (first < 0x80) {
        *codePoint = first;
        return 1;
    }

    int size = (first & 0xE0) == 0xC0 ? 2 : (first & 0xF0) == 0xE0 ? 3 : (first & 0xF8) == 0xF0 ? 4 : 0;
    if (size == 0 || size > remaining) {
        *codePoint = first;
        return 1;
    }

    uint32_t value = first & (0xFFu >> (size + 1));
    for (int i = 1; i < size; i++) {
        unsigned char next = (unsigned char)text[i];
        if ((next & 0xC0) != 0x80) {
            *codePoint = first;
            return 1;
        }
        value = (value << 6) | (next & 0x3F);
    }

    *codePoint = value;
    return size;
}

int utf8Encode(uint32_t codePoint, char *out) {
    if (codePoint < 0x80) {
        out[0] = (char)codePoint;
        return 1;
    }
    if (codePoint < 0x800) {
        out[0] = (char)(0xC0 | (codePoint >> 6));
        out[1] = (char)(0x80 | (codePoint & 0x3F));
        return 2;
    }
    if (codePoint < 0x10000) {
        out[0] = (char)(0xE0 | (codePoint >> 12));
        out[1] = (char)(0x80 | ((codePoint >> 6) & 0x3F));
        out[2] = (char)(0x80 | (codePoint & 0x3F));
        return 3;
    }
    out[0] = (char)(0xF0 | (codePoint >> 18));
    out[1] = (char)(0x80 | ((codePoint >> 12) & 0x3F));
    out[2] = (char)(0x80 | ((codePoint >> 6) & 0x3F));
    out[3] = (char)(0x80 | (codePoint & 0x3F));
    return 4;
}

int utf8Length(const char *text, int byteLength) {
    int count = 0;
    int offset = 0;
    while (offset < byteLength) {
        uint32_t codePoint;
        offset += utf8Decode(text + offset, byteLength - offset, &codePoint);
        count++;
    }
    return count;
}

int utf8Offset(const char *text, int byteLength, int charIndex) {
    int offset = 0;
    while (charIndex > 0 && offset < byteLength) {
        uint32_t codePoint;
        offset += utf8Decode(text + offset, byteLength - offset, &codePoint);
        charIndex--;
    }
    return offset;
}

/* ASCII dışındaki küçük/büyük harf çiftleri (ı ve İ ayrıca ele alınır). */
static const uint32_t casePairs[][2] = {
    {0xE7, 0xC7},   /* ç Ç */
    {0x11F, 0x11E}, /* ğ Ğ */
    {0xF6, 0xD6},   /* ö Ö */
    {0x15F, 0x15E}, /* ş Ş */
    {0xFC, 0xDC},   /* ü Ü */
    {0xE2, 0xC2},   /* â Â */
    {0xEE, 0xCE},   /* î Î */
    {0xFB, 0xDB},   /* û Û */
};

#define CASE_PAIR_COUNT (sizeof(casePairs) / sizeof(casePairs[0]))

uint32_t trLower(uint32_t codePoint) {
    if (codePoint == 'I') return 0x131;  /* I -> ı */
    if (codePoint == 0x130) return 'i';  /* İ -> i */
    if (codePoint >= 'A' && codePoint <= 'Z') return codePoint + 32;
    for (size_t i = 0; i < CASE_PAIR_COUNT; i++) {
        if (casePairs[i][1] == codePoint) return casePairs[i][0];
    }
    return codePoint;
}

uint32_t trUpper(uint32_t codePoint) {
    if (codePoint == 'i') return 0x130;  /* i -> İ */
    if (codePoint == 0x131) return 'I';  /* ı -> I */
    if (codePoint >= 'a' && codePoint <= 'z') return codePoint - 32;
    for (size_t i = 0; i < CASE_PAIR_COUNT; i++) {
        if (casePairs[i][0] == codePoint) return casePairs[i][1];
    }
    return codePoint;
}

/* Türk alfabesi sırası; q, w, x İngilizcedeki yerlerinde. */
static const uint32_t alphabet[] = {
    'a', 'b', 'c', 0xE7, 'd', 'e', 'f', 'g', 0x11F, 'h', 0x131, 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 0xF6, 'p', 'q', 'r', 's', 0x15F, 't', 'u', 0xFC, 'v', 'w', 'x', 'y', 'z',
};

#define ALPHABET_SIZE (sizeof(alphabet) / sizeof(alphabet[0]))

/* Harfler alfabe sırasına, diğer karakterler kod noktalarına göre sıralanır. */
static uint32_t collationKey(uint32_t codePoint) {
    uint32_t lower = trLower(codePoint);
    if (lower == 0xE2) lower = 'a';
    else if (lower == 0xEE) lower = 'i';
    else if (lower == 0xFB) lower = 'u';

    for (size_t i = 0; i < ALPHABET_SIZE; i++) {
        if (alphabet[i] == lower) return 0x110000u + (uint32_t)i;
    }
    return codePoint;
}

int trCompare(const char *a, int aLength, const char *b, int bLength) {
    int aOffset = 0;
    int bOffset = 0;
    while (aOffset < aLength && bOffset < bLength) {
        uint32_t aPoint, bPoint;
        aOffset += utf8Decode(a + aOffset, aLength - aOffset, &aPoint);
        bOffset += utf8Decode(b + bOffset, bLength - bOffset, &bPoint);
        uint32_t aKey = collationKey(aPoint);
        uint32_t bKey = collationKey(bPoint);
        if (aKey != bKey) return aKey < bKey ? -1 : 1;
    }
    if (aOffset < aLength) return 1;
    if (bOffset < bLength) return -1;

    /* Harfleri aynı, yalnızca büyük/küçüklüğü farklı metinler için kararlı bir sıra. */
    int order = memcmp(a, b, (size_t)(aLength < bLength ? aLength : bLength));
    if (order != 0) return order;
    return aLength - bLength;
}
