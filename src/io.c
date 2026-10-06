#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

char *readSource(const char *path, const char **problem) {
    FILE *file = fopen(path, "rb");
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
