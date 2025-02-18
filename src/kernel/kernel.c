#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "../boot/multiboot.h"
#include "../gdt/gdt.h"
#include "framebuffer.h"
#include "../mm/memory.h"
#include "../stl/printf.h"
/*  Some screen stuff. */
/*  The number of columns. */

/*  The attribute of an character. */
#define ATTRIBUTE 7
/*  The video memory address. */
#define CHECK_FLAG(flags, bit) ((flags) & (1 << (bit)))

// page_directory[i] = 
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
#include "../debugging/qemu.h"

static inline void flush_tlb() {
    uint32_t cr3;
    __asm__ volatile("mov %%cr3, %0" : "=r"(cr3));
    __asm__ volatile("mov %0, %%cr3" :: "r"(cr3));
}

extern uint32_t initial_page_dir[1024];
static char * itoa( uint64_t value, char * str, int base )
{
    char * rc;
    char * ptr;
    char * low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    rc = ptr = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

/*  Format a string and print it on the screen, just like the libc
   function printf. */



void kernel_main(uint32_t magic,multiboot_info_t * mbi)
{
  

  /*  Clear the screen. */
  initGdt();

  uint32_t mod1 = *(uint32_t * )(mbi->mods_addr + 4);
  uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
  initMemory(mbi->mem_upper * 1024,physicalAllocStart);
  kmallocInit(0x1000);

//   char * ch = (char *)0x5000;
//   itoa(mbi->framebuffer_addr,ch,10);
//   QemuConsolePrintStr(ch);
// //   QemuConsolePrintChar('\n');
//     for(uint64_t i = 0;i<mbi->framebuffer_width * mbi->framebuffer_height*mbi->framebuffer_bpp * 8 * 2;i+=32768){
//     memMapPage(0x7A000 + i ,mbi->framebuffer_addr + i,2|1);
//   }
    for(uint64_t i = 0;i<mbi->framebuffer_height * mbi->framebuffer_pitch;i+=4096){
        memMapPage(0xCA7000 + i ,mbi->framebuffer_addr + i,2|1);
    }

  


    // init(mbi);

  

}