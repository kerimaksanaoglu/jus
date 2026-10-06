#ifndef JUS_IO_H
#define JUS_IO_H

/*
 * Bir kaynak dosyasını okur. Başarılıysa malloc ile ayrılmış, sonu '\0' ile
 * biten metni döndürür (UTF-8 BOM atılmış olarak). Başarısızsa NULL döndürür ve
 * *problem'e nedeni anlatan sabit bir metin yazar.
 */
char *readSource(const char *path, const char **problem);

#endif
