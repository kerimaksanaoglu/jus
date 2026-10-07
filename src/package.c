#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "io.h"
#include "package.h"

/*
 * Paket yöneticisi. Paketler git depolarıdır ve bulunulan klasördeki
 * jus_paketleri/<ad>/ altına indirilir. 'kullan ad' deyimi, standart
 * kütüphanede ve dosyanın kendi klasöründe bulamadığı modülü
 * jus_paketleri/ad/ad.jus dosyasında arar.
 */

#define PACKAGE_DIRECTORY "jus_paketleri"
#define MAX_PATH_LENGTH 1024

static void printPackageUsage(FILE *out) {
    fputs("Kullanım: jus paket <komut>\n"
          "\n"
          "  jus paket kur <git-adresi> [ad]   Paketi jus_paketleri/<ad> altına indirir.\n"
          "  jus paket listele                 Kurulu paketleri listeler.\n"
          "  jus paket kaldır <ad>             Paketi siler.\n",
          out);
}

/* Paket adı klasör adı olarak kullanılır: yalnızca harf, rakam, '_' ve '-'. */
static bool isValidName(const char *name) {
    if (name[0] == '\0' || strlen(name) > 100) return false;
    for (const char *p = name; *p != '\0'; p++) {
        unsigned char c = (unsigned char)*p;
        bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                  c == '_' || c == '-' || c >= 0x80;
        if (!ok) return false;
    }
    return true;
}

/* Adres kabuk komutuna yazılır; yalnızca adreslerde geçen zararsız karakterlere izin verilir. */
static bool isSafeAddress(const char *address) {
    if (address[0] == '\0' || address[0] == '-' || strlen(address) > 500) return false;
    for (const char *p = address; *p != '\0'; p++) {
        char c = *p;
        bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                  strchr(":/._-@~+\\", c) != NULL;
        if (!ok) return false;
    }
    return true;
}

/* Adresin son parçasından paket adını türetir: .../jus-renkler.git -> renkler */
static void nameFromAddress(const char *address, char *name, size_t size) {
    const char *start = address;
    for (const char *p = address; *p != '\0'; p++) {
        if ((*p == '/' || *p == '\\' || *p == ':') && p[1] != '\0') start = p + 1;
    }
    snprintf(name, size, "%s", start);

    size_t length = strlen(name);
    while (length > 0 && (name[length - 1] == '/' || name[length - 1] == '\\')) name[--length] = '\0';
    if (length > 4 && strcmp(name + length - 4, ".git") == 0) name[length - 4] = '\0';
    if (strncmp(name, "jus-", 4) == 0) memmove(name, name + 4, strlen(name + 4) + 1);
}

static bool fileExists(const char *path) {
    FILE *file = openFile(path, "rb");
    if (file == NULL) return false;
    fclose(file);
    return true;
}

static void noop(const char *name, void *context) {
    (void)name;
    (void)context;
}

static bool directoryExists(const char *path) {
    return listDirectory(path, noop, NULL);
}

typedef struct {
    const char *directory;
    bool ok;
} RemoveContext;

static bool removeTree(const char *path);

static void removeEntry(const char *name, void *context) {
    RemoveContext *remove = (RemoveContext *)context;
    char path[MAX_PATH_LENGTH];
    if (snprintf(path, sizeof(path), "%s/%s", remove->directory, name) >= (int)sizeof(path)) {
        remove->ok = false;
        return;
    }
    if (!removeTree(path)) remove->ok = false;
}

/* Dosyayı ya da klasörü içindekilerle birlikte siler. */
static bool removeTree(const char *path) {
    RemoveContext context = {path, true};
    if (listDirectory(path, removeEntry, &context)) {
        return context.ok && removeDirectory(path);
    }
    return removeFile(path);
}

static int install(const char *address, const char *requestedName) {
    if (!isSafeAddress(address)) {
        fprintf(stderr, "jus: '%s' geçerli bir git adresi değil.\n", address);
        return JUS_EXIT_USAGE;
    }

    char name[512];
    if (requestedName != NULL) {
        snprintf(name, sizeof(name), "%s", requestedName);
    } else {
        nameFromAddress(address, name, sizeof(name));
    }
    if (!isValidName(name)) {
        fprintf(stderr, "jus: '%s' geçerli bir paket adı değil; adı ayrıca belirtin: "
                        "jus paket kur <adres> <ad>\n", name);
        return JUS_EXIT_USAGE;
    }

    char target[MAX_PATH_LENGTH];
    snprintf(target, sizeof(target), "%s/%.200s", PACKAGE_DIRECTORY, name);
    if (directoryExists(target) || fileExists(target)) {
        fprintf(stderr, "jus: '%s' paketi zaten kurulu. Yeniden kurmak için önce kaldırın: "
                        "jus paket kaldır %s\n", name, name);
        return 1;
    }
    makeDirectory(PACKAGE_DIRECTORY);

    char command[MAX_PATH_LENGTH * 2];
    snprintf(command, sizeof(command), "git clone --depth 1 --quiet \"%s\" \"%s\"", address, target);
    fflush(stdout);
    if (system(command) != 0) {
        fprintf(stderr, "jus: '%s' indirilemedi. git kurulu mu, adres doğru mu?\n", address);
        return 1;
    }

    char entry[MAX_PATH_LENGTH * 2];
    snprintf(entry, sizeof(entry), "%s/%s.jus", target, name);
    printf("'%s' paketi %s klasörüne kuruldu.\n", name, target);
    if (!fileExists(entry)) {
        printf("Uyarı: pakette '%s.jus' dosyası yok; 'kullan %s' çalışmayacak. "
               "Paketin modülleri 'kullan \"%s/<modül>\"' ile kullanılabilir.\n", name, name, name);
    } else {
        printf("Kullanmak için: kullan %s\n", name);
    }
    return 0;
}

static void printEntry(const char *name, void *context) {
    int *count = (int *)context;
    if (name[0] == '.') return;
    printf("%s\n", name);
    (*count)++;
}

static int list(void) {
    int count = 0;
    listDirectory(PACKAGE_DIRECTORY, printEntry, &count);
    if (count == 0) printf("Kurulu paket yok.\n");
    return 0;
}

static int uninstall(const char *name) {
    if (!isValidName(name)) {
        fprintf(stderr, "jus: '%s' geçerli bir paket adı değil.\n", name);
        return JUS_EXIT_USAGE;
    }
    char target[MAX_PATH_LENGTH];
    snprintf(target, sizeof(target), "%s/%s", PACKAGE_DIRECTORY, name);
    if (!directoryExists(target)) {
        fprintf(stderr, "jus: '%s' adında kurulu bir paket yok.\n", name);
        return 1;
    }
    if (!removeTree(target)) {
        fprintf(stderr, "jus: '%s' klasörü tümüyle silinemedi.\n", target);
        return 1;
    }
    printf("'%s' paketi kaldırıldı.\n", name);
    return 0;
}

int runPackageCommand(int argc, char **argv) {
    if (argc >= 1 && strcmp(argv[0], "kur") == 0 && (argc == 2 || argc == 3)) {
        return install(argv[1], argc == 3 ? argv[2] : NULL);
    }
    if (argc == 1 && strcmp(argv[0], "listele") == 0) {
        return list();
    }
    if (argc == 2 && (strcmp(argv[0], "kaldır") == 0 || strcmp(argv[0], "kaldir") == 0)) {
        return uninstall(argv[1]);
    }

    printPackageUsage(stderr);
    return JUS_EXIT_USAGE;
}
