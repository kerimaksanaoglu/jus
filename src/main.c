#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "common.h"
#include "vm.h"

static void printUsage(FILE *out) {
    fputs("Kullanım: jus [seçenek] [dosya.jus]\n"
          "\n"
          "  jus                  Etkileşimli kipi başlatır.\n"
          "  jus dosya.jus        Dosyadaki programı çalıştırır.\n"
          "  jus --surum          Sürüm numarasını yazar.\n"
          "  jus --yardim         Bu yardım metnini yazar.\n",
          out);
}

/* Dosyayı okur; hata durumunda ileti yazıp NULL döndürür. Kaynak UTF-8 olmalıdır. */
static char *readFile(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "jus: '%s' dosyası açılamadı.\n", path);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    if (fileSize < 0) {
        fprintf(stderr, "jus: '%s' dosyası okunamadı.\n", path);
        fclose(file);
        return NULL;
    }

    char *buffer = (char *)malloc((size_t)fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "jus: '%s' dosyası için bellek yetersiz.\n", path);
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, sizeof(char), (size_t)fileSize, file);
    fclose(file);
    if (bytesRead < (size_t)fileSize) {
        fprintf(stderr, "jus: '%s' dosyası okunamadı.\n", path);
        free(buffer);
        return NULL;
    }
    buffer[bytesRead] = '\0';

    const unsigned char *bytes = (const unsigned char *)buffer;
    if (bytesRead >= 2 &&
        ((bytes[0] == 0xFF && bytes[1] == 0xFE) || (bytes[0] == 0xFE && bytes[1] == 0xFF))) {
        fprintf(stderr, "jus: '%s' UTF-16 olarak kaydedilmiş. Kaynak dosyalar UTF-8 olmalıdır.\n",
                path);
        free(buffer);
        return NULL;
    }
    if (memchr(buffer, '\0', bytesRead) != NULL) {
        fprintf(stderr, "jus: '%s' bir metin dosyası değil. Kaynak dosyalar UTF-8 olmalıdır.\n",
                path);
        free(buffer);
        return NULL;
    }

    /* UTF-8 BOM varsa atla. */
    if (bytesRead >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) {
        memmove(buffer, buffer + 3, bytesRead - 3 + 1);
    }

    return buffer;
}

static int runFile(const char *path) {
    char *source = readFile(path);
    if (source == NULL) return JUS_EXIT_NO_INPUT;

    initVM();
    InterpretResult result = interpret(path, source, false);
    freeVM();
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) return JUS_EXIT_COMPILE_ERROR;
    if (result == INTERPRET_RUNTIME_ERROR) return JUS_EXIT_RUNTIME_ERROR;
    return 0;
}

/* Satırı sonundaki satır sonu karakterleri olmadan arabelleğe ekler. Girdi bittiyse false. */
static bool readLine(char **buffer, size_t *length, size_t *capacity) {
    bool readAny = false;
    int c;
    while ((c = fgetc(stdin)) != EOF) {
        readAny = true;
        if (c == '\n') break;
        if (*length + 2 >= *capacity) {
            *capacity = *capacity < 256 ? 256 : *capacity * 2;
            char *grown = (char *)realloc(*buffer, *capacity);
            if (grown == NULL) {
                fprintf(stderr, "jus: bellek yetersiz.\n");
                exit(JUS_EXIT_OUT_OF_MEMORY);
            }
            *buffer = grown;
        }
        (*buffer)[(*length)++] = (char)c;
    }
    if (*buffer != NULL) {
        if (*length > 0 && (*buffer)[*length - 1] == '\r') (*length)--;
        (*buffer)[*length] = '\0';
    }
    return readAny;
}

static bool endsWithColon(const char *text, size_t length) {
    while (length > 0 && (text[length - 1] == ' ' || text[length - 1] == '\t')) length--;
    return length > 0 && text[length - 1] == ':';
}

static void repl(void) {
    printf("JUS %s - çıkmak için 'çıkış' yazın.\n", JUS_VERSION);
    initVM();

    char *buffer = NULL;
    size_t capacity = 0;

    for (;;) {
        size_t length = 0;
        fputs(">>> ", stdout);
        fflush(stdout);
        if (!readLine(&buffer, &length, &capacity)) {
            fputc('\n', stdout);
            break;
        }
        if (buffer == NULL || length == 0) continue;
        if (strcmp(buffer, "çıkış") == 0) break;

        /* ':' ile biten satır bir blok açar; boş satıra kadar okumayı sürdür. */
        if (endsWithColon(buffer, length)) {
            for (;;) {
                buffer[length++] = '\n';
                size_t lineStart = length;
                fputs("... ", stdout);
                fflush(stdout);
                if (!readLine(&buffer, &length, &capacity)) break;
                if (length == lineStart) break;
            }
            buffer[length] = '\0';
        }

        interpret("<etkileşimli>", buffer, true);
    }

    free(buffer);
    freeVM();
}

int main(int argc, char *argv[]) {
#ifdef _WIN32
    /* Konsolun Türkçe karakterleri doğru göstermesi için UTF-8 kullan. */
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc == 1) {
        repl();
        return 0;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "--surum") == 0) {
            printf("JUS %s\n", JUS_VERSION);
            return 0;
        }
        if (strcmp(argv[1], "--yardim") == 0) {
            printUsage(stdout);
            return 0;
        }
        if (argv[1][0] == '-') {
            fprintf(stderr, "jus: bilinmeyen seçenek '%s'.\n\n", argv[1]);
            printUsage(stderr);
            return JUS_EXIT_USAGE;
        }
        return runFile(argv[1]);
    }

    printUsage(stderr);
    return JUS_EXIT_USAGE;
}
