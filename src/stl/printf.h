#ifndef PRINT_F
#define PRINT_F
#include <stddef.h>
#include <stdbool.h> 
#include <stdarg.h>
#include <stdint.h>
#include "string.h"
#include "ctype.h"
#include "../kernel/framebuffer.h"
#include "../debugging/qemu.h"
#define DEBUGGING_SERIAL true
char* __int_str(intmax_t i, char b[], int base, bool plusSignIfNeeded, bool spaceSignIfNeeded,
    int paddingNo, bool justify, bool zeroPad) ;
int vprintf (const char* format, va_list list);
int printf (const char* format, ...);
#endif