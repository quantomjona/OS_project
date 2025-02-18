#ifndef KMALLOC_H
#define KMALLOC_H

#include "../memory.h"
#include <stdint.h>
#include <stdbool.h>
void kmallocInit(uint32_t initialHeapSize);
void changeHeapSize(int newSize);
#endif