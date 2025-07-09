#include "lexer.h"
#include "../include/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Lexer'ı başlat
 * @param lexer Lexer yapısı pointer'ı
 * @param source Kaynak kod string'i
 */
void lexer_init(Lexer *lexer, const char *source) {
    lexer->src = source;
    lexer->pos = 0;
    lexer->line = 1;
}

/**
 * Mevcut karakteri görüntüle
 * @param lexer Lexer yapısı pointer'ı
 * @return Mevcut karakter
 */
static char lexer_peek(Lexer *lexer) {
    if (lexer->pos >= strlen(lexer->src)) {
        return '\0';
    }
    return lexer->src[lexer->pos];
}

/**
 * Sonraki karaktere geç ve mevcut karakteri döndür
 * @param lexer Lexer yapısı pointer'ı
 * @return Geçilen karakter
 */
static char lexer_advance(Lexer *lexer) {
    char c = lexer_peek(lexer);
    lexer->pos++;
    if (c == '\n') {
        lexer->line++;
    }
    return c;
}

/**
 * Boşluk karakterlerini atla
 * @param lexer Lexer yapısı pointer'ı
 */
static void lexer_skip_whitespace(Lexer *lexer) {
    while (lexer_peek(lexer) != '\0') {
        char c = lexer_peek(lexer);
        if (c == ' ' || c == '\t' || c == '\r') {
            lexer_advance(lexer);
        } else {
            break;
        }
    }
}

/**
 * Token oluştur
 * @param lexer Lexer yapısı pointer'ı
 * @param type Token türü
 * @param start Başlangıç pozisyonu
 * @param length Token uzunluğu
 * @return Oluşturulan token
 */
static Token lexer_make_token(Lexer *lexer, TokenType type, int start, int length) {
    Token token;
    token.type = type;
    token.line = lexer->line;
    
    token.lexeme = malloc(length + 1);
    strncpy(token.lexeme, lexer->src + start, length);
    token.lexeme[length] = '\0';
    
    return token;
}

/**
 * Anahtar kelime kontrolü - TürkKod 2053 Zenginleştirilmiş Sözlük
 * @param lexeme Kontrol edilecek kelime
 * @return Token türü (KEYWORD veya IDENT)
 */
static TokenType lexer_check_keyword(const char *lexeme) {
    // Temel anahtar kelimeler (mevcut)
    if (strcmp(lexeme, "değişken") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "yaz") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "eğer") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "değilse") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "tekrar") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "fonksiyon") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "dönüş") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "doğru") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "yanlış") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "ve") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "veya") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "kez") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "döngü") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "iken") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "girdi") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "boşluk") == 0) return TOK_KEYWORD;
    
    // Kontrol Yapıları - Yeni
    if (strcmp(lexeme, "başlat") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "bitir") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "devam") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "kır") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "dene") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "yakala") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "fırlat") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "seç") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "durum") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "varsayılan") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "atla") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "git") == 0) return TOK_KEYWORD;
    
    // Veri Yapıları - Yeni
    if (strcmp(lexeme, "dizi") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "liste") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "sözlük") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "küme") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "yığın") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "sıra") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "nesne") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "sınıf") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "miras") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "arabirim") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "özellik") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "yöntem") == 0) return TOK_KEYWORD;
    
    // Döngü Yapıları - Yeni
    if (strcmp(lexeme, "her") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "kadar") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "için") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "tekrarla") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "ileri") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "geri") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "durdu") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "devam_et") == 0) return TOK_KEYWORD;
    
    // Modüler Programlama - Yeni
    if (strcmp(lexeme, "içeri_al") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "dışarı_ver") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "modül") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "paket") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "kütüphane") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "açık") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "kapalı") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "korumalı") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "özel") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "genel") == 0) return TOK_KEYWORD;
    
    // Mantık İfadeleri - Yeni
    if (strcmp(lexeme, "bu") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "o") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "şu") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "böyle") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "şöyle") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "öyle") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "hangi") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "nerede") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "nasıl") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "ne_zaman") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "neden") == 0) return TOK_KEYWORD;
    
    // Türkiye'ye özel built-in fonksiyonlar
    if (strcmp(lexeme, "turkiye_sehir_mi") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "tc_kimlik_gecerli") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "turkiye_telefon_gecerli") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "tl_formatla") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "turkiye_saati") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "turkiye_bayram_kontrol") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "turkiye_cografya_bilgi") == 0) return TOK_KEYWORD;
    if (strcmp(lexeme, "turkiye_baskent_mi") == 0) return TOK_KEYWORD;
    
    return TOK_IDENT;
}

/**
 * Identifier token'ını oku
 * @param lexer Lexer yapısı pointer'ı
 * @return Token
 */
static Token lexer_read_identifier(Lexer *lexer) {
    int start = lexer->pos;
    
    // Harf, rakam veya _ karakterlerini oku
    while (lexer_peek(lexer) != '\0') {
        char c = lexer_peek(lexer);
        // This is a simplistic check. A proper implementation would handle all UTF-8 characters.
        if (isalnum(c) || c == '_' || (unsigned char)c > 127) {
            lexer_advance(lexer);
        } else {
            break;
        }
    }
    
    int length = lexer->pos - start;
    Token token = lexer_make_token(lexer, TOK_IDENT, start, length);
    
    // Anahtar kelime kontrolü
    token.type = lexer_check_keyword(token.lexeme);
    
    return token;
}

/**
 * Sayı token'ını oku
 * @param lexer Lexer yapısı pointer'ı
 * @return Token
 */
static Token lexer_read_number(Lexer *lexer) {
    int start = lexer->pos;
    
    // Rakamları oku
    while (isdigit(lexer_peek(lexer))) {
        lexer_advance(lexer);
    }
    
    // Ondalık nokta kontrolü
    if (lexer_peek(lexer) == '.') {
        lexer_advance(lexer); // '.' karakterini atla
        
        // Ondalık kısmı oku
        while (isdigit(lexer_peek(lexer))) {
            lexer_advance(lexer);
        }
    }
    
    int length = lexer->pos - start;
    return lexer_make_token(lexer, TOK_NUMBER, start, length);
}

/**
 * String token'ını oku
 * @param lexer Lexer yapısı pointer'ı
 * @return Token
 */
static Token lexer_read_string(Lexer *lexer) {
    int start = lexer->pos;
    lexer_advance(lexer); // Açılış tırnağını atla
    
    while (lexer_peek(lexer) != '\0' && lexer_peek(lexer) != '"') {
        // TODO: implement escape sequences (\n, \t, \", \\, etc.)
        if (lexer_peek(lexer) == '\n') {
            lexer->line++;
        }
        lexer_advance(lexer);
    }
    
    if (lexer_peek(lexer) == '\0') {
        fprintf(stderr, "Hata (satır %d): Kapatılmamış string literal\n", lexer->line);
        exit(EXIT_FAILURE);
    }
    
    lexer_advance(lexer); // Kapanış tırnağını atla
    
    int length = lexer->pos - start;
    Token token = lexer_make_token(lexer, TOK_STRING, start, length);
    
    // Tırnak işaretlerini kaldır
    char *new_lexeme = malloc(length - 1);
    strncpy(new_lexeme, token.lexeme + 1, length - 2);
    new_lexeme[length - 2] = '\0';
    free(token.lexeme);
    token.lexeme = new_lexeme;
    
    return token;
}

/**
 * Yorum satırını atla
 * @param lexer Lexer yapısı pointer'ı
 */
static void lexer_skip_comment(Lexer *lexer) {
    // '//' karakterlerini atla
    lexer_advance(lexer); // ilk '/'
    lexer_advance(lexer); // ikinci '/'
    
    // Satır sonuna kadar oku
    while (lexer_peek(lexer) != '\0' && lexer_peek(lexer) != '\n') {
        lexer_advance(lexer);
    }
}

/**
 * Tek bir token tarar
 * @param lexer Lexer yapısı pointer'ı
 * @return Token
 */
static Token lexer_scan_token(Lexer *lexer) {
    lexer_skip_whitespace(lexer);
    
    char c = lexer_peek(lexer);
    
    // EOF kontrolü
    if (c == '\0') {
        Token token;
        token.type = TOK_EOF;
        token.lexeme = malloc(1);
        token.lexeme[0] = '\0';
        token.line = lexer->line;
        return token;
    }
    
    // Newline
    if (c == '\n') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_NEWLINE;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "\n");
        token.line = lexer->line - 1; // Çünkü advance'de line artmış
        return token;
    }
    
    // Yorum satırı
    if (c == '/' && lexer->pos + 1 < strlen(lexer->src) && lexer->src[lexer->pos + 1] == '/') {
        lexer_skip_comment(lexer);
        return lexer_scan_token(lexer); // Recursively get next token
    }
    
    // Colon
    if (c == ':') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_COLON;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, ":");
        token.line = lexer->line;
        return token;
    }

    // Assign and Equality
    if (c == '=') {
        lexer_advance(lexer);
        if (lexer_peek(lexer) == '=') {
            lexer_advance(lexer);
            Token token;
            token.type = TOK_EQ;
            token.lexeme = malloc(3);
            strcpy(token.lexeme, "==");
            token.line = lexer->line;
            return token;
        } else {
            Token token;
            token.type = TOK_ASSIGN;
            token.lexeme = malloc(2);
            strcpy(token.lexeme, "=");
            token.line = lexer->line;
            return token;
        }
    }

    // Operators
    if (c == '+') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_PLUS;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "+");
        token.line = lexer->line;
        return token;
    }
    if (c == '-') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_MINUS;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "-");
        token.line = lexer->line;
        return token;
    }
    if (c == '*') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_MULTIPLY;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "*");
        token.line = lexer->line;
        return token;
    }
    if (c == '/') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_DIVIDE;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "/");
        token.line = lexer->line;
        return token;
    }
    if (c == '%') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_MODULO;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "%");
        token.line = lexer->line;
        return token;
    }

    if (c == '>') {
        lexer_advance(lexer);
        if (lexer_peek(lexer) == '=') {
            lexer_advance(lexer);
            Token token;
            token.type = TOK_GE;
            token.lexeme = malloc(3);
            strcpy(token.lexeme, ">=");
            token.line = lexer->line;
            return token;
        } else {
            Token token;
            token.type = TOK_GT;
            token.lexeme = malloc(2);
            strcpy(token.lexeme, ">");
            token.line = lexer->line;
            return token;
        }
    }
    if (c == '<') {
        lexer_advance(lexer);
        if (lexer_peek(lexer) == '=') {
            lexer_advance(lexer);
            Token token;
            token.type = TOK_LE;
            token.lexeme = malloc(3);
            strcpy(token.lexeme, "<=");
            token.line = lexer->line;
            return token;
        } else {
            Token token;
            token.type = TOK_LT;
            token.lexeme = malloc(2);
            strcpy(token.lexeme, "<");
            token.line = lexer->line;
            return token;
        }
    }
    if (c == '!') {
        lexer_advance(lexer);
        if (lexer_peek(lexer) == '=') {
            lexer_advance(lexer);
            Token token;
            token.type = TOK_NE;
            token.lexeme = malloc(3);
            strcpy(token.lexeme, "!=");
            token.line = lexer->line;
            return token;
        } else {
            Token token;
            token.type = TOK_NOT;
            token.lexeme = malloc(2);
            strcpy(token.lexeme, "!");
            token.line = lexer->line;
            return token;
        }
    }
    
    // String literal
    if (c == '"') {
        return lexer_read_string(lexer);
    }
    
    // Number
    if (isdigit(c)) {
        return lexer_read_number(lexer);
    }
    
    // Identifier veya keyword (Türkçe karakter desteği)
    if (isalpha(c) || c == '_' || (unsigned char)c > 127) {
        return lexer_read_identifier(lexer);
    }
    
    // Parentheses
    if (c == '(') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_LPAREN;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "(");
        token.line = lexer->line;
        return token;
    }
    if (c == ')') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_RPAREN;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, ")");
        token.line = lexer->line;
        return token;
    }
    
    // Brackets - Köşeli parantezler
    if (c == '[') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_LBRACKET;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "[");
        token.line = lexer->line;
        return token;
    }
    if (c == ']') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_RBRACKET;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "]");
        token.line = lexer->line;
        return token;
    }
    
    // Braces - Süslü parantezler
    if (c == '{') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_LBRACE;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "{");
        token.line = lexer->line;
        return token;
    }
    if (c == '}') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_RBRACE;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, "}");
        token.line = lexer->line;
        return token;
    }
    
    // Comma and Semicolon
    if (c == ',') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_COMMA;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, ",");
        token.line = lexer->line;
        return token;
    }
    if (c == ';') {
        lexer_advance(lexer);
        Token token;
        token.type = TOK_SEMICOLON;
        token.lexeme = malloc(2);
        strcpy(token.lexeme, ";");
        token.line = lexer->line;
        return token;
    }
    
    // TODO: implement additional operators as needed
    
    // Geçersiz karakter
    fprintf(stderr, "Hata (satır %d): Geçersiz karakter '%c'\n", lexer->line, c);
    exit(EXIT_FAILURE);
}

/**
 * Kaynak kodun tamamını tokenize et
 * @param lexer Lexer yapısı pointer'ı
 * @param outCount Çıktı token sayısı
 * @return Token dizisi
 */
Token *lexer_scan_all(Lexer *lexer, int *outCount) {
    if (!lexer || !outCount) {
        return NULL;
    }
    
    int capacity = 128;
    Token *tokens = malloc(capacity * sizeof(Token));
    int count = 0;
    
    Token token;
    do {
        token = lexer_scan_token(lexer);
        
        // Kapasite kontrolü ve gerekirse genişlet
        if (count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
            if (!tokens) {
                fprintf(stderr, "Hata: Bellek tahsisi başarısız\n");
                exit(EXIT_FAILURE);
            }
        }
        
        tokens[count++] = token;
        
    } while (token.type != TOK_EOF);
    
    *outCount = count;
    return tokens;
}

/**
 * Token dizisini bellek temizliği için serbest bırak
 * @param tokens Token dizisi
 * @param count Token sayısı
 */
void token_free(Token *tokens, int count) {
    if (!tokens) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        if (tokens[i].lexeme) {
            free(tokens[i].lexeme);
        }
    }
    
    free(tokens);
}