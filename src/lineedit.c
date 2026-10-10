#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#ifdef __APPLE__
#define _DARWIN_C_SOURCE 1
#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lineedit.h"
#include "text.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#ifndef ENABLE_VIRTUAL_TERMINAL_INPUT
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0200
#endif
#else
#include <errno.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

/*
 * Etkileşimli kip için satır düzenleyici. Dosya iki katmandan oluşur:
 *  - uçbirim katmanı (termEnter, termLeave, termReadKey, termWrite, termColumns):
 *    her platform için ayrı, tuş vuruşlarını tek bir sayıya çevirir;
 *  - düzenleyici çekirdeği: tampon, imleç, geçmiş, tamamlama ve çizim.
 * Çizim tek satırlıdır (linenoise gibi): satır genişliği aşarsa görünen pencere
 * imleçle birlikte kayar. Her kod noktası bir sütun sayılır.
 */

#define HISTORY_MAX 1000
#define ESCAPE_WAIT_MS 50

/* Tuş kodları: 0..0x1FFFFF arası değerler kod noktasıdır (denetim karakterleri dahil),
 * bunun üstü özel tuşlardır. */
enum {
    LK_UP = 0x200000,
    LK_DOWN,
    LK_LEFT,
    LK_RIGHT,
    LK_HOME,
    LK_END,
    LK_DELETE,
    LK_IGNORE, /* anlamsız ya da tanınmayan tuş; yok sayılır */
    LK_EOF     /* girdi kapandı ya da okunamadı */
};

static bool terminalActive = false;

static void restoreAndExit(void);

#ifdef _WIN32

/* ---- Windows uçbirim katmanı ---- */

static HANDLE winIn = INVALID_HANDLE_VALUE;
static HANDLE winOut = INVALID_HANDLE_VALUE;
static DWORD winInMode = 0;
static DWORD winOutMode = 0;

static bool termEnter(void) {
    winIn = GetStdHandle(STD_INPUT_HANDLE);
    winOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (winIn == NULL || winIn == INVALID_HANDLE_VALUE) return false;
    if (winOut == NULL || winOut == INVALID_HANDLE_VALUE) return false;
    /* Yönlendirilmiş girdi/çıktıda GetConsoleMode başarısız olur: konsol değildir. */
    if (!GetConsoleMode(winIn, &winInMode)) return false;
    if (!GetConsoleMode(winOut, &winOutMode)) return false;

    DWORD out = winOutMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(winOut, out)) return false;

    /* Satır girişi, yankı ve Ctrl+C işleme kapalı: Ctrl+C karakter olarak gelir. */
    DWORD in = winInMode & ~(DWORD)(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT |
                                    ENABLE_VIRTUAL_TERMINAL_INPUT);
    if (!SetConsoleMode(winIn, in)) {
        SetConsoleMode(winOut, winOutMode);
        return false;
    }
    return true;
}

static void termLeave(void) {
    SetConsoleMode(winIn, winInMode);
    SetConsoleMode(winOut, winOutMode);
}

static int termColumns(void) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(winOut, &info)) {
        int columns = info.srWindow.Right - info.srWindow.Left + 1;
        if (columns > 0) return columns;
    }
    return 80;
}

/*
 * Çıktı WriteConsoleW ile yazılır: UTF-8 metni kendimiz UTF-16'ya çeviririz.
 * WriteConsoleA konsolun çıktı kod sayfasına (çoğu kurulumda 65001 değildir)
 * bağlıdır, WriteConsoleW ise kod sayfasından bağımsız ve her zaman doğrudur.
 * Geçersiz baytlar U+FFFD olur.
 */
static void termWrite(const char *data, size_t length) {
    if (length == 0) return;
    /* Her UTF-8 bayt en çok bir UTF-16 birimi üretir (4 bayt -> 2 birim). */
    WCHAR *wide = malloc(length * sizeof(WCHAR));
    if (wide == NULL) restoreAndExit();

    size_t count = 0;
    size_t i = 0;
    while (i < length) {
        uint32_t codePoint;
        int used = utf8Decode(data + i, (int)(length - i), &codePoint);
        if (used == 1 && (unsigned char)data[i] >= 0x80) codePoint = 0xFFFD;
        i += (size_t)used;
        if (codePoint > 0x10FFFF || (codePoint >= 0xD800 && codePoint <= 0xDFFF)) codePoint = 0xFFFD;
        if (codePoint >= 0x10000) {
            codePoint -= 0x10000;
            wide[count++] = (WCHAR)(0xD800 + (codePoint >> 10));
            wide[count++] = (WCHAR)(0xDC00 + (codePoint & 0x3FF));
        } else {
            wide[count++] = (WCHAR)codePoint;
        }
    }

    size_t done = 0;
    while (done < count) {
        DWORD written = 0;
        if (!WriteConsoleW(winOut, wide + done, (DWORD)(count - done), &written, NULL) || written == 0) break;
        done += written;
    }
    free(wide);
}

static uint32_t termReadKey(void) {
    static WORD pendingRepeat = 0;
    static uint32_t pendingKey = LK_IGNORE;
    static WCHAR highSurrogate = 0;

    if (pendingRepeat > 0) {
        pendingRepeat--;
        return pendingKey;
    }

    INPUT_RECORD record;
    DWORD read = 0;
    if (!ReadConsoleInputW(winIn, &record, 1, &read) || read == 0) return LK_EOF;
    if (record.EventType != KEY_EVENT) return LK_IGNORE;

    const KEY_EVENT_RECORD *event = &record.Event.KeyEvent;
    WCHAR unit = event->uChar.UnicodeChar;
    /* Alt+sayı tuşlarıyla girilen karakter, Alt'ın bırakılışında gelir. */
    if (!event->bKeyDown && !(event->wVirtualKeyCode == VK_MENU && unit != 0)) return LK_IGNORE;

    uint32_t key;
    if (unit == 0) {
        switch (event->wVirtualKeyCode) {
            case VK_UP: key = LK_UP; break;
            case VK_DOWN: key = LK_DOWN; break;
            case VK_LEFT: key = LK_LEFT; break;
            case VK_RIGHT: key = LK_RIGHT; break;
            case VK_HOME: key = LK_HOME; break;
            case VK_END: key = LK_END; break;
            case VK_DELETE: key = LK_DELETE; break;
            default: return LK_IGNORE;
        }
    } else if (unit >= 0xD800 && unit <= 0xDBFF) {
        highSurrogate = unit; /* vekil çiftinin ilk yarısı; ikincisini bekle */
        return LK_IGNORE;
    } else if (unit >= 0xDC00 && unit <= 0xDFFF) {
        if (highSurrogate == 0) return LK_IGNORE;
        key = 0x10000u + (((uint32_t)highSurrogate - 0xD800u) << 10) + ((uint32_t)unit - 0xDC00u);
        highSurrogate = 0;
    } else {
        highSurrogate = 0;
        key = unit;
    }

    if (event->wRepeatCount > 1) {
        pendingKey = key;
        pendingRepeat = (WORD)(event->wRepeatCount - 1);
    }
    return key;
}

#else

/* ---- POSIX uçbirim katmanı ---- */

static struct termios savedTermios;
static int pushedBack = -1; /* geçersiz çok baytlı dizide okunup geri bırakılan bayt */

static bool termEnter(void) {
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) return false;
    const char *term = getenv("TERM");
    if (term != NULL && strcmp(term, "dumb") == 0) return false;
    if (tcgetattr(STDIN_FILENO, &savedTermios) != 0) return false;

    struct termios raw = savedTermios;
    /* ISTRIP açık kalırsa UTF-8'in sekizinci biti atılır; ICRNL kapalı: Enter '\r' gelir. */
    raw.c_iflag &= ~(tcflag_t)(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_cflag |= (tcflag_t)CS8;
    /* ISIG kapalı: Ctrl+C karakter olarak gelir. Çıktı işleme (OPOST) açık bırakılır. */
    raw.c_lflag &= ~(tcflag_t)(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    /* TCSAFLUSH değil: yapıştırılmış, henüz okunmamış sonraki satırlar kaybolmasın. */
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &raw) != 0) return false;
    return true;
}

static void termLeave(void) {
    tcsetattr(STDIN_FILENO, TCSADRAIN, &savedTermios);
}

static int termColumns(void) {
    struct winsize size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col > 0) return size.ws_col;
    return 80;
}

static void termWrite(const char *data, size_t length) {
    size_t done = 0;
    while (done < length) {
        ssize_t written = write(STDOUT_FILENO, data + done, length - done);
        if (written < 0) {
            if (errno == EINTR) continue;
            break;
        }
        done += (size_t)written;
    }
}

/* Bir bayt okur. timeoutMs < 0 ise sonsuz bekler. Dönüş: bayt, -1 (EOF/hata) ya da -2 (süre doldu). */
static int readByte(int timeoutMs) {
    if (pushedBack >= 0) {
        int value = pushedBack;
        pushedBack = -1;
        return value;
    }

    if (timeoutMs >= 0) {
        struct pollfd descriptor;
        descriptor.fd = STDIN_FILENO;
        descriptor.events = POLLIN;
        descriptor.revents = 0;
        int ready;
        do {
            ready = poll(&descriptor, 1, timeoutMs);
        } while (ready < 0 && errno == EINTR);
        if (ready == 0) return -2;
        if (ready < 0) return -1;
    }

    for (;;) {
        unsigned char byte;
        ssize_t count = read(STDIN_FILENO, &byte, 1);
        if (count == 1) return byte;
        if (count == 0) return -1;
        if (errno != EINTR) return -1;
    }
}

static uint32_t keyFromFinal(int final) {
    switch (final) {
        case 'A': return LK_UP;
        case 'B': return LK_DOWN;
        case 'C': return LK_RIGHT;
        case 'D': return LK_LEFT;
        case 'H': return LK_HOME;
        case 'F': return LK_END;
        default: return LK_IGNORE;
    }
}

/* ESC okundu; devamındaki diziyi çözer. Tek başına ESC ve bilinmeyen diziler yok sayılır. */
static uint32_t readEscape(void) {
    int next = readByte(ESCAPE_WAIT_MS);
    if (next < 0) return LK_IGNORE;

    if (next == 'O') {
        int final = readByte(ESCAPE_WAIT_MS);
        return final < 0 ? LK_IGNORE : keyFromFinal(final);
    }
    if (next != '[') return LK_IGNORE; /* Alt+tuş: yut */

    char parameters[16];
    int count = 0;
    int final;
    for (;;) {
        final = readByte(ESCAPE_WAIT_MS);
        if (final < 0) return LK_IGNORE;
        if (final >= 0x40 && final <= 0x7E) break;
        if (count < (int)sizeof(parameters) - 1) parameters[count++] = (char)final;
    }
    parameters[count] = '\0';

    if (final == '[') { /* Linux konsolu işlev tuşları: ESC [ [ A */
        readByte(ESCAPE_WAIT_MS);
        return LK_IGNORE;
    }
    if (final == '~') {
        switch (atoi(parameters)) {
            case 1:
            case 7: return LK_HOME;
            case 4:
            case 8: return LK_END;
            case 3: return LK_DELETE;
            default: return LK_IGNORE;
        }
    }
    return keyFromFinal(final);
}

static uint32_t termReadKey(void) {
    int first = readByte(-1);
    if (first < 0) return LK_EOF;
    if (first == 27) return readEscape();
    if (first < 0x80) return (uint32_t)first;

    int size = (first & 0xE0) == 0xC0 ? 2 : (first & 0xF0) == 0xE0 ? 3 : (first & 0xF8) == 0xF0 ? 4 : 0;
    if (size == 0) return LK_IGNORE;

    char bytes[4];
    bytes[0] = (char)first;
    for (int i = 1; i < size; i++) {
        int next = readByte(200);
        if (next == -1) return LK_EOF;
        if (next == -2) return LK_IGNORE;
        if ((next & 0xC0) != 0x80) {
            pushedBack = next; /* dizi bozuk: bu bayt başka bir tuşa aittir */
            return LK_IGNORE;
        }
        bytes[i] = (char)next;
    }

    uint32_t codePoint;
    if (utf8Decode(bytes, size, &codePoint) != size) return LK_IGNORE;
    return codePoint;
}

#endif

/* ---- Ortak yardımcılar ---- */

/* Bellek tükenirse uçbirimi geri yükleyip programı bitirir (depodaki diğer yerler gibi). */
static void restoreAndExit(void) {
    if (terminalActive) {
        termLeave();
        terminalActive = false;
    }
    fprintf(stderr, "jus: bellek yetersiz.\n");
    exit(JUS_EXIT_OUT_OF_MEMORY);
}

static void *checked(void *pointer) {
    if (pointer == NULL) restoreAndExit();
    return pointer;
}

static char *copyBytes(const char *text, int length) {
    char *copy = checked(malloc((size_t)length + 1));
    memcpy(copy, text, (size_t)length);
    copy[length] = '\0';
    return copy;
}

/* ---- Geçmiş ---- */

static char *history[HISTORY_MAX];
static int historyCount = 0;

static bool isBlank(const char *line) {
    for (; *line != '\0'; line++) {
        if (*line != ' ' && *line != '\t' && *line != '\r' && *line != '\n') return false;
    }
    return true;
}

void lineEditHistoryAdd(const char *line) {
    if (line == NULL || isBlank(line)) return;
    if (historyCount > 0 && strcmp(history[historyCount - 1], line) == 0) return;

    if (historyCount == HISTORY_MAX) {
        free(history[0]);
        memmove(history, history + 1, sizeof(history[0]) * (HISTORY_MAX - 1));
        historyCount--;
    }
    history[historyCount++] = copyBytes(line, (int)strlen(line));
}

/*
 * Dosya biçimi: satır başına bir giriş. Giriş içindeki satır sonu "\n", satır
 * başı "\r" ve ters bölü "\\" olarak kaçışlanır; böylece kaynak koddaki "\n"
 * gibi metinler de geri yüklemede bozulmaz.
 */
void lineEditHistorySave(const char *path) {
    FILE *file = fopen(path, "wb");
    if (file == NULL) return;

    for (int i = 0; i < historyCount; i++) {
        for (const char *p = history[i]; *p != '\0'; p++) {
            if (*p == '\n') fputs("\\n", file);
            else if (*p == '\r') fputs("\\r", file);
            else if (*p == '\\') fputs("\\\\", file);
            else fputc(*p, file);
        }
        fputc('\n', file);
    }
    fclose(file);
}

/* Kaçışları çözer (yerinde); tanınmayan kaçışlar olduğu gibi kalır. */
static void unescapeHistoryLine(char *line, int *length) {
    int out = 0;
    for (int i = 0; i < *length; i++) {
        if (line[i] == '\\' && i + 1 < *length) {
            char next = line[i + 1];
            if (next == 'n') { line[out++] = '\n'; i++; continue; }
            if (next == 'r') { line[out++] = '\r'; i++; continue; }
            if (next == '\\') { line[out++] = '\\'; i++; continue; }
        }
        line[out++] = line[i];
    }
    *length = out;
    line[out] = '\0';
}

void lineEditHistoryLoad(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) return;

    int capacity = 256;
    int length = 0;
    char *line = checked(malloc((size_t)capacity));
    int c;

    /* UTF-8 imzası (BOM) varsa atla. */
    unsigned char mark[3];
    if (fread(mark, 1, 3, file) != 3 || mark[0] != 0xEF || mark[1] != 0xBB || mark[2] != 0xBF) rewind(file);

    for (;;) {
        c = fgetc(file);
        if (c == EOF || c == '\n') {
            if (c == EOF && length == 0) break;
            if (length > 0 && line[length - 1] == '\r') length--;
            line[length] = '\0';
            unescapeHistoryLine(line, &length);
            lineEditHistoryAdd(line);
            length = 0;
            if (c == EOF) break;
            continue;
        }
        if (length + 2 > capacity) {
            capacity *= 2;
            line = checked(realloc(line, (size_t)capacity));
        }
        line[length++] = (char)c;
    }

    free(line);
    fclose(file);
}

/* ---- Düzenleyici ---- */

typedef struct {
    char *buffer;
    int length;
    int capacity;
    int position; /* imleç, bayt cinsinden; her zaman bir kod noktası sınırında */
    const char *prompt;
    int promptWidth;
    LineEditCompleteFn complete;
    char **edits;   /* geçmiş gezintisinde düzenlenmiş satırlar (historyCount + 1 öğe; sonuncusu yazılmakta olan satır) */
    int historyIndex;
    char *out;      /* çizim tamponu */
    int outLength;
    int outCapacity;
} Editor;

static void outAppend(Editor *editor, const char *text, int length) {
    if (editor->outLength + length > editor->outCapacity) {
        int capacity = editor->outCapacity == 0 ? 256 : editor->outCapacity;
        while (capacity < editor->outLength + length) capacity *= 2;
        editor->out = checked(realloc(editor->out, (size_t)capacity));
        editor->outCapacity = capacity;
    }
    memcpy(editor->out + editor->outLength, text, (size_t)length);
    editor->outLength += length;
}

static void outAppendString(Editor *editor, const char *text) {
    outAppend(editor, text, (int)strlen(text));
}

/* İstemin görünen genişliği: renk gibi CSI dizileri sayılmaz. */
static int promptColumns(const char *prompt) {
    int width = 0;
    int length = (int)strlen(prompt);
    int i = 0;
    while (i < length) {
        if (prompt[i] == 27 && i + 1 < length && prompt[i + 1] == '[') {
            i += 2;
            while (i < length && !((unsigned char)prompt[i] >= 0x40 && (unsigned char)prompt[i] <= 0x7E)) i++;
            if (i < length) i++;
            continue;
        }
        uint32_t codePoint;
        i += utf8Decode(prompt + i, length - i, &codePoint);
        width++;
    }
    return width;
}

static void ensureCapacity(Editor *editor, int needed) {
    if (needed + 1 <= editor->capacity) return;
    int capacity = editor->capacity;
    while (capacity < needed + 1) capacity *= 2;
    editor->buffer = checked(realloc(editor->buffer, (size_t)capacity));
    editor->capacity = capacity;
}

static void insertBytes(Editor *editor, const char *bytes, int count) {
    ensureCapacity(editor, editor->length + count);
    memmove(editor->buffer + editor->position + count, editor->buffer + editor->position,
            (size_t)(editor->length - editor->position));
    memcpy(editor->buffer + editor->position, bytes, (size_t)count);
    editor->length += count;
    editor->position += count;
    editor->buffer[editor->length] = '\0';
}

/* [from, to) aralığını siler; imleç aralığın içindeyse başına, sonrasındaysa kayarak taşınır. */
static void deleteRange(Editor *editor, int from, int to) {
    if (from >= to) return;
    memmove(editor->buffer + from, editor->buffer + to, (size_t)(editor->length - to));
    editor->length -= to - from;
    editor->buffer[editor->length] = '\0';
    if (editor->position >= to) editor->position -= to - from;
    else if (editor->position > from) editor->position = from;
}

static void setBuffer(Editor *editor, const char *text) {
    int length = (int)strlen(text);
    ensureCapacity(editor, length);
    memcpy(editor->buffer, text, (size_t)length + 1);
    editor->length = length;
    editor->position = length;
}

static int previousOffset(const Editor *editor, int position) {
    int index = utf8Length(editor->buffer, position);
    return index > 0 ? utf8Offset(editor->buffer, editor->length, index - 1) : 0;
}

static int nextOffset(const Editor *editor, int position) {
    if (position >= editor->length) return editor->length;
    uint32_t codePoint;
    return position + utf8Decode(editor->buffer + position, editor->length - position, &codePoint);
}

/* Satırı yeniden çizer: görünen pencereyi seçer, metni yazar, imleci yerine koyar. */
static void refresh(Editor *editor) {
    int columns = termColumns();
    int available = columns - editor->promptWidth - 1; /* son sütun boş: otomatik satır sarmasını önler */
    if (available < 1) available = 1;

    int charLength = utf8Length(editor->buffer, editor->length);
    int cursor = utf8Length(editor->buffer, editor->position);
    int start = cursor >= available ? cursor - available + 1 : 0;
    int end = charLength;
    if (end - start > available) end = start + available;

    editor->outLength = 0;
    outAppendString(editor, "\r");
    outAppendString(editor, editor->prompt);

    int offset = utf8Offset(editor->buffer, editor->length, start);
    int limit = utf8Offset(editor->buffer, editor->length, end);
    while (offset < limit) {
        uint32_t codePoint;
        int size = utf8Decode(editor->buffer + offset, editor->length - offset, &codePoint);
        unsigned char first = (unsigned char)editor->buffer[offset];
        if (codePoint == '\n') {
            outAppend(editor, "\xC2\xB6", 2); /* çok satırlı geçmiş girişlerinde satır sonu: ¶ */
        } else if (codePoint == '\t') {
            outAppend(editor, " ", 1);
        } else if (codePoint < 32 || codePoint == 127 || (codePoint >= 0x80 && codePoint < 0xA0) ||
                   (size == 1 && first >= 0x80)) {
            outAppend(editor, "?", 1); /* denetim karakteri ya da geçersiz bayt */
        } else {
            outAppend(editor, editor->buffer + offset, size);
        }
        offset += size;
    }

    outAppendString(editor, "\x1b[0K\r");
    int column = editor->promptWidth + cursor - start;
    if (column > 0) {
        char move[24];
        snprintf(move, sizeof(move), "\x1b[%dC", column);
        outAppendString(editor, move);
    }
    termWrite(editor->out, (size_t)editor->outLength);
}

/* Geçmiş gezintisi: şu an görünen satırı, geri dönülürse korunmak üzere saklar. */
static void historyStore(Editor *editor) {
    if (editor->edits == NULL) return;
    int index = editor->historyIndex;
    char *copy = copyBytes(editor->buffer, editor->length);
    if (index < historyCount && strcmp(history[index], copy) == 0) {
        free(copy); /* değişmemiş: özgün geçmiş girişi yeterli */
        copy = NULL;
    }
    free(editor->edits[index]);
    editor->edits[index] = copy;
}

static void historyShow(Editor *editor) {
    int index = editor->historyIndex;
    const char *text = "";
    if (editor->edits != NULL && editor->edits[index] != NULL) text = editor->edits[index];
    else if (index < historyCount) text = history[index];
    setBuffer(editor, text);
}

/* ---- Tamamlama ---- */

typedef struct {
    char **items;
    int count;
    int capacity;
    const char *prefix;
    int prefixLength;
} Candidates;

static void addCandidate(const char *candidate, void *context) {
    Candidates *candidates = context;
    if (candidate == NULL) return;
    size_t length = strlen(candidate);
    if (length < (size_t)candidates->prefixLength ||
        memcmp(candidate, candidates->prefix, (size_t)candidates->prefixLength) != 0) {
        return;
    }
    if (candidates->count == candidates->capacity) {
        candidates->capacity = candidates->capacity == 0 ? 16 : candidates->capacity * 2;
        candidates->items = checked(realloc(candidates->items, sizeof(char *) * (size_t)candidates->capacity));
    }
    candidates->items[candidates->count++] = copyBytes(candidate, (int)length);
}

static int compareCandidates(const void *a, const void *b) {
    const char *left = *(char *const *)a;
    const char *right = *(char *const *)b;
    return trCompare(left, (int)strlen(left), right, (int)strlen(right));
}

static bool isNameByte(unsigned char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c >= 0x80;
}

/* Adaylar sütunlar halinde (ls gibi, sütun sırasıyla) yeni satırlara yazılır. */
static void printCandidates(char **items, int count) {
    int widest = 0;
    for (int i = 0; i < count; i++) {
        int width = utf8Length(items[i], (int)strlen(items[i]));
        if (width > widest) widest = width;
    }
    int columnWidth = widest + 2;
    int columns = termColumns();
    int perRow = columns / columnWidth;
    if (perRow < 1) perRow = 1;
    int rows = (count + perRow - 1) / perRow;

    termWrite("\r\n", 2);
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < perRow; column++) {
            int index = column * rows + row;
            if (index >= count) break;
            termWrite(items[index], strlen(items[index]));
            int next = (column + 1) * rows + row;
            if (column + 1 < perRow && next < count) {
                int pad = columnWidth - utf8Length(items[index], (int)strlen(items[index]));
                char spaces[64];
                while (pad > 0) {
                    int chunk = pad < (int)sizeof(spaces) ? pad : (int)sizeof(spaces);
                    memset(spaces, ' ', (size_t)chunk);
                    termWrite(spaces, (size_t)chunk);
                    pad -= chunk;
                }
            }
        }
        termWrite("\r\n", 2);
    }
}

static void completeWord(Editor *editor) {
    if (editor->complete == NULL) return;

    int start = editor->position;
    while (start > 0 && isNameByte((unsigned char)editor->buffer[start - 1])) start--;
    int prefixLength = editor->position - start;
    if (prefixLength == 0) return;

    /* Geri çağrı sırasında tampon değişmez; yine de ön eki kopyalayıp veririz. */
    char *prefix = copyBytes(editor->buffer + start, prefixLength);
    Candidates candidates = {NULL, 0, 0, prefix, prefixLength};
    editor->complete(prefix, prefixLength, addCandidate, &candidates);

    if (candidates.count > 0) {
        qsort(candidates.items, (size_t)candidates.count, sizeof(char *), compareCandidates);
        int unique = 1;
        for (int i = 1; i < candidates.count; i++) {
            if (strcmp(candidates.items[i], candidates.items[unique - 1]) == 0) free(candidates.items[i]);
            else candidates.items[unique++] = candidates.items[i];
        }
        candidates.count = unique;

        /* Ortak önek: bayt bayt bulunur, sonra kod noktası sınırına geri çekilir. */
        const char *first = candidates.items[0];
        int common = (int)strlen(first);
        for (int i = 1; i < candidates.count; i++) {
            int j = 0;
            while (j < common && candidates.items[i][j] == first[j]) j++;
            common = j;
        }
        while (common > prefixLength && ((unsigned char)first[common] & 0xC0) == 0x80) common--;

        if (common > prefixLength) insertBytes(editor, first + prefixLength, common - prefixLength);
        refresh(editor);
        if (candidates.count > 1) {
            printCandidates(candidates.items, candidates.count);
            refresh(editor);
        }
    }

    for (int i = 0; i < candidates.count; i++) free(candidates.items[i]);
    free(candidates.items);
    free(prefix);
}

/* ---- Ana işlev ---- */

bool lineEditSupported(void) {
    if (!termEnter()) return false;
    termLeave();
    return true;
}

/* Uçbirim desteklenmiyorsa (yönlendirilmiş girdi gibi) düz okuma. */
static char *readPlainLine(const char *prompt) {
    fputs(prompt, stdout);
    fflush(stdout);

    int capacity = 128;
    int length = 0;
    char *line = checked(malloc((size_t)capacity));
    int c;
    while ((c = fgetc(stdin)) != EOF && c != '\n') {
        if (length + 2 > capacity) {
            capacity *= 2;
            line = checked(realloc(line, (size_t)capacity));
        }
        line[length++] = (char)c;
    }
    if (c == EOF && length == 0) {
        free(line);
        return NULL;
    }
    if (length > 0 && line[length - 1] == '\r') length--;
    line[length] = '\0';
    return line;
}

static bool insertable(uint32_t codePoint) {
    if (codePoint < 32 || codePoint == 127) return false;
    if (codePoint >= 0x80 && codePoint < 0xA0) return false;
    if (codePoint >= 0xD800 && codePoint <= 0xDFFF) return false;
    return codePoint <= 0x10FFFF;
}

char *lineEdit(const char *prompt, LineEditCompleteFn complete) {
    if (prompt == NULL) prompt = "";
    fflush(stdout);
    fflush(stderr);

    if (!termEnter()) return readPlainLine(prompt);
    terminalActive = true;

    /* İstem birden çok satırsa son satır dışındakiler bir kez yazılır; yeniden çizilen yalnızca son satırdır. */
    const char *lastBreak = strrchr(prompt, '\n');
    if (lastBreak != NULL) {
        const char *segment = prompt;
        for (const char *p = prompt; p <= lastBreak; p++) {
            if (*p != '\n') continue;
            termWrite(segment, (size_t)(p - segment));
            termWrite("\r\n", 2);
            segment = p + 1;
        }
        prompt = lastBreak + 1;
    }

    Editor editor;
    memset(&editor, 0, sizeof(editor));
    editor.capacity = 128;
    editor.buffer = checked(malloc((size_t)editor.capacity));
    editor.buffer[0] = '\0';
    editor.prompt = prompt;
    editor.promptWidth = promptColumns(prompt);
    editor.complete = complete;
    editor.edits = calloc((size_t)historyCount + 1, sizeof(char *)); /* NULL olabilir: düzenlemeler korunmaz */
    editor.historyIndex = historyCount;

    char *result = NULL;
    bool done = false;
    refresh(&editor);

    while (!done) {
        uint32_t key = termReadKey();
        switch (key) {
            case LK_EOF:
                done = true;
                break;

            case LK_IGNORE:
                break;

            case '\r':
            case '\n':
                editor.position = editor.length;
                refresh(&editor);
                termWrite("\r\n", 2);
                result = editor.buffer;
                editor.buffer = NULL;
                done = true;
                break;

            case 3: /* Ctrl+C */
                termWrite("^C\r\n", 4);
                result = copyBytes("", 0);
                done = true;
                break;

            case 4: /* Ctrl+D */
                if (editor.length == 0) {
                    termWrite("\r\n", 2);
                    done = true;
                } else if (editor.position < editor.length) {
                    deleteRange(&editor, editor.position, nextOffset(&editor, editor.position));
                    refresh(&editor);
                }
                break;

#ifdef _WIN32
            case 26: /* Ctrl+Z: Windows'ta boş satırda girdi sonu */
                if (editor.length == 0) {
                    termWrite("\r\n", 2);
                    done = true;
                }
                break;
#endif

            case 1: /* Ctrl+A */
            case LK_HOME:
                editor.position = 0;
                refresh(&editor);
                break;

            case 5: /* Ctrl+E */
            case LK_END:
                editor.position = editor.length;
                refresh(&editor);
                break;

            case 2: /* Ctrl+B */
            case LK_LEFT:
                editor.position = previousOffset(&editor, editor.position);
                refresh(&editor);
                break;

            case 6: /* Ctrl+F */
            case LK_RIGHT:
                editor.position = nextOffset(&editor, editor.position);
                refresh(&editor);
                break;

            case 8:   /* Ctrl+H */
            case 127: /* Backspace */
                if (editor.position > 0) {
                    deleteRange(&editor, previousOffset(&editor, editor.position), editor.position);
                    refresh(&editor);
                }
                break;

            case LK_DELETE:
                if (editor.position < editor.length) {
                    deleteRange(&editor, editor.position, nextOffset(&editor, editor.position));
                    refresh(&editor);
                }
                break;

            case 11: /* Ctrl+K */
                deleteRange(&editor, editor.position, editor.length);
                refresh(&editor);
                break;

            case 21: /* Ctrl+U */
                deleteRange(&editor, 0, editor.position);
                refresh(&editor);
                break;

            case 23: { /* Ctrl+W: önceki kelime (boşluklarla ayrılmış) */
                int from = editor.position;
                while (from > 0 && (editor.buffer[from - 1] == ' ' || editor.buffer[from - 1] == '\t')) from--;
                while (from > 0 && editor.buffer[from - 1] != ' ' && editor.buffer[from - 1] != '\t') from--;
                deleteRange(&editor, from, editor.position);
                refresh(&editor);
                break;
            }

            case 12: /* Ctrl+L */
                termWrite("\x1b[H\x1b[2J", 7);
                refresh(&editor);
                break;

            case 16: /* Ctrl+P */
            case LK_UP:
                if (editor.historyIndex > 0) {
                    historyStore(&editor);
                    editor.historyIndex--;
                    historyShow(&editor);
                    refresh(&editor);
                }
                break;

            case 14: /* Ctrl+N */
            case LK_DOWN:
                if (editor.historyIndex < historyCount) {
                    historyStore(&editor);
                    editor.historyIndex++;
                    historyShow(&editor);
                    refresh(&editor);
                }
                break;

            case 9: /* Tab */
                completeWord(&editor);
                break;

            default:
                if (key < LK_UP && insertable(key)) {
                    char encoded[4];
                    int size = utf8Encode(key, encoded);
                    insertBytes(&editor, encoded, size);
                    refresh(&editor);
                }
                break;
        }
    }

    termLeave();
    terminalActive = false;

    if (editor.edits != NULL) {
        for (int i = 0; i <= historyCount; i++) free(editor.edits[i]);
        free(editor.edits);
    }
    free(editor.buffer);
    free(editor.out);
    return result;
}
