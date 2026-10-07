#ifndef JUS_FORMAT_H
#define JUS_FORMAT_H

/*
 * JUS kaynağını standart biçime getirir. Başarılıysa malloc ile ayrılmış,
 * '\0' ile biten yeni metni döndürür (çağıran free eder). Kaynakta sözcük
 * düzeyinde hata varsa (kapanmamış metin, geçersiz karakter, tutarsız girinti)
 * NULL döndürür ve *problem'e sabit bir Türkçe açıklama, *line'a satır numarası yazar.
 * problem ve line NULL verilebilir.
 */
char *formatSource(const char *source, const char **problem, int *line);

#endif
