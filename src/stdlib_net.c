#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#else
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#endif

#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET Socket;
#define closeSocket closesocket
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
typedef int Socket;
#define INVALID_SOCKET (-1)
#define closeSocket close
#endif

#include "object.h"
#include "stdlib_modules.h"
#include "value.h"
#include "vm.h"

/*
 * ağ modülü: TCP bağlantıları. Bağlantılar ve dinleyiciler programda birer
 * sayı (tutamaç) ile temsil edilir. Okunan verinin fazlası bağlantının
 * arabelleğinde bekler.
 */

#define MAX_CONNECTIONS 256
#define RECEIVE_CHUNK 4096

typedef struct {
    bool used;
    bool listener;
    Socket socket;
    char *buffer;
    int length;
    int capacity;
} Connection;

static Connection connections[MAX_CONNECTIONS];

static bool startNetwork(void) {
#ifdef _WIN32
    static bool started = false;
    if (!started) {
        WSADATA data;
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return false;
        started = true;
    }
#else
    /* Karşı tarafın kapattığı bağlantıya yazmak programı sonlandırmasın; hata dönsün. */
    signal(SIGPIPE, SIG_IGN);
#endif
    return true;
}

static int addConnection(Socket socket, bool listener) {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i].used) continue;
        connections[i].used = true;
        connections[i].listener = listener;
        connections[i].socket = socket;
        connections[i].buffer = NULL;
        connections[i].length = 0;
        connections[i].capacity = 0;
        return i + 1;
    }
    return 0;
}

/* Tutamaçtan bağlantıyı bulur; listener, beklenen türü belirtir. */
static Connection *findConnection(const char *name, Value handle, bool listener) {
    Connection *connection = NULL;
    if (IS_NUMBER(handle)) {
        double number = AS_NUMBER(handle);
        if (number == floor(number) && number >= 1 && number <= MAX_CONNECTIONS) {
            connection = &connections[(int)number - 1];
        }
    }
    if (connection == NULL || !connection->used) {
        nativeFail("'%s' için geçerli, açık bir %s gerekli.", name, listener ? "dinleyici" : "bağlantı");
        return NULL;
    }
    if (connection->listener != listener) {
        nativeFail("'%s' bir %s ister; %s verildi.", name, listener ? "dinleyici" : "bağlantı",
                   listener ? "bağlantı" : "dinleyici");
        return NULL;
    }
    return connection;
}

static bool toPort(const char *name, Value value, int *port) {
    if (!IS_NUMBER(value) || AS_NUMBER(value) != floor(AS_NUMBER(value)) || AS_NUMBER(value) < 0 ||
        AS_NUMBER(value) > 65535) {
        return nativeFail("'%s' için port 0 ile 65535 arasında bir tam sayı olmalı.", name);
    }
    *port = (int)AS_NUMBER(value);
    return true;
}

static void prepareSocket(Socket socket) {
#ifdef SO_NOSIGPIPE
    int on = 1;
    setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, (const char *)&on, sizeof(on));
#else
    (void)socket;
#endif
}

/* ağ.bağlan(sunucu, port): TCP bağlantısı açar. */
static bool baglanNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'ağ.bağlan' için sunucu adı metin olmalı; %s verildi.",
                          valueTypeName(args[0]));
    }
    int port = 0;
    if (!toPort("ağ.bağlan", args[1], &port)) return false;
    if (!startNetwork()) return nativeFail("Ağ başlatılamadı.");

    char service[16];
    snprintf(service, sizeof(service), "%d", port);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *addresses = NULL;
    if (getaddrinfo(AS_CSTRING(args[0]), service, &hints, &addresses) != 0) {
        return nativeFail("'%.200s' adresi çözülemedi.", AS_CSTRING(args[0]));
    }

    Socket connected = INVALID_SOCKET;
    for (struct addrinfo *address = addresses; address != NULL; address = address->ai_next) {
        Socket candidate = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
        if (candidate == INVALID_SOCKET) continue;
        if (connect(candidate, address->ai_addr, (int)address->ai_addrlen) == 0) {
            connected = candidate;
            break;
        }
        closeSocket(candidate);
    }
    freeaddrinfo(addresses);

    if (connected == INVALID_SOCKET) {
        return nativeFail("'%.200s' sunucusunun %d numaralı portuna bağlanılamadı.",
                          AS_CSTRING(args[0]), port);
    }
    prepareSocket(connected);

    int handle = addConnection(connected, false);
    if (handle == 0) {
        closeSocket(connected);
        return nativeFail("Aynı anda en çok %d bağlantı açık olabilir.", MAX_CONNECTIONS);
    }
    *result = NUMBER_VAL(handle);
    return true;
}

/* ağ.dinle(port) / ağ.dinle(port, adres): gelen bağlantıları bekleyen dinleyici açar. */
static bool dinleNative(int argCount, Value *args, Value *result) {
    if (argCount < 1 || argCount > 2) {
        return nativeFail("'ağ.dinle' 1 ya da 2 argüman alır, %d verildi.", argCount);
    }
    int port = 0;
    if (!toPort("ağ.dinle", args[0], &port)) return false;
    const char *host = "127.0.0.1";
    if (argCount == 2) {
        if (!IS_STRING(args[1])) {
            return nativeFail("'ağ.dinle' için adres metin olmalı; %s verildi.",
                              valueTypeName(args[1]));
        }
        host = AS_CSTRING(args[1]);
    }
    if (!startNetwork()) return nativeFail("Ağ başlatılamadı.");

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons((unsigned short)port);
    if (inet_pton(AF_INET, host, &address.sin_addr) != 1) {
        return nativeFail("'%.100s' geçerli bir IPv4 adresi değil.", host);
    }

    Socket listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == INVALID_SOCKET) return nativeFail("Dinleyici oluşturulamadı.");
    int on = 1;
#ifdef _WIN32
    /* Windows'ta SO_REUSEADDR, kullanımdaki bir portun ikinci kez dinlenmesine izin verir. */
    setsockopt(listener, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (const char *)&on, sizeof(on));
#else
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on));
#endif

    if (bind(listener, (struct sockaddr *)&address, sizeof(address)) != 0 ||
        listen(listener, 16) != 0) {
        closeSocket(listener);
        return nativeFail("%d numaralı port dinlenemiyor; başka bir program kullanıyor olabilir.", port);
    }

    int handle = addConnection(listener, true);
    if (handle == 0) {
        closeSocket(listener);
        return nativeFail("Aynı anda en çok %d bağlantı açık olabilir.", MAX_CONNECTIONS);
    }
    *result = NUMBER_VAL(handle);
    return true;
}

/* ağ.port(dinleyici): dinleyicinin kullandığı port (0 ile açıldıysa sistemin seçtiği). */
static bool portNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    Connection *listener = findConnection("ağ.port", args[0], true);
    if (listener == NULL) return false;

    struct sockaddr_in address;
    socklen_t length = sizeof(address);
    if (getsockname(listener->socket, (struct sockaddr *)&address, &length) != 0) {
        return nativeFail("Dinleyicinin portu okunamadı.");
    }
    *result = NUMBER_VAL(ntohs(address.sin_port));
    return true;
}

/* ağ.kabul_et(dinleyici): bir bağlantı gelene kadar bekler ve onu döndürür. */
static bool kabulEtNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    Connection *listener = findConnection("ağ.kabul_et", args[0], true);
    if (listener == NULL) return false;

    fflush(stdout);
    Socket accepted = accept(listener->socket, NULL, NULL);
    if (accepted == INVALID_SOCKET) return nativeFail("Gelen bağlantı kabul edilemedi.");
    prepareSocket(accepted);

    int handle = addConnection(accepted, false);
    if (handle == 0) {
        closeSocket(accepted);
        return nativeFail("Aynı anda en çok %d bağlantı açık olabilir.", MAX_CONNECTIONS);
    }
    *result = NUMBER_VAL(handle);
    return true;
}

/* ağ.gönder(bağlantı, metin): metnin tamamını gönderir. */
static bool gonderNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    Connection *connection = findConnection("ağ.gönder", args[0], false);
    if (connection == NULL) return false;
    if (!IS_STRING(args[1])) {
        return nativeFail("'ağ.gönder' metin gönderir; %s verildi.", valueTypeName(args[1]));
    }

    ObjString *data = AS_STRING(args[1]);
    int sent = 0;
    while (sent < data->length) {
#ifdef MSG_NOSIGNAL
        int flags = MSG_NOSIGNAL;
#else
        int flags = 0;
#endif
        int count = (int)send(connection->socket, data->chars + sent, data->length - sent, flags);
        if (count <= 0) return nativeFail("Veri gönderilemedi; bağlantı kopmuş olabilir.");
        sent += count;
    }
    return true;
}

/* Bağlantıdan bir parça daha okuyup arabelleğe ekler. 1: veri geldi, 0: bağlantı kapandı, -1: hata. */
static int receiveMore(Connection *connection) {
    if (connection->capacity < connection->length + RECEIVE_CHUNK) {
        int capacity = connection->capacity == 0 ? RECEIVE_CHUNK * 2 : connection->capacity * 2;
        char *buffer = (char *)realloc(connection->buffer, (size_t)capacity);
        if (buffer == NULL) {
            fprintf(stderr, "jus: bellek yetersiz.\n");
            exit(JUS_EXIT_OUT_OF_MEMORY);
        }
        connection->buffer = buffer;
        connection->capacity = capacity;
    }

    int count = (int)recv(connection->socket, connection->buffer + connection->length,
                          RECEIVE_CHUNK, 0);
    if (count < 0) return -1;
    if (count == 0) return 0;
    connection->length += count;
    return 1;
}

/* Arabelleğin ilk count baytını metin olarak alır ve arabellekten çıkarır. */
static Value takeBytes(Connection *connection, int count, int skip) {
    Value text = OBJ_VAL(copyString(connection->buffer, count));
    int consumed = count + skip;
    memmove(connection->buffer, connection->buffer + consumed,
            (size_t)(connection->length - consumed));
    connection->length -= consumed;
    return text;
}

static bool receiveFailed(void) {
    return nativeFail("Veri alınamadı; zaman aşımına uğradı ya da bağlantı koptu.");
}

/* ağ.al(bağlantı, en_çok): en çok verilen bayt kadar veri; bağlantı kapandıysa boş. */
static bool alNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    Connection *connection = findConnection("ağ.al", args[0], false);
    if (connection == NULL) return false;
    if (!IS_NUMBER(args[1]) || AS_NUMBER(args[1]) < 1 || AS_NUMBER(args[1]) > 1e9) {
        return nativeFail("'ağ.al' için bayt sayısı pozitif bir sayı olmalı.");
    }
    int limit = (int)AS_NUMBER(args[1]);

    if (connection->length == 0) {
        int status = receiveMore(connection);
        if (status < 0) return receiveFailed();
        if (status == 0) {
            *result = NIL_VAL;
            return true;
        }
    }
    *result = takeBytes(connection, connection->length < limit ? connection->length : limit, 0);
    return true;
}

/* ağ.tam_al(bağlantı, adet): tam olarak verilen bayt kadar veri (bağlantı erken kapanırsa daha az). */
static bool tamAlNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    Connection *connection = findConnection("ağ.tam_al", args[0], false);
    if (connection == NULL) return false;
    if (!IS_NUMBER(args[1]) || AS_NUMBER(args[1]) < 0 || AS_NUMBER(args[1]) > 1e9) {
        return nativeFail("'ağ.tam_al' için bayt sayısı negatif olmayan bir sayı olmalı.");
    }
    int wanted = (int)AS_NUMBER(args[1]);

    while (connection->length < wanted) {
        int status = receiveMore(connection);
        if (status < 0) return receiveFailed();
        if (status == 0) break;
    }
    *result = takeBytes(connection, connection->length < wanted ? connection->length : wanted, 0);
    return true;
}

/* ağ.satır_al(bağlantı): bir satır (satır sonu atılmış); veri bittiyse boş. */
static bool satirAlNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    Connection *connection = findConnection("ağ.satır_al", args[0], false);
    if (connection == NULL) return false;

    int searched = 0;
    for (;;) {
        char *newline = connection->length > searched
                            ? (char *)memchr(connection->buffer + searched, '\n',
                                             (size_t)(connection->length - searched))
                            : NULL;
        if (newline != NULL) {
            int length = (int)(newline - connection->buffer);
            int lineLength = length > 0 && connection->buffer[length - 1] == '\r' ? length - 1 : length;
            *result = takeBytes(connection, lineLength, length - lineLength + 1);
            return true;
        }
        searched = connection->length;

        int status = receiveMore(connection);
        if (status < 0) return receiveFailed();
        if (status == 0) {
            *result = connection->length == 0 ? NIL_VAL : takeBytes(connection, connection->length, 0);
            return true;
        }
    }
}

/* ağ.zaman_aşımı(bağlantı, saniye): veri beklerken geçebilecek en uzun süre; 0 sınırsız. */
static bool zamanAsimiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    Connection *connection = findConnection("ağ.zaman_aşımı", args[0], false);
    if (connection == NULL) return false;
    if (!IS_NUMBER(args[1]) || AS_NUMBER(args[1]) < 0) {
        return nativeFail("'ağ.zaman_aşımı' için süre negatif olmayan bir sayı olmalı.");
    }
    double seconds = AS_NUMBER(args[1]);
#ifdef _WIN32
    DWORD timeout = (DWORD)(seconds * 1000);
    setsockopt(connection->socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
#else
    struct timeval timeout;
    timeout.tv_sec = (time_t)seconds;
    timeout.tv_usec = (suseconds_t)((seconds - floor(seconds)) * 1e6);
    setsockopt(connection->socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
#endif
    return true;
}

/* ağ.kapat(tutamaç): bağlantıyı ya da dinleyiciyi kapatır. */
static bool kapatNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    (void)result;
    Connection *connection = NULL;
    if (IS_NUMBER(args[0]) && AS_NUMBER(args[0]) == floor(AS_NUMBER(args[0])) &&
        AS_NUMBER(args[0]) >= 1 && AS_NUMBER(args[0]) <= MAX_CONNECTIONS) {
        connection = &connections[(int)AS_NUMBER(args[0]) - 1];
    }
    if (connection == NULL || !connection->used) {
        return nativeFail("'ağ.kapat' için geçerli, açık bir bağlantı ya da dinleyici gerekli.");
    }

    closeSocket(connection->socket);
    free(connection->buffer);
    connection->buffer = NULL;
    connection->used = false;
    return true;
}

/* ağ.bayt_sayısı(metin): metnin UTF-8 olarak kapladığı bayt sayısı. */
static bool baytSayisiNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'ağ.bayt_sayısı' metin ister; %s verildi.", valueTypeName(args[0]));
    }
    *result = NUMBER_VAL(AS_STRING(args[0])->length);
    return true;
}

static int hexDigit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* ağ.url_çöz(metin): %XX dizilerini ve '+' işaretini çözer. */
static bool urlCozNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'ağ.url_çöz' metin ister; %s verildi.", valueTypeName(args[0]));
    }
    ObjString *text = AS_STRING(args[0]);
    char *buffer = (char *)malloc((size_t)text->length + 1);
    if (buffer == NULL) return nativeFail("Bellek yetersiz.");

    int length = 0;
    for (int i = 0; i < text->length; i++) {
        char c = text->chars[i];
        if (c == '+') {
            buffer[length++] = ' ';
        } else if (c == '%' && i + 2 < text->length && hexDigit(text->chars[i + 1]) >= 0 &&
                   hexDigit(text->chars[i + 2]) >= 0) {
            buffer[length++] = (char)(hexDigit(text->chars[i + 1]) * 16 + hexDigit(text->chars[i + 2]));
            i += 2;
        } else {
            buffer[length++] = c;
        }
    }
    *result = OBJ_VAL(copyString(buffer, length));
    free(buffer);
    return true;
}

/* ağ.url_kodla(metin): harf, rakam ve - _ . ~ dışındaki baytları %XX olarak yazar. */
static bool urlKodlaNative(int argCount, Value *args, Value *result) {
    (void)argCount;
    if (!IS_STRING(args[0])) {
        return nativeFail("'ağ.url_kodla' metin ister; %s verildi.", valueTypeName(args[0]));
    }
    ObjString *text = AS_STRING(args[0]);
    char *buffer = (char *)malloc((size_t)text->length * 3 + 1);
    if (buffer == NULL) return nativeFail("Bellek yetersiz.");

    int length = 0;
    for (int i = 0; i < text->length; i++) {
        unsigned char c = (unsigned char)text->chars[i];
        bool plain = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                     c == '-' || c == '_' || c == '.' || c == '~';
        if (plain) {
            buffer[length++] = (char)c;
        } else {
            length += snprintf(buffer + length, 4, "%%%02X", c);
        }
    }
    *result = OBJ_VAL(copyString(buffer, length));
    free(buffer);
    return true;
}

void defineNetworkModule(void) {
    ObjModule *net = defineModule("ağ");
    moduleDefineNative(net, "bağlan", 2, baglanNative);
    moduleDefineNative(net, "dinle", -1, dinleNative);
    moduleDefineNative(net, "port", 1, portNative);
    moduleDefineNative(net, "kabul_et", 1, kabulEtNative);
    moduleDefineNative(net, "gönder", 2, gonderNative);
    moduleDefineNative(net, "al", 2, alNative);
    moduleDefineNative(net, "tam_al", 2, tamAlNative);
    moduleDefineNative(net, "satır_al", 1, satirAlNative);
    moduleDefineNative(net, "zaman_aşımı", 2, zamanAsimiNative);
    moduleDefineNative(net, "kapat", 1, kapatNative);
    moduleDefineNative(net, "bayt_sayısı", 1, baytSayisiNative);
    moduleDefineNative(net, "url_çöz", 1, urlCozNative);
    moduleDefineNative(net, "url_kodla", 1, urlKodlaNative);
}
