// Türkiye'ye Özel Özellikler - Turkey-Specific Features
// JUS Programlama Dili için

#include "turkiye_features.h"
#include "../include/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Türkiye şehirleri listesi
static const char* TURKIYE_SEHIRLER[] = {
    "Adana", "Adıyaman", "Afyonkarahisar", "Ağrı", "Amasya", "Ankara", "Antalya", "Artvin",
    "Aydın", "Balıkesir", "Bilecik", "Bingöl", "Bitlis", "Bolu", "Burdur", "Bursa", "Çanakkale",
    "Çankırı", "Çorum", "Denizli", "Diyarbakır", "Edirne", "Elazığ", "Erzincan", "Erzurum",
    "Eskişehir", "Gaziantep", "Giresun", "Gümüşhane", "Hakkâri", "Hatay", "Isparta", "Mersin",
    "İstanbul", "İzmir", "Kars", "Kastamonu", "Kayseri", "Kırklareli", "Kırşehir", "Kocaeli",
    "Konya", "Kütahya", "Malatya", "Manisa", "Kahramanmaraş", "Mardin", "Muğla", "Muş", "Nevşehir",
    "Niğde", "Ordu", "Rize", "Sakarya", "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdağ", "Tokat",
    "Trabzon", "Tunceli", "Şanlıurfa", "Uşak", "Van", "Yozgat", "Zonguldak", "Aksaray", "Bayburt",
    "Karaman", "Kırıkkale", "Batman", "Şırnak", "Bartın", "Ardahan", "Iğdır", "Yalova", "Karabük",
    "Kilis", "Osmaniye", "Düzce"
};

static const int TURKIYE_SEHIR_SAYISI = sizeof(TURKIYE_SEHIRLER) / sizeof(TURKIYE_SEHIRLER[0]);

// Türkiye'nin başkenti
static const char* TURKIYE_BASKENT = "Ankara";

// Türk Lirası formatı
static const char* TL_SYMBOL = "₺";

/**
 * Türkiye şehir kontrolü
 */
int turkiye_sehir_mi(const char* sehir) {
    if (!sehir) return 0;
    
    for (int i = 0; i < TURKIYE_SEHIR_SAYISI; i++) {
        if (strcmp(sehir, TURKIYE_SEHIRLER[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Türkiye başkenti kontrolü
 */
int turkiye_baskent_mi(const char* sehir) {
    if (!sehir) return 0;
    return strcmp(sehir, TURKIYE_BASKENT) == 0;
}

/**
 * TC Kimlik No doğrulama algoritması
 */
int tc_kimlik_gecerli(const char* tc_str) {
    if (!tc_str || strlen(tc_str) != 11) return 0;
    
    // Sadece rakam kontrolü
    for (int i = 0; i < 11; i++) {
        if (tc_str[i] < '0' || tc_str[i] > '9') {
            return 0;
        }
    }
    
    // İlk rakam 0 olamaz
    if (tc_str[0] == '0') return 0;
    
    // TC Kimlik No algoritması
    long long tc = atoll(tc_str);
    int digits[11];
    
    // Rakamları diziye çevir
    for (int i = 10; i >= 0; i--) {
        digits[i] = tc % 10;
        tc /= 10;
    }
    
    // 10. rakam hesaplama
    int sum_odd = digits[0] + digits[2] + digits[4] + digits[6] + digits[8];
    int sum_even = digits[1] + digits[3] + digits[5] + digits[7];
    int check_10 = ((sum_odd * 7) - sum_even) % 10;
    
    if (check_10 != digits[9]) return 0;
    
    // 11. rakam hesaplama
    int sum_all = 0;
    for (int i = 0; i < 10; i++) {
        sum_all += digits[i];
    }
    int check_11 = sum_all % 10;
    
    if (check_11 != digits[10]) return 0;
    
    return 1;
}

/**
 * Türkiye telefon numarası doğrulama
 */
int turkiye_telefon_gecerli(const char* telefon) {
    if (!telefon) return 0;
    
    // Boşlukları ve özel karakterleri temizle
    char temiz[20] = {0};
    int j = 0;
    
    for (int i = 0; telefon[i] && j < 19; i++) {
        if (telefon[i] >= '0' && telefon[i] <= '9') {
            temiz[j++] = telefon[i];
        }
    }
    
    // 11 haneli olmalı ve 0 ile başlamalı
    if (j != 11 || temiz[0] != '0') return 0;
    
    // İkinci rakam 5 (cep) veya 2-4 (sabit) olmalı
    if (temiz[1] != '5' && temiz[1] != '2' && temiz[1] != '3' && temiz[1] != '4') {
        return 0;
    }
    
    return 1;
}

/**
 * Türk Lirası formatı
 */
char* tl_formatla(double miktar) {
    static char buffer[50];
    snprintf(buffer, sizeof(buffer), "%.2f %s", miktar, TL_SYMBOL);
    return buffer;
}

/**
 * Türkiye saati
 */
char* turkiye_saati() {
    static char buffer[100];
    time_t rawtime;
    struct tm * timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    // Türkiye saati UTC+3
    timeinfo->tm_hour = (timeinfo->tm_hour + 3) % 24;
    
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", timeinfo);
    return buffer;
}

/**
 * Türkiye'deki bayramlar
 */
const char* turkiye_bayram_kontrol(int gun, int ay) {
    // Sabit bayramlar
    if (ay == 1 && gun == 1) return "Yılbaşı";
    if (ay == 4 && gun == 23) return "Ulusal Egemenlik ve Çocuk Bayramı";
    if (ay == 5 && gun == 1) return "Emek ve Dayanışma Günü";
    if (ay == 5 && gun == 19) return "Atatürk'ü Anma, Gençlik ve Spor Bayramı";
    if (ay == 7 && gun == 15) return "Demokrasi ve Milli Birlik Günü";
    if (ay == 8 && gun == 30) return "Zafer Bayramı";
    if (ay == 10 && gun == 29) return "Cumhuriyet Bayramı";
    
    // Dini bayramlar dinamik olarak hesaplanmalı (bu basit bir örnek)
    return NULL;
}

/**
 * Türkiye coğrafi bilgi
 */
char* turkiye_cografya_bilgi() {
    static char buffer[500];
    snprintf(buffer, sizeof(buffer), 
        "Türkiye Cumhuriyeti:\n"
        "Başkent: %s\n"
        "Yüzölçümü: 783.356 km²\n"
        "Nüfus: ~84 milyon\n"
        "İl Sayısı: 81\n"
        "Kıtalar: Avrupa ve Asya\n"
        "Para Birimi: Türk Lirası (%s)\n"
        "Saat Dilimi: UTC+3",
        TURKIYE_BASKENT, TL_SYMBOL);
    
    return buffer;
}

/**
 * Türkiye matematik terimi çevirisi
 */
const char* matematik_terim_cevir(const char* terim) {
    if (strcmp(terim, "toplam") == 0) return "sum";
    if (strcmp(terim, "fark") == 0) return "difference";
    if (strcmp(terim, "çarpım") == 0) return "product";
    if (strcmp(terim, "bölüm") == 0) return "quotient";
    if (strcmp(terim, "kalan") == 0) return "remainder";
    if (strcmp(terim, "kök") == 0) return "root";
    if (strcmp(terim, "kare") == 0) return "square";
    if (strcmp(terim, "küp") == 0) return "cube";
    
    return "bilinmeyen";
}