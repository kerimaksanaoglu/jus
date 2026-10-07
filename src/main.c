#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif

#include "common.h"
#include "io.h"
#include "stdlib_modules.h"
#include "vm.h"

static void printUsage(FILE *out) {
    fputs("Kullanım: jus [seçenek] [dosya.jus]\n"
          "\n"
          "  jus                  Etkileşimli kipi başlatır.\n"
          "  jus dosya.jus [...]  Dosyadaki programı çalıştırır; kalan argümanlar programa verilir.\n"
          "  jus test [yol]       Adı _test.jus ile biten dosyalardaki testleri çalıştırır.\n"
          "  jus --surum          Sürüm numarasını yazar.\n"
          "  jus --yardim         Bu yardım metnini yazar.\n",
          out);
}

static int runFile(const char *path, int argumentCount, char **arguments) {
    const char *problem = NULL;
    char *source = readSource(path, &problem);
    if (source == NULL) {
        fprintf(stderr, "jus: '%s': %s.\n", path, problem);
        return JUS_EXIT_NO_INPUT;
    }

    initVM();
    setScriptPath(path);
    setScriptArguments(argumentCount, arguments);
    InterpretResult result = interpret(path, source, false);
    freeVM();
    free(source);

    if (result == INTERPRET_COMPILE_ERROR) return JUS_EXIT_COMPILE_ERROR;
    if (result == INTERPRET_RUNTIME_ERROR) return JUS_EXIT_RUNTIME_ERROR;
    return 0;
}

static bool endsWith(const char *text, const char *suffix) {
    size_t length = strlen(text);
    size_t suffixLength = strlen(suffix);
    return length >= suffixLength && strcmp(text + length - suffixLength, suffix) == 0;
}

typedef struct {
    char **paths;
    int count;
    int capacity;
    const char *directory;
} PathList;

static void addPath(PathList *list, const char *directory, const char *name) {
    if (list->count == list->capacity) {
        list->capacity = list->capacity == 0 ? 16 : list->capacity * 2;
        list->paths = (char **)realloc(list->paths, sizeof(char *) * (size_t)list->capacity);
    }
    size_t length = strlen(directory) + strlen(name) + 2;
    char *path = (char *)malloc(length);
    if (list->paths == NULL || path == NULL) {
        fprintf(stderr, "jus: bellek yetersiz.\n");
        exit(JUS_EXIT_OUT_OF_MEMORY);
    }
    if (directory[0] == '\0') {
        snprintf(path, length, "%s", name);
    } else {
        snprintf(path, length, "%s/%s", directory, name);
    }
    list->paths[list->count++] = path;
}

static void collectEntry(const char *name, void *context) {
    PathList *list = (PathList *)context;
    if (name[0] == '.') return;
    addPath(list, list->directory, name);
}

static int comparePaths(const void *a, const void *b) {
    return strcmp(*(char *const *)a, *(char *const *)b);
}

/* path bir klasörse içindeki test dosyalarını (alt klasörler dahil) çalıştırır; dosyaysa onu. */
static void runTestsUnder(const char *path, int *files, int *passed, int *failed) {
    PathList entries = {NULL, 0, 0, path};
    if (listDirectory(path, collectEntry, &entries)) {
        qsort(entries.paths, (size_t)entries.count, sizeof(char *), comparePaths);
        for (int i = 0; i < entries.count; i++) {
            runTestsUnder(entries.paths[i], files, passed, failed);
            free(entries.paths[i]);
        }
        free(entries.paths);
        return;
    }

    if (!endsWith(path, "_test.jus")) return;
    const char *problem = NULL;
    char *source = readSource(path, &problem);
    if (source == NULL) return;

    (*files)++;
    initVM();
    setScriptPath(path);
    runTestFile(path, source, passed, failed);
    freeVM();
    free(source);
}

static int runTests(const char *path) {
    int files = 0, passed = 0, failed = 0;
    runTestsUnder(path, &files, &passed, &failed);

    if (files == 0) {
        fprintf(stderr, "jus: '%s' altında adı _test.jus ile biten dosya bulunamadı.\n", path);
        return JUS_EXIT_NO_INPUT;
    }
    printf("\n%d dosya, %d test: %d geçti, %d kaldı.\n", files, passed + failed, passed, failed);
    return failed == 0 ? 0 : 1;
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
    /* Çıktıda satır sonu her platformda '\n' olsun. */
    _setmode(_fileno(stdout), _O_BINARY);
    _setmode(_fileno(stderr), _O_BINARY);
    /* Argümanları sistemin kod sayfasıyla değil, UTF-8 olarak al. */
    int utf8Count = 0;
    char **utf8 = utf8Arguments(&utf8Count);
    if (utf8 != NULL) {
        argc = utf8Count;
        argv = utf8;
    }
#endif

    if (argc == 1) {
        repl();
        return 0;
    }

    if (strcmp(argv[1], "--surum") == 0) {
        printf("JUS %s\n", JUS_VERSION);
        return 0;
    }
    if (strcmp(argv[1], "--yardim") == 0) {
        printUsage(stdout);
        return 0;
    }
    if (strcmp(argv[1], "test") == 0) {
        if (argc > 3) {
            printUsage(stderr);
            return JUS_EXIT_USAGE;
        }
        return runTests(argc == 3 ? argv[2] : ".");
    }
    if (argv[1][0] == '-') {
        fprintf(stderr, "jus: bilinmeyen seçenek '%s'.\n\n", argv[1]);
        printUsage(stderr);
        return JUS_EXIT_USAGE;
    }

    /* Dosya adından sonraki argümanlar programa aktarılır (sistem.argümanlar). */
    return runFile(argv[1], argc - 2, argv + 2);
}
