#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../boot/multiboot.h"
#include "../gdt/gdt.h"
/*  Some screen stuff. */
/*  The number of columns. */

/*  The attribute of an character. */
#define ATTRIBUTE               7
/*  The video memory address. */
#define VIDEO                   0xB8000
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
/*  Variables. */
/*  Save the X position. */




/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

/*  Format a string and print it on the screen, just like the libc
   function printf. */
void printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;
  
  while ((c = *format++) != 0)
    {
      if (c != '%')
        putchar (c);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;
          
          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                putchar (pad0 ? '0' : ' ');
              while (*p)
                putchar (*p++);
              break;

            default:
              putchar (*((int *) arg++));
              break;
            }
        }
    }
}


void kernel_main(unsigned long magic, unsigned long addr){

  /*  Clear the screen. */
 multiboot_info_t *mbi;
  
  /* Clear the screen. */


  /* Am I booted by a Multiboot-compliant boot loader? */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
      return;
    }

  /* Set MBI to the address of the Multiboot information structure. */
  mbi = (multiboot_info_t *) addr;

  /* Print out the flags. */

  /* Are mem_* valid? */


  /* Is boot_device valid? */

  /* Are mods_* valid? */
//   if (CHECK_FLAG (mbi->flags, 3))
//     {
//       multiboot_module_t *mod;
//       int i;
      

//       for (i = 0, mod = (multiboot_module_t *) mbi->mods_addr;
//            i < mbi->mods_count;
//            i++, mod++)
//         printf (" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
//                 (unsigned) mod->mod_start,
//                 (unsigned) mod->mod_end,
//                 (char *) mod->cmdline);
//     }

  /* Bits 4 and 5 are mutually exclusive! */
//   if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
//     {
//       printf ("Both bits 4 and 5 are set.\n");
//       return;
//     }

//   /* Is the symbol table of a.out valid? */
//   if (CHECK_FLAG (mbi->flags, 4))
//     {
//       multiboot_aout_symbol_table_t *multiboot_aout_sym = &(mbi->u.aout_sym);
      
//       printf ("multiboot_aout_symbol_table: tabsize = 0x%0x, "
//               "strsize = 0x%x, addr = 0x%x\n",
//               (unsigned) multiboot_aout_sym->tabsize,
//               (unsigned) multiboot_aout_sym->strsize,
//               (unsigned) multiboot_aout_sym->addr);
//     }

  /* Is the section header table of ELF valid? */
//   if (CHECK_FLAG (mbi->flags, 5))
//     {
//       multiboot_elf_section_header_table_t *multiboot_elf_sec = &(mbi->u.elf_sec);

//       printf ("multiboot_elf_sec: num = %u, size = 0x%x,"
//               " addr = 0x%x, shndx = 0x%x\n",
//               (unsigned) multiboot_elf_sec->num, (unsigned) multiboot_elf_sec->size,
//               (unsigned) multiboot_elf_sec->addr, (unsigned) multiboot_elf_sec->shndx);
//     }

//   /* Are mmap_* valid? */
//   if (CHECK_FLAG (mbi->flags, 6))
//     {
//       multiboot_memory_map_t *mmap;
      
//       printf ("mmap_addr = 0x%x, mmap_length = 0x%x\n",
//               (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
//       for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
//            (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
//            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
//                                     + mmap->size + sizeof (mmap->size)))
//         printf (" size = 0x%x, base_addr = 0x%x%08x,"
//                 " length = 0x%x%08x, type = 0x%x\n",
//                 (unsigned) mmap->size,
//                 (unsigned) (mmap->addr >> 32),
//                 (unsigned) (mmap->addr & 0xffffffff),
//                 (unsigned) (mmap->len >> 32),
//                 (unsigned) (mmap->len & 0xffffffff),
//                 (unsigned) mmap->type);
//     }

  /* Draw diagonal blue line. */
  if (CHECK_FLAG (mbi->flags, 12))
	{
		multiboot_uint32_t color;
		unsigned i;
		fb =(char *) ((void *) (unsigned long) mbi->framebuffer_addr);
		
		pixelwidth=mbi->framebuffer_bpp/8;
		screen_width = mbi->framebuffer_width;
		screen_height = mbi->framebuffer_height;
		pitch = mbi->framebuffer_pitch;
	}
	drawchar('a',0,0);
	// for(int i = 0;i<255;i++){
	// 	drawchar(i,(i*9)%screen_width  , (i*9/screen_width)*16);
	// }
	// for(int i=0;i<255;i++){

	// 	putpixel(i,i,0xff);
	// }







}