#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "memory.h"
#include "../boot/multiboot.h"
#include "../debugging/qemu.h"
static uint8_t pixelwidth;
static uint32_t screen_width;
static uint32_t screen_height;

static uint32_t pitch;
void init(multiboot_info_t *mbi);
void cls(void);
void putchar(int c);
void putpixel(int x,int y,int color);
void drawchar(unsigned char c, int x, int y);
#endif 
