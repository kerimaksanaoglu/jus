#ifndef JUS_IO_H
#define JUS_IO_H

#include <stdio.h>

#include "common.h"

/*
 * Dosya sistemi yardımcıları. Tüm yollar UTF-8'dir; Windows'ta geniş karakterli
 * sistem çağrılarına çevrilir, böylece Türkçe karakterli yollar çalışır.
 */

/*
 * Bir kaynak dosyasını okur. Başarılıysa malloc ile ayrılmış, sonu '\0' ile
 * biten metni döndürür (UTF-8 BOM atılmış olarak). Başarısızsa NULL döndürür ve
 * *problem'e nedeni anlatan sabit bir metin yazar.
 */
char *readSource(const char *path, const char **problem);

/* Dosyayı olduğu gibi okur; malloc ile ayrılmış veri döner, *size bayt sayısıdır. Hata: NULL ve *problem. */
unsigned char *readBinaryFile(const char *path, size_t *size, const char **problem);
FILE *openFile(const char *path, const char *mode);
bool removeFile(const char *path);
bool makeDirectory(const char *path);
/* Yalnızca boş klasörleri siler. */
bool removeDirectory(const char *path);
/* Klasördeki her girdi için each çağrılır ("." ve ".." hariç). Klasör açılamazsa false. */
bool listDirectory(const char *path, void (*each)(const char *name, void *context), void *context);

#ifdef _WIN32
/* Programın argümanlarını UTF-8 olarak döndürür; dizi ve metinler süreç boyunca yaşar. */
char **utf8Arguments(int *count);
#endif

#endif
