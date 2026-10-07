# JUS derleme dosyası
#
#   make          yorumlayıcıyı derler
#   make test     test paketini çalıştırır
#   make examples örnek programların sınamalarını çalıştırır
#   make clean    derleme çıktılarını siler
#
# Ek derleyici bayrakları EXTRA ile verilir, örneğin:
#   make EXTRA="-g -fsanitize=address,undefined -DJUS_DEBUG_STRESS_GC"

CC = gcc
CFLAGS = -O2
WARNINGS = -std=c99 -Wall -Wextra -pedantic
EXTRA =

ifeq ($(OS),Windows_NT)
EXE = .exe
LDLIBS = -lm -lws2_32
else
EXE =
LDLIBS = -lm
endif

TARGET = jus$(EXE)
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCES:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(EXTRA) $(OBJECTS) -o $@ $(LDLIBS)

build/%.o: src/%.c $(HEADERS) | build
	$(CC) $(WARNINGS) $(CFLAGS) $(EXTRA) -c $< -o $@

build:
	mkdir -p build

test: $(TARGET)
	bash tests/calistir.sh ./$(TARGET)
	bash tests/araclar.sh ./$(TARGET)

# Örnek programların uçtan uca sınamaları
examples: $(TARGET)
	for betik in examples/programlar/*/dene.sh; do bash $$betik ./$(TARGET) || exit 1; done

clean:
	rm -rf build $(TARGET)

.PHONY: all test examples clean
