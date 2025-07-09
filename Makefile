# JUS Yorumlayıcısı Makefile
# JUS Interpreter Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude
SRCDIR = src
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/lexer.c $(SRCDIR)/parser.c $(SRCDIR)/interpreter.c $(SRCDIR)/turkiye_features.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = jus

# Ana hedef / Main target
all: $(TARGET)

# Executable oluştur / Create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -lm

# Object dosyalarını derle / Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Temizlik / Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Debug sürümü / Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Test / Test
test: $(TARGET)
	@echo "JUS Yorumlayıcısı test ediliyor... / Testing JUS Interpreter..."
	@if [ -f examples/merhaba.jus ]; then \
		./$(TARGET) examples/merhaba.jus; \
	else \
		echo "Test dosyası bulunamadı / Test file not found: examples/merhaba.jus"; \
	fi

# Kurulum / Install (isteğe bağlı)
install: $(TARGET)
	@echo "JUS kurulumu henüz desteklenmiyor / JUS installation not yet supported"

# Yardım / Help
help:
	@echo "JUS Yorumlayıcısı Derleme Seçenekleri / JUS Interpreter Build Options:"
	@echo ""
	@echo "  make          - Normal derleme / Normal build"
	@echo "  make debug    - Debug derleme / Debug build"
	@echo "  make clean    - Temizlik / Clean build files"
	@echo "  make test     - Test çalıştır / Run tests"
	@echo "  make help     - Bu yardımı göster / Show this help"
	@echo ""
	@echo "Kullanım / Usage:"
	@echo "  ./jus dosya.jus"

# Phony targets
.PHONY: all clean debug test install help