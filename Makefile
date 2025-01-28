# Plan :
# TODO: add explanation , and improve the general cleaness
SOURCEDIR := src
CFLAGS =-std=gnu99 -ffreestanding -O2 -Wall -Wextra
GCC = /home/yonatan/opt/cross/bin/i686-elf-gcc
GCC-ASM = /home/yonatan/opt/cross/bin/i686-elf-as
CRTI_OBJ=src/crt/crti.o
CRTBEGIN_OBJ:=$(shell $(GCC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(GCC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ=src/crt/crtn.o

C_SRCS := $(shell find $(SOURCEDIR) -type f -name '*.c')
C_OBJS := $(patsubst %.c,%.o,$(C_SRCS))
ASM_SRCS := $(shell find $(SOURCEDIR) -type f -name '*.asm')
ASM_OBJS := $(patsubst %.asm,%.o,$(ASM_SRCS))
BOOT_OBJ := src/boot/boot.o
.PHONY:all kernel image

all: kernel image


kernel: $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(C_OBJS) $(ASM_OBJS) $(BOOT_OBJ) $(CRTEND_OBJ) $(CRTN_OBJ)
	$(GCC) -T linker.ld -o isodir/boot/myos.bin $^  font.o -nostdlib -ffreestanding -lgcc 
image:
	$(shell grub-mkrescue -o myos.iso isodir)
	rm $(C_OBJS)
	rm $(ASM_OBJS)
	rm $(CRTN_OBJ)
	rm $(CRTI_OBJ)
	rm $(BOOT_OBJ)



$(C_OBJS): %.o: %.c

	$(GCC) $(CFLAGS) -c $^ -o $@
%.c:
	touch $@
$(ASM_OBJS): %.o: %.asm
	nasm -f elf32 $^ -o $@
$(CRTI_OBJ): %.o: %.s
	echo gyatt
	$(GCC-ASM) $^ -o $@
	echo skibidy
$(CRTN_OBJ): %.o : %.s
	$(GCC-ASM) $^ -o $@
$(BOOT_OBJ):%.o : %.S
	$(GCC-ASM) $^ -o $@

%.s:
	echo gyatt

	touch $@
%.asm:
	touch $@


