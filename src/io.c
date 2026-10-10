#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include "io.h"

#ifdef _WIN32

/* UTF-8 metni geniş karakterli metne çevirir; çağıran free ile serbest bırakır. */
static wchar_t *toWide(const char *text) {
    int length = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (length <= 0) return NULL;
    wchar_t *wide = (wchar_t *)malloc(sizeof(wchar_t) * (size_t)length);
    if (wide == NULL) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, text, -1, wide, length);
    return wide;
}

static char *fromWide(const wchar_t *wide) {
    int length = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
    if (length <= 0) return NULL;
    char *text = (char *)malloc((size_t)length);
    if (text == NULL) return NULL;
    WideCharToMultiByte(CP_UTF8, 0, wide, -1, text, length, NULL, NULL);
    return text;
}

FILE *openFile(const char *path, const char *mode) {
    wchar_t *widePath = toWide(path);
    wchar_t *wideMode = toWide(mode);
    FILE *file = NULL;
    if (widePath != NULL && wideMode != NULL) file = _wfopen(widePath, wideMode);
    free(widePath);
    free(wideMode);
    return file;
}

bool removeFile(const char *path) {
    wchar_t *wide = toWide(path);
    bool removed = wide != NULL && DeleteFileW(wide) != 0;
    if (wide != NULL && !removed) {
        /* Salt okunur dosyalar silinemez; özniteliği kaldırıp yeniden dene. */
        SetFileAttributesW(wide, FILE_ATTRIBUTE_NORMAL);
        removed = DeleteFileW(wide) != 0;
    }
    free(wide);
    return removed;
}

bool makeDirectory(const char *path) {
    wchar_t *wide = toWide(path);
    bool created = wide != NULL && CreateDirectoryW(wide, NULL) != 0;
    free(wide);
    return created;
}

bool removeDirectory(const char *path) {
    wchar_t *wide = toWide(path);
    bool removed = wide != NULL && RemoveDirectoryW(wide) != 0;
    free(wide);
    return removed;
}

bool listDirectory(const char *path, void (*each)(const char *name, void *context), void *context) {
    size_t length = strlen(path);
    char *pattern = (char *)malloc(length + 3);
    if (pattern == NULL) return false;
    memcpy(pattern, path, length);
    memcpy(pattern + length, "\\*", 3);
    wchar_t *wide = toWide(pattern);
    free(pattern);
    if (wide == NULL) return false;

    WIN32_FIND_DATAW entry;
    HANDLE handle = FindFirstFileW(wide, &entry);
    free(wide);
    if (handle == INVALID_HANDLE_VALUE) return false;

    do {
        if (wcscmp(entry.cFileName, L".") == 0 || wcscmp(entry.cFileName, L"..") == 0) continue;
        char *name = fromWide(entry.cFileName);
        if (name != NULL) {
            each(name, context);
            free(name);
        }
    } while (FindNextFileW(handle, &entry) != 0);
    FindClose(handle);
    return true;
}

char **utf8Arguments(int *count) {
    wchar_t **wide = CommandLineToArgvW(GetCommandLineW(), count);
    if (wide == NULL) return NULL;
    char **arguments = (char **)malloc(sizeof(char *) * (size_t)(*count + 1));
    if (arguments == NULL) return NULL;
    for (int i = 0; i < *count; i++) {
        arguments[i] = fromWide(wide[i]);
        if (arguments[i] == NULL) return NULL;
    }
    arguments[*count] = NULL;
    LocalFree(wide);
    return arguments;
}

#else

FILE *openFile(const char *path, const char *mode) {
    return fopen(path, mode);
}

bool removeFile(const char *path) {
    return remove(path) == 0;
}

bool makeDirectory(const char *path) {
    return mkdir(path, 0777) == 0;
}

bool removeDirectory(const char *path) {
    return rmdir(path) == 0;
}

bool listDirectory(const char *path, void (*each)(const char *name, void *context), void *context) {
    DIR *directory = opendir(path);
    if (directory == NULL) return false;

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        each(entry->d_name, context);
    }
    closedir(directory);
    return true;
}

#endif

unsigned char *readBinaryFile(const char *path, size_t *size, const char **problem) {
    FILE *file = openFile(path, "rb");
    if (file == NULL) {
        *problem = "dosya açılamadı";
        return NULL;
    }
    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    if (fileSize < 0) {
        *problem = "dosya okunamadı";
        fclose(file);
        return NULL;
    }
    unsigned char *buffer = (unsigned char *)malloc((size_t)fileSize + 1);
    if (buffer == NULL) {
        *problem = "bellek yetersiz";
        fclose(file);
        return NULL;
    }
    size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
    fclose(file);
    if (bytesRead < (size_t)fileSize) {
        *problem = "dosya okunamadı";
        free(buffer);
        return NULL;
    }
    *size = bytesRead;
    return buffer;
}

char *readSource(const char *path, const char **problem) {
    FILE *file = openFile(path, "rb");
    if (file == NULL) {
        *problem = "dosya açılamadı";
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);
    if (fileSize < 0) {
        *problem = "dosya okunamadı";
        fclose(file);
        return NULL;
    }

    char *buffer = (char *)malloc((size_t)fileSize + 1);
    if (buffer == NULL) {
        *problem = "bellek yetersiz";
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(buffer, sizeof(char), (size_t)fileSize, file);
    fclose(file);
    if (bytesRead < (size_t)fileSize) {
        *problem = "dosya okunamadı";
        free(buffer);
        return NULL;
    }
    buffer[bytesRead] = '\0';

    const unsigned char *bytes = (const unsigned char *)buffer;
    if (bytesRead >= 2 &&
        ((bytes[0] == 0xFF && bytes[1] == 0xFE) || (bytes[0] == 0xFE && bytes[1] == 0xFF))) {
        *problem = "dosya UTF-16 olarak kaydedilmiş; kaynak dosyalar UTF-8 olmalıdır";
        free(buffer);
        return NULL;
    }
    if (memchr(buffer, '\0', bytesRead) != NULL) {
        *problem = "dosya bir metin dosyası değil; kaynak dosyalar UTF-8 olmalıdır";
        free(buffer);
        return NULL;
    }

    /* UTF-8 BOM varsa atla. */
    if (bytesRead >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF) {
        memmove(buffer, buffer + 3, bytesRead - 3 + 1);
    }

    return buffer;
}
