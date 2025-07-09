#ifndef TURKIYE_FEATURES_H
#define TURKIYE_FEATURES_H

/**
 * Türkiye'ye Özel Özellikler
 * Turkey-Specific Features for JUS Programming Language
 */

/**
 * Türkiye şehir kontrolü
 * @param sehir Şehir adı
 * @return 1 eğer geçerli Türkiye şehri, 0 değilse
 */
int turkiye_sehir_mi(const char* sehir);

/**
 * Türkiye başkenti kontrolü
 * @param sehir Şehir adı
 * @return 1 eğer Türkiye'nin başkenti, 0 değilse
 */
int turkiye_baskent_mi(const char* sehir);

/**
 * TC Kimlik No doğrulama
 * @param tc_str 11 haneli TC kimlik numarası string
 * @return 1 eğer geçerli, 0 değilse
 */
int tc_kimlik_gecerli(const char* tc_str);

/**
 * Türkiye telefon numarası doğrulama
 * @param telefon Telefon numarası string
 * @return 1 eğer geçerli, 0 değilse
 */
int turkiye_telefon_gecerli(const char* telefon);

/**
 * Türk Lirası formatı
 * @param miktar Para miktarı
 * @return Formatlanmış TL string
 */
char* tl_formatla(double miktar);

/**
 * Türkiye saati
 * @return Türkiye saati string
 */
char* turkiye_saati();

/**
 * Türkiye'deki bayramlar
 * @param gun Gün (1-31)
 * @param ay Ay (1-12)
 * @return Bayram adı veya NULL
 */
const char* turkiye_bayram_kontrol(int gun, int ay);

/**
 * Türkiye coğrafi bilgi
 * @return Türkiye hakkında coğrafi bilgiler
 */
char* turkiye_cografya_bilgi();

/**
 * Türkiye matematik terimi çevirisi
 * @param terim Türkçe matematik terimi
 * @return İngilizce karşılığı
 */
const char* matematik_terim_cevir(const char* terim);

#endif /* TURKIYE_FEATURES_H */