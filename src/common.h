#ifndef JUS_COMMON_H
#define JUS_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define JUS_VERSION "1.0.0"

#define UINT8_COUNT (UINT8_MAX + 1)

/* Çıkış kodları (sysexits.h ile uyumlu). */
#define JUS_EXIT_USAGE 64
#define JUS_EXIT_COMPILE_ERROR 65
#define JUS_EXIT_NO_INPUT 66
#define JUS_EXIT_RUNTIME_ERROR 70
#define JUS_EXIT_OUT_OF_MEMORY 71

#endif
