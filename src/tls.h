#ifndef JUS_TLS_H
#define JUS_TLS_H

#include <stddef.h>
#include <stdint.h>

#include "common.h"

/*
 * Şifreli bağlantı (TLS) katmanı. Var olan bir TCP soketinin üzerinde çalışır;
 * platformun kendi altyapısını kullanır:
 *   Windows: SChannel; macOS: Secure Transport (TLS 1.2);
 *   Linux: çalışma zamanında yüklenen sistem OpenSSL'i (libssl 3 ya da 1.1).
 * Sunucu sertifikası işletim sisteminin kök sertifika deposuyla doğrulanır ve
 * sunucu adıyla eşleşmesi istenir.
 */

typedef struct Tls Tls;

/* Bu derlemede ve bu bilgisayarda TLS kullanılabilir mi? Değilse *reason nedenini anlatır. */
bool tlsAvailable(const char **reason);

/*
 * Soket üzerinde istemci olarak el sıkışır. host, sertifika doğrulaması ve
 * sunucu adı bildirimi (SNI) için kullanılır. Başarısızsa NULL döner ve error'a
 * Türkçe bir açıklama yazılır.
 */
Tls *tlsConnect(intptr_t socket, const char *host, char *error, size_t errorSize);

/* Tüm veriyi gönderir; başarısızsa false. */
bool tlsSend(Tls *tls, const char *data, int length);

/* En çok capacity bayt okur: >0 okunan sayı, 0 karşı taraf kapattı, -1 hata ya da zaman aşımı. */
int tlsRecv(Tls *tls, char *buffer, int capacity);

/* Bağlantıyı kapatma bildirimi gönderir ve kaynakları bırakır; soketi kapatmaz. */
void tlsClose(Tls *tls);

#endif
