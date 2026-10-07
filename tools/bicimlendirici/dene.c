/*
 * Biçimlendirici sınama programı.
 *
 *   dene dosya.jus              biçimlendirilmiş kaynağı stdout'a yazar
 *   dene --belirtec dosya.jus   dosyanın belirteçlerini satır satır döker
 *
 * Kaynakta sözcük hatası varsa stderr'e "satır N: ileti" yazar, 2 ile çıkar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>

/* -std=c99 altında MinGW başlıkları _setmode bildirimini gizler. */
int _setmode(int fd, int mode);
#endif

#include "format.h"
#include "scanner.h"

static char *readFile(const char *path) {
    FILE *f = fopen(path, "rb");
    if (f == NULL) return NULL;
    size_t cap = 4096, len = 0;
    char *buf = (char *)malloc(cap);
    if (buf == NULL) {
        fclose(f);
        return NULL;
    }
    size_t got;
    while ((got = fread(buf + len, 1, cap - len - 1, f)) > 0) {
        len += got;
        if (len + 1 >= cap) {
            cap *= 2;
            buf = (char *)realloc(buf, cap);
            if (buf == NULL) {
                fclose(f);
                return NULL;
            }
        }
    }
    fclose(f);
    buf[len] = '\0';
    return buf;
}

static void dumpTokens(const char *source) {
    initScanner(source);
    for (;;) {
        Token t = scanToken();
        if (t.type == TOKEN_EOF) break;
        if (t.type == TOKEN_ERROR) {
            printf("%d\tERROR\t%s\n", (int)t.type, t.message);
            break;
        }
        if (t.type == TOKEN_NEWLINE || t.type == TOKEN_INDENT || t.type == TOKEN_DEDENT) {
            printf("%d\n", (int)t.type);
        } else {
            printf("%d\t%.*s\n", (int)t.type, t.length, t.start);
        }
    }
}

int main(int argc, char **argv) {
    int tokenMode = 0;
    const char *path = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--belirtec") == 0) tokenMode = 1;
        else path = argv[i];
    }
    if (path == NULL) {
        fprintf(stderr, "Kullanım: dene [--belirtec] dosya.jus\n");
        return 64;
    }
    char *source = readFile(path);
    if (source == NULL) {
        fprintf(stderr, "Dosya okunamadı: %s\n", path);
        return 66;
    }
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    if (tokenMode) {
        /* BOM'u biçimlendiriciyle aynı şekilde atla. */
        const char *text = source;
        if ((unsigned char)text[0] == 0xEF && (unsigned char)text[1] == 0xBB &&
            (unsigned char)text[2] == 0xBF) {
            text += 3;
        }
        dumpTokens(text);
        free(source);
        return 0;
    }
    const char *problem = NULL;
    int line = 0;
    char *out = formatSource(source, &problem, &line);
    if (out == NULL) {
        fprintf(stderr, "satır %d: %s\n", line, problem);
        free(source);
        return 2;
    }
    fwrite(out, 1, strlen(out), stdout);
    free(out);
    free(source);
    return 0;
}
