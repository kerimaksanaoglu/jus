# JUS derleme dosyası
#
#   make          yorumlayıcıyı derler
#   make test     test paketini çalıştırır
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
else
EXE =
endif

TARGET = jus$(EXE)
SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
OBJECTS = $(SOURCES:src/%.c=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(EXTRA) $(OBJECTS) -o $@ -lm

build/%.o: src/%.c $(HEADERS) | build
	$(CC) $(WARNINGS) $(CFLAGS) $(EXTRA) -c $< -o $@

build:
	mkdir -p build

test: $(TARGET)
	bash tests/calistir.sh ./$(TARGET)

clean:
	rm -rf build $(TARGET)

.PHONY: all test clean
