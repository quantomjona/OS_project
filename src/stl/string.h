#ifndef STRING_H
#define STRING_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
char* strcpy(char* destination, const char* source);
char *strcat(char *destination, const char *source);
void *memcpy(void *dest_str, const void * src_str, size_t n);
void *memset(void *str, int c, size_t n);
size_t
strlen(const char *str);
#endif
