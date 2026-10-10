#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tls.h"

/*
 * Üç arka uç da aynı dört işlemi sağlar: el sıkışma, gönderme, alma, kapatma.
 * Hepsi bloklayan soketler üzerinde çalışır; okuma zaman aşımı soketin
 * SO_RCVTIMEO ayarıyla gelir ve recv() başarısız olduğunda -1 döndürülür.
 */

static void setError(char *error, size_t size, const char *message) {
    if (error != NULL && size > 0) snprintf(error, size, "%s", message);
}

/* Üç arka uçta da aynı sözcüklerle bildirilen sertifika sorunları. */
#define MSG_EXPIRED "Sunucunun sertifikasının süresi dolmuş."
#define MSG_HOSTNAME "Sunucunun sertifikası verilen sunucu adına ait değil."
#define MSG_UNTRUSTED "Sunucunun sertifikası güvenilir bir kök sertifikaya dayanmıyor."
#define MSG_CERT "Sunucunun sertifikası doğrulanamadı."
#define MSG_HANDSHAKE "TLS el sıkışması başarısız oldu."

/* ======================================================================== */
#if defined(__EMSCRIPTEN__)

bool tlsAvailable(const char **reason) {
    *reason = "Tarayıcıda şifreli bağlantı açılamaz.";
    return false;
}

Tls *tlsConnect(intptr_t socket, const char *host, char *error, size_t errorSize) {
    (void)socket;
    (void)host;
    setError(error, errorSize, "Tarayıcıda şifreli bağlantı açılamaz.");
    return NULL;
}

bool tlsSend(Tls *tls, const char *data, int length) {
    (void)tls; (void)data; (void)length;
    return false;
}

int tlsRecv(Tls *tls, char *buffer, int capacity) {
    (void)tls; (void)buffer; (void)capacity;
    return -1;
}

void tlsClose(Tls *tls) {
    (void)tls;
}

/* ======================================================================== */
#elif defined(_WIN32)

#define SECURITY_WIN32
#include <winsock2.h>
#include <windows.h>
#include <schannel.h>
#include <security.h>
#include <sspi.h>

#define TLS_CHUNK 16384

struct Tls {
    SOCKET socket;
    CredHandle credentials;
    CtxtHandle context;
    SecPkgContext_StreamSizes sizes;
    /* Soketten okunmuş, henüz çözülmemiş veri. */
    char *encrypted;
    int encryptedLength;
    int encryptedCapacity;
    /* Çözülmüş ama çağırana henüz verilmemiş veri. */
    char *plain;
    int plainLength;
    int plainCapacity;
    bool closed;
};

bool tlsAvailable(const char **reason) {
    (void)reason;
    return true;
}

static bool growBuffer(char **buffer, int *capacity, int needed) {
    if (*capacity >= needed) return true;
    int size = *capacity == 0 ? TLS_CHUNK : *capacity;
    while (size < needed) size *= 2;
    char *grown = (char *)realloc(*buffer, (size_t)size);
    if (grown == NULL) return false;
    *buffer = grown;
    *capacity = size;
    return true;
}

static bool sendAll(SOCKET socket, const char *data, int length) {
    int sent = 0;
    while (sent < length) {
        int count = send(socket, data + sent, length - sent, 0);
        if (count <= 0) return false;
        sent += count;
    }
    return true;
}

/* Sokete okunan veriyi şifreli arabelleğe ekler. 1: veri, 0: kapandı, -1: hata. */
static int receiveEncrypted(Tls *tls) {
    if (!growBuffer(&tls->encrypted, &tls->encryptedCapacity, tls->encryptedLength + TLS_CHUNK)) return -1;
    int count = recv(tls->socket, tls->encrypted + tls->encryptedLength, TLS_CHUNK, 0);
    if (count < 0) return -1;
    if (count == 0) return 0;
    tls->encryptedLength += count;
    return 1;
}

static void dropEncrypted(Tls *tls, int consumed) {
    memmove(tls->encrypted, tls->encrypted + consumed, (size_t)(tls->encryptedLength - consumed));
    tls->encryptedLength -= consumed;
}

static const char *schannelMessage(SECURITY_STATUS status) {
    switch (status) {
        case SEC_E_WRONG_PRINCIPAL: return MSG_HOSTNAME;
        case SEC_E_UNTRUSTED_ROOT: return MSG_UNTRUSTED;
        case SEC_E_CERT_EXPIRED: return MSG_EXPIRED;
        case SEC_E_CERT_UNKNOWN: return MSG_CERT;
        case SEC_E_ILLEGAL_MESSAGE: return "Sunucu geçersiz bir TLS iletisi gönderdi.";
        case SEC_E_ALGORITHM_MISMATCH: return "Sunucuyla ortak bir şifreleme yöntemi bulunamadı.";
        case SEC_E_UNSUPPORTED_FUNCTION: return "Sunucunun TLS sürümü desteklenmiyor.";
        default: return MSG_HANDSHAKE;
    }
}

Tls *tlsConnect(intptr_t socket, const char *host, char *error, size_t errorSize) {
    Tls *tls = (Tls *)calloc(1, sizeof(Tls));
    if (tls == NULL) {
        setError(error, errorSize, "Bellek yetersiz.");
        return NULL;
    }
    tls->socket = (SOCKET)socket;

    SCHANNEL_CRED credentialData;
    memset(&credentialData, 0, sizeof(credentialData));
    credentialData.dwVersion = SCHANNEL_CRED_VERSION;
    /* Sertifika zinciri sistem deposuyla kendiliğinden doğrulanır; istemci sertifikası kullanılmaz. */
    credentialData.dwFlags = SCH_CRED_AUTO_CRED_VALIDATION | SCH_CRED_NO_DEFAULT_CREDS | SCH_USE_STRONG_CRYPTO;

    SECURITY_STATUS status = AcquireCredentialsHandleA(NULL, (SEC_CHAR *)UNISP_NAME_A, SECPKG_CRED_OUTBOUND, NULL,
                                                       &credentialData, NULL, NULL, &tls->credentials, NULL);
    if (status != SEC_E_OK) {
        setError(error, errorSize, "TLS kimlik bilgileri alınamadı.");
        free(tls);
        return NULL;
    }

    DWORD flags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY |
                  ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM | ISC_RET_EXTENDED_ERROR;
    DWORD outFlags = 0;
    bool haveContext = false;
    bool ok = false;

    for (;;) {
        SecBuffer inBuffers[2];
        SecBufferDesc inDesc;
        inBuffers[0].BufferType = SECBUFFER_TOKEN;
        inBuffers[0].pvBuffer = tls->encrypted;
        inBuffers[0].cbBuffer = (unsigned long)tls->encryptedLength;
        inBuffers[1].BufferType = SECBUFFER_EMPTY;
        inBuffers[1].pvBuffer = NULL;
        inBuffers[1].cbBuffer = 0;
        inDesc.ulVersion = SECBUFFER_VERSION;
        inDesc.cBuffers = 2;
        inDesc.pBuffers = inBuffers;

        SecBuffer outBuffer;
        SecBufferDesc outDesc;
        outBuffer.BufferType = SECBUFFER_TOKEN;
        outBuffer.pvBuffer = NULL;
        outBuffer.cbBuffer = 0;
        outDesc.ulVersion = SECBUFFER_VERSION;
        outDesc.cBuffers = 1;
        outDesc.pBuffers = &outBuffer;

        status = InitializeSecurityContextA(&tls->credentials, haveContext ? &tls->context : NULL,
                                            (SEC_CHAR *)host, flags, 0, 0, haveContext ? &inDesc : NULL, 0,
                                            haveContext ? NULL : &tls->context, &outDesc, &outFlags, NULL);
        haveContext = true;

        if (outBuffer.pvBuffer != NULL && outBuffer.cbBuffer > 0) {
            bool sent = sendAll(tls->socket, (const char *)outBuffer.pvBuffer, (int)outBuffer.cbBuffer);
            FreeContextBuffer(outBuffer.pvBuffer);
            if (!sent) {
                setError(error, errorSize, "TLS el sıkışması sırasında veri gönderilemedi.");
                break;
            }
        }

        if (status == SEC_E_INCOMPLETE_MESSAGE) {
            int received = receiveEncrypted(tls);
            if (received <= 0) {
                setError(error, errorSize, "Sunucu TLS el sıkışması sırasında bağlantıyı kapattı.");
                break;
            }
            continue;
        }

        /* Kullanılan girdi atılır; SECBUFFER_EXTRA artan veridir (uygulama verisi olabilir). */
        if (inBuffers[1].BufferType == SECBUFFER_EXTRA && inBuffers[1].cbBuffer > 0) {
            dropEncrypted(tls, tls->encryptedLength - (int)inBuffers[1].cbBuffer);
        } else {
            tls->encryptedLength = 0;
        }

        if (status == SEC_E_OK) {
            ok = true;
            break;
        }
        if (status == SEC_I_CONTINUE_NEEDED) {
            if (tls->encryptedLength == 0) {
                int received = receiveEncrypted(tls);
                if (received <= 0) {
                    setError(error, errorSize, "Sunucu TLS el sıkışması sırasında bağlantıyı kapattı.");
                    break;
                }
            }
            continue;
        }
        setError(error, errorSize, schannelMessage(status));
        break;
    }

    if (ok && QueryContextAttributesA(&tls->context, SECPKG_ATTR_STREAM_SIZES, &tls->sizes) != SEC_E_OK) {
        setError(error, errorSize, "TLS akış boyutları alınamadı.");
        ok = false;
    }
    if (!ok) {
        if (haveContext) DeleteSecurityContext(&tls->context);
        FreeCredentialsHandle(&tls->credentials);
        free(tls->encrypted);
        free(tls);
        return NULL;
    }
    return tls;
}

bool tlsSend(Tls *tls, const char *data, int length) {
    int maximum = (int)tls->sizes.cbMaximumMessage;
    char *message = (char *)malloc(tls->sizes.cbHeader + (size_t)maximum + tls->sizes.cbTrailer);
    if (message == NULL) return false;

    int sent = 0;
    bool ok = true;
    while (ok && sent < length) {
        int chunk = length - sent < maximum ? length - sent : maximum;
        memcpy(message + tls->sizes.cbHeader, data + sent, (size_t)chunk);

        SecBuffer buffers[4];
        buffers[0].BufferType = SECBUFFER_STREAM_HEADER;
        buffers[0].pvBuffer = message;
        buffers[0].cbBuffer = tls->sizes.cbHeader;
        buffers[1].BufferType = SECBUFFER_DATA;
        buffers[1].pvBuffer = message + tls->sizes.cbHeader;
        buffers[1].cbBuffer = (unsigned long)chunk;
        buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
        buffers[2].pvBuffer = message + tls->sizes.cbHeader + chunk;
        buffers[2].cbBuffer = tls->sizes.cbTrailer;
        buffers[3].BufferType = SECBUFFER_EMPTY;
        buffers[3].pvBuffer = NULL;
        buffers[3].cbBuffer = 0;
        SecBufferDesc desc;
        desc.ulVersion = SECBUFFER_VERSION;
        desc.cBuffers = 4;
        desc.pBuffers = buffers;

        if (EncryptMessage(&tls->context, 0, &desc, 0) != SEC_E_OK) {
            ok = false;
            break;
        }
        int total = (int)(buffers[0].cbBuffer + buffers[1].cbBuffer + buffers[2].cbBuffer);
        ok = sendAll(tls->socket, message, total);
        sent += chunk;
    }
    free(message);
    return ok;
}

/* Çözülmüş veriyi düz arabelleğe ekler. */
static bool appendPlain(Tls *tls, const char *data, int length) {
    if (!growBuffer(&tls->plain, &tls->plainCapacity, tls->plainLength + length)) return false;
    memcpy(tls->plain + tls->plainLength, data, (size_t)length);
    tls->plainLength += length;
    return true;
}

/* Şifreli arabellekteki kayıtları çözer. 1: düz veri hazır, 0: kapandı, -1: hata, 2: daha fazla veri gerekli. */
static int decryptAvailable(Tls *tls) {
    while (tls->encryptedLength > 0) {
        SecBuffer buffers[4];
        buffers[0].BufferType = SECBUFFER_DATA;
        buffers[0].pvBuffer = tls->encrypted;
        buffers[0].cbBuffer = (unsigned long)tls->encryptedLength;
        for (int i = 1; i < 4; i++) {
            buffers[i].BufferType = SECBUFFER_EMPTY;
            buffers[i].pvBuffer = NULL;
            buffers[i].cbBuffer = 0;
        }
        SecBufferDesc desc;
        desc.ulVersion = SECBUFFER_VERSION;
        desc.cBuffers = 4;
        desc.pBuffers = buffers;

        SECURITY_STATUS status = DecryptMessage(&tls->context, &desc, 0, NULL);
        if (status == SEC_E_INCOMPLETE_MESSAGE) return 2;
        if (status == SEC_I_CONTEXT_EXPIRED) {
            tls->closed = true;
            return tls->plainLength > 0 ? 1 : 0;
        }
        if (status != SEC_E_OK && status != SEC_I_RENEGOTIATE) return -1;

        int extra = 0;
        for (int i = 0; i < 4; i++) {
            if (buffers[i].BufferType == SECBUFFER_DATA && buffers[i].cbBuffer > 0) {
                if (!appendPlain(tls, (const char *)buffers[i].pvBuffer, (int)buffers[i].cbBuffer)) return -1;
            } else if (buffers[i].BufferType == SECBUFFER_EXTRA) {
                extra = (int)buffers[i].cbBuffer;
            }
        }
        dropEncrypted(tls, tls->encryptedLength - extra);
        if (status == SEC_I_RENEGOTIATE) return -1; /* yeniden anlaşma desteklenmiyor */
        if (tls->plainLength > 0) return 1;
    }
    return tls->plainLength > 0 ? 1 : 2;
}

int tlsRecv(Tls *tls, char *buffer, int capacity) {
    while (tls->plainLength == 0) {
        if (tls->closed) return 0;
        int state = decryptAvailable(tls);
        if (state == 1) break;
        if (state == 0) return 0;
        if (state < 0) return -1;
        int received = receiveEncrypted(tls);
        if (received < 0) return -1;
        if (received == 0) {
            tls->closed = true;
            return 0;
        }
    }
    int count = tls->plainLength < capacity ? tls->plainLength : capacity;
    memcpy(buffer, tls->plain, (size_t)count);
    memmove(tls->plain, tls->plain + count, (size_t)(tls->plainLength - count));
    tls->plainLength -= count;
    return count;
}

void tlsClose(Tls *tls) {
    if (tls == NULL) return;
    DWORD type = SCHANNEL_SHUTDOWN;
    SecBuffer buffer;
    buffer.BufferType = SECBUFFER_TOKEN;
    buffer.pvBuffer = &type;
    buffer.cbBuffer = sizeof(type);
    SecBufferDesc desc;
    desc.ulVersion = SECBUFFER_VERSION;
    desc.cBuffers = 1;
    desc.pBuffers = &buffer;
    if (ApplyControlToken(&tls->context, &desc) == SEC_E_OK) {
        SecBuffer out;
        out.BufferType = SECBUFFER_TOKEN;
        out.pvBuffer = NULL;
        out.cbBuffer = 0;
        SecBufferDesc outDesc;
        outDesc.ulVersion = SECBUFFER_VERSION;
        outDesc.cBuffers = 1;
        outDesc.pBuffers = &out;
        DWORD flags = ISC_REQ_SEQUENCE_DETECT | ISC_REQ_REPLAY_DETECT | ISC_REQ_CONFIDENTIALITY |
                      ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_STREAM;
        DWORD outFlags = 0;
        SECURITY_STATUS status = InitializeSecurityContextA(&tls->credentials, &tls->context, NULL, flags, 0, 0,
                                                            NULL, 0, &tls->context, &outDesc, &outFlags, NULL);
        if ((status == SEC_E_OK || status == SEC_I_CONTEXT_EXPIRED) && out.pvBuffer != NULL) {
            sendAll(tls->socket, (const char *)out.pvBuffer, (int)out.cbBuffer);
        }
        if (out.pvBuffer != NULL) FreeContextBuffer(out.pvBuffer);
    }
    DeleteSecurityContext(&tls->context);
    FreeCredentialsHandle(&tls->credentials);
    free(tls->encrypted);
    free(tls->plain);
    free(tls);
}

/* ======================================================================== */
#elif defined(__APPLE__)

/*
 * Secure Transport macOS 10.15'ten bu yana kullanımdan kaldırılmış durumdadır
 * ancak güncel sürümlerde çalışır; TLS 1.2 ile konuşur. Yol haritasındaki karar
 * ve Network.framework'e geçiş koşulları için docs/yol-haritasi.md'ye bakın.
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <Security/SecureTransport.h>
#include <Security/Security.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

struct Tls {
    int socket;
    SSLContextRef context;
    bool closed;
};

bool tlsAvailable(const char **reason) {
    (void)reason;
    return true;
}

static OSStatus readCallback(SSLConnectionRef connection, void *data, size_t *length) {
    const Tls *tls = (const Tls *)connection;
    size_t wanted = *length;
    size_t done = 0;
    while (done < wanted) {
        ssize_t count = recv(tls->socket, (char *)data + done, wanted - done, 0);
        if (count > 0) {
            done += (size_t)count;
            continue;
        }
        *length = done;
        if (count == 0) return errSSLClosedGraceful;
        if (errno == EINTR) continue;
        /* Zaman aşımı ve diğer hatalar: bağlantı koptu sayılır. */
        return errSSLClosedAbort;
    }
    *length = done;
    return noErr;
}

static OSStatus writeCallback(SSLConnectionRef connection, const void *data, size_t *length) {
    const Tls *tls = (const Tls *)connection;
    size_t wanted = *length;
    size_t done = 0;
    while (done < wanted) {
        ssize_t count = send(tls->socket, (const char *)data + done, wanted - done, 0);
        if (count > 0) {
            done += (size_t)count;
            continue;
        }
        if (count < 0 && errno == EINTR) continue;
        *length = done;
        return errSSLClosedAbort;
    }
    *length = done;
    return noErr;
}

/*
 * Secure Transport sertifika sorunlarının çoğunu errSSLXCertChainInvalid ile
 * bildirir; nedeni öğrenmek için sunucunun güven nesnesi yeniden değerlendirilir.
 */
static const char *certificateReason(SSLContextRef context) {
    SecTrustRef trust = NULL;
    if (SSLCopyPeerTrust(context, &trust) != noErr || trust == NULL) return MSG_CERT;
    CFErrorRef failure = NULL;
    const char *message = MSG_CERT;
    if (!SecTrustEvaluateWithError(trust, &failure) && failure != NULL) {
        switch (CFErrorGetCode(failure)) {
            case errSecCertificateExpired: message = MSG_EXPIRED; break;
            case errSecHostNameMismatch: message = MSG_HOSTNAME; break;
            case errSecNotTrusted:
            case errSecCreateChainFailed:
            case errSecVerifyActionFailed: message = MSG_UNTRUSTED; break;
            default: break;
        }
        CFRelease(failure);
    }
    CFRelease(trust);
    return message;
}

static const char *secureTransportMessage(SSLContextRef context, OSStatus status) {
    switch (status) {
        case errSSLXCertChainInvalid: return certificateReason(context);
        case errSSLBadCert: return MSG_CERT;
        case errSSLHostNameMismatch: return MSG_HOSTNAME;
        case errSSLCertExpired: return MSG_EXPIRED;
        case errSSLUnknownRootCert:
        case errSSLNoRootCert: return MSG_UNTRUSTED;
        case errSSLNegotiation: return "Sunucuyla ortak bir şifreleme yöntemi bulunamadı.";
        case errSSLClosedGraceful:
        case errSSLClosedAbort: return "Sunucu TLS el sıkışması sırasında bağlantıyı kapattı.";
        default: return MSG_HANDSHAKE;
    }
}

Tls *tlsConnect(intptr_t socket, const char *host, char *error, size_t errorSize) {
    Tls *tls = (Tls *)calloc(1, sizeof(Tls));
    if (tls == NULL) {
        setError(error, errorSize, "Bellek yetersiz.");
        return NULL;
    }
    tls->socket = (int)socket;
    tls->context = SSLCreateContext(NULL, kSSLClientSide, kSSLStreamType);
    if (tls->context == NULL) {
        setError(error, errorSize, "TLS bağlamı oluşturulamadı.");
        free(tls);
        return NULL;
    }

    OSStatus status = SSLSetIOFuncs(tls->context, readCallback, writeCallback);
    if (status == noErr) status = SSLSetConnection(tls->context, tls);
    if (status == noErr) status = SSLSetPeerDomainName(tls->context, host, strlen(host));
    if (status == noErr) status = SSLSetProtocolVersionMin(tls->context, kTLSProtocol12);
    while (status == noErr) {
        status = SSLHandshake(tls->context);
        if (status == errSSLWouldBlock) status = noErr; /* geri çağrı bloklar; yeniden dene */
        else break;
    }
    if (status != noErr) {
        setError(error, errorSize, secureTransportMessage(tls->context, status));
        CFRelease(tls->context);
        free(tls);
        return NULL;
    }
    return tls;
}

bool tlsSend(Tls *tls, const char *data, int length) {
    size_t sent = 0;
    while (sent < (size_t)length) {
        size_t processed = 0;
        OSStatus status = SSLWrite(tls->context, data + sent, (size_t)length - sent, &processed);
        sent += processed;
        if (status != noErr && status != errSSLWouldBlock) return false;
    }
    return true;
}

int tlsRecv(Tls *tls, char *buffer, int capacity) {
    if (tls->closed) return 0;
    size_t processed = 0;
    OSStatus status = SSLRead(tls->context, buffer, (size_t)capacity, &processed);
    if (processed > 0) return (int)processed;
    if (status == errSSLClosedGraceful || status == errSSLClosedNoNotify) {
        tls->closed = true;
        return 0;
    }
    return -1;
}

void tlsClose(Tls *tls) {
    if (tls == NULL) return;
    SSLClose(tls->context);
    CFRelease(tls->context);
    free(tls);
}
#pragma clang diagnostic pop

/* ======================================================================== */
#else

/*
 * Linux ve diğer POSIX sistemler: OpenSSL çalışma zamanında yüklenir; derleme
 * başlıklara ya da kütüphaneye bağımlı değildir. libssl.so.3 (OpenSSL 3) ya da
 * libssl.so.1.1 aranır. Kütüphane yoksa https kullanılamaz, http çalışır.
 */
#include <dlfcn.h>

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_method_st SSL_METHOD;

#define SSL_CTRL_SET_TLSEXT_HOSTNAME 55
#define TLSEXT_NAMETYPE_host_name 0
#define SSL_VERIFY_PEER 1
#define SSL_ERROR_ZERO_RETURN 6
#define X509_V_OK 0
#define X509_V_ERR_CERT_HAS_EXPIRED 10
#define X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT 18
#define X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN 19
#define X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY 20
#define X509_V_ERR_CERT_UNTRUSTED 27
#define X509_V_ERR_HOSTNAME_MISMATCH 62

typedef const SSL_METHOD *(*fn_TLS_client_method)(void);
typedef SSL_CTX *(*fn_SSL_CTX_new)(const SSL_METHOD *);
typedef void (*fn_SSL_CTX_free)(SSL_CTX *);
typedef int (*fn_SSL_CTX_set_default_verify_paths)(SSL_CTX *);
typedef void (*fn_SSL_CTX_set_verify)(SSL_CTX *, int, void *);
typedef SSL *(*fn_SSL_new)(SSL_CTX *);
typedef void (*fn_SSL_free)(SSL *);
typedef int (*fn_SSL_set_fd)(SSL *, int);
typedef long (*fn_SSL_ctrl)(SSL *, int, long, void *);
typedef int (*fn_SSL_set1_host)(SSL *, const char *);
typedef int (*fn_SSL_connect)(SSL *);
typedef int (*fn_SSL_read)(SSL *, void *, int);
typedef int (*fn_SSL_write)(SSL *, const void *, int);
typedef int (*fn_SSL_shutdown)(SSL *);
typedef int (*fn_SSL_get_error)(const SSL *, int);
typedef long (*fn_SSL_get_verify_result)(const SSL *);
typedef const char *(*fn_X509_verify_cert_error_string)(long);

static struct {
    bool tried;
    void *library;
    const char *failure;
    fn_TLS_client_method TLS_client_method;
    fn_SSL_CTX_new SSL_CTX_new;
    fn_SSL_CTX_free SSL_CTX_free;
    fn_SSL_CTX_set_default_verify_paths SSL_CTX_set_default_verify_paths;
    fn_SSL_CTX_set_verify SSL_CTX_set_verify;
    fn_SSL_new SSL_new;
    fn_SSL_free SSL_free;
    fn_SSL_set_fd SSL_set_fd;
    fn_SSL_ctrl SSL_ctrl;
    fn_SSL_set1_host SSL_set1_host;
    fn_SSL_connect SSL_connect;
    fn_SSL_read SSL_read;
    fn_SSL_write SSL_write;
    fn_SSL_shutdown SSL_shutdown;
    fn_SSL_get_error SSL_get_error;
    fn_SSL_get_verify_result SSL_get_verify_result;
    fn_X509_verify_cert_error_string X509_verify_cert_error_string;
} openssl;

/* dlsym nesne işaretçisi döndürür; işlev işaretçisine kopyalama ISO C'de dolaylı yapılır. */
static void loadSymbol(void *target, const char *name) {
    void *symbol = dlsym(openssl.library, name);
    memcpy(target, &symbol, sizeof(symbol));
}

#define LOAD(name) \
    do { \
        loadSymbol(&openssl.name, #name); \
        if (openssl.name == NULL) { \
            openssl.failure = "Sistemdeki OpenSSL kütüphanesi beklenen işlevleri içermiyor (sürüm 1.1 ya da 3 gerekir)."; \
            return false; \
        } \
    } while (false)

static bool loadOpenSsl(void) {
    if (openssl.tried) return openssl.failure == NULL;
    openssl.tried = true;

    const char *names[] = {"libssl.so.3", "libssl.so.1.1", "libssl.so"};
    for (size_t i = 0; i < sizeof(names) / sizeof(names[0]) && openssl.library == NULL; i++) {
        openssl.library = dlopen(names[i], RTLD_NOW | RTLD_GLOBAL);
    }
    if (openssl.library == NULL) {
        openssl.failure = "Şifreli bağlantı için sistemde OpenSSL (libssl.so.3 ya da libssl.so.1.1) gerekir; "
                          "paket yöneticinizle 'openssl' ya da 'libssl3' kurun.";
        return false;
    }
    LOAD(TLS_client_method);
    LOAD(SSL_CTX_new);
    LOAD(SSL_CTX_free);
    LOAD(SSL_CTX_set_default_verify_paths);
    LOAD(SSL_CTX_set_verify);
    LOAD(SSL_new);
    LOAD(SSL_free);
    LOAD(SSL_set_fd);
    LOAD(SSL_ctrl);
    LOAD(SSL_set1_host);
    LOAD(SSL_connect);
    LOAD(SSL_read);
    LOAD(SSL_write);
    LOAD(SSL_shutdown);
    LOAD(SSL_get_error);
    LOAD(SSL_get_verify_result);
    /* libcrypto içindedir; libssl onu yüklediği için RTLD_GLOBAL ile görülür. Yoksa genel ileti kullanılır. */
    loadSymbol(&openssl.X509_verify_cert_error_string, "X509_verify_cert_error_string");
    return true;
}

#undef LOAD

struct Tls {
    int socket;
    SSL_CTX *context;
    SSL *ssl;
    bool closed;
};

bool tlsAvailable(const char **reason) {
    if (loadOpenSsl()) return true;
    *reason = openssl.failure;
    return false;
}

Tls *tlsConnect(intptr_t socket, const char *host, char *error, size_t errorSize) {
    if (!loadOpenSsl()) {
        setError(error, errorSize, openssl.failure);
        return NULL;
    }
    Tls *tls = (Tls *)calloc(1, sizeof(Tls));
    if (tls == NULL) {
        setError(error, errorSize, "Bellek yetersiz.");
        return NULL;
    }
    tls->socket = (int)socket;
    tls->context = openssl.SSL_CTX_new(openssl.TLS_client_method());
    if (tls->context == NULL) {
        setError(error, errorSize, "TLS bağlamı oluşturulamadı.");
        free(tls);
        return NULL;
    }
    openssl.SSL_CTX_set_default_verify_paths(tls->context);
    openssl.SSL_CTX_set_verify(tls->context, SSL_VERIFY_PEER, NULL);

    tls->ssl = openssl.SSL_new(tls->context);
    if (tls->ssl == NULL || openssl.SSL_set_fd(tls->ssl, tls->socket) != 1 ||
        openssl.SSL_set1_host(tls->ssl, host) != 1) {
        setError(error, errorSize, "TLS bağlantısı hazırlanamadı.");
        tlsClose(tls);
        return NULL;
    }
    openssl.SSL_ctrl(tls->ssl, SSL_CTRL_SET_TLSEXT_HOSTNAME, TLSEXT_NAMETYPE_host_name, (void *)host);

    if (openssl.SSL_connect(tls->ssl) != 1) {
        long verify = openssl.SSL_get_verify_result(tls->ssl);
        if (verify == X509_V_ERR_CERT_HAS_EXPIRED) {
            setError(error, errorSize, MSG_EXPIRED);
        } else if (verify == X509_V_ERR_HOSTNAME_MISMATCH) {
            setError(error, errorSize, MSG_HOSTNAME);
        } else if (verify == X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT || verify == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN ||
                   verify == X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY || verify == X509_V_ERR_CERT_UNTRUSTED) {
            setError(error, errorSize, MSG_UNTRUSTED);
        } else if (verify != X509_V_OK) {
            const char *detail = openssl.X509_verify_cert_error_string != NULL
                                     ? openssl.X509_verify_cert_error_string(verify)
                                     : "";
            char message[256];
            snprintf(message, sizeof(message), "%s%s%s", MSG_CERT, detail[0] != '\0' ? " " : "", detail);
            setError(error, errorSize, message);
        } else {
            setError(error, errorSize, MSG_HANDSHAKE);
        }
        tlsClose(tls);
        return NULL;
    }
    return tls;
}

bool tlsSend(Tls *tls, const char *data, int length) {
    int sent = 0;
    while (sent < length) {
        int count = openssl.SSL_write(tls->ssl, data + sent, length - sent);
        if (count <= 0) return false;
        sent += count;
    }
    return true;
}

int tlsRecv(Tls *tls, char *buffer, int capacity) {
    if (tls->closed) return 0;
    int count = openssl.SSL_read(tls->ssl, buffer, capacity);
    if (count > 0) return count;
    if (openssl.SSL_get_error(tls->ssl, count) == SSL_ERROR_ZERO_RETURN) {
        tls->closed = true;
        return 0;
    }
    return -1;
}

void tlsClose(Tls *tls) {
    if (tls == NULL) return;
    if (tls->ssl != NULL) {
        if (!tls->closed) openssl.SSL_shutdown(tls->ssl);
        openssl.SSL_free(tls->ssl);
    }
    if (tls->context != NULL) openssl.SSL_CTX_free(tls->context);
    free(tls);
}

#endif
