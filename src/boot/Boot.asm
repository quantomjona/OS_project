MBOOT_PAGE_ALIGN EQU 1
MBOOT_MEM_INFO EQU 2
MBOOT_USE_GFX EQU 4

MBOOT_MAGIC EQU 0x1BADB002
MBOOT_FLAGS EQU MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_USE_GFX
MBOOT_CHECKSUM EQU -(MBOOT_MAGIC + MBOOT_FLAGS)

section .multiboot
ALIGN 4
    DD MBOOT_MAGIC
    DD MBOOT_FLAGS
    DD MBOOT_CHECKSUM

    DD 0, 0, 0, 0, 0



    DD 0
    DD 0
    DD 0
    DD 32
SECTION .bss
ALIGN 16
stack_bottom:
    RESB 16384 * 8
stack_top:
end_:

section .boot

global _start
_start:
    ;moves initial_page_dir to ecx,subtructing 0xC... because initial_page_dir is located at higher half
    MOV ecx, (initial_page_dir - 0xC0000000)
    MOV cr3, ecx ; moving pointer into cr3 register ( setting the location of the directory) 

    MOV ecx, cr4 ;moving cr4
    OR ecx, 0x10 ;enable page size extension (PSE)
    MOV cr4, ecx ;mov back into cr4
 
    MOV ecx, cr0 ;mov from cr0
    OR ecx, 0x80000000 ; enable paging
    MOV cr0, ecx ;move back

    JMP higher_half ; jump to higher half

section .text
global higher_half
higher_half:
    MOV esp, stack_top ; initalise stack
    PUSH ebx ; push multiboot data
    PUSH eax ; push magic number
    XOR ebp, ebp ; 
    extern kernel_main
    CALL kernel_main

halt:
    hlt
    JMP halt


section .data
align 4096
global initial_page_dir
initial_page_dir:
    DD 10000011b ;131 or 0x83
    ; DD 10000011b | (1<<22)
    TIMES 768-1 DD 0
    ; this is a weird part , but since we want to map our kernel to 0xC0000000, 0xC00 must be the byte offset of the table containing the kernel inside the initial_page_dir
    ; since each entry is 4 bytes in length, 4 * 768 = 0xC00, and so this is where the kernel is.
    ; we also map the kernel to 4kb of memory hence the 4 entrys
    DD (0 << 22) | 10000011b ; 0 | ... 
    DD (1 << 22) | 10000011b ; 0x400000 |...
    DD (2 << 22) | 10000011b ; 0x800000 |...
    DD (3 << 22) | 10000011b ; 0xC00000 |...

    TIMES 256-4 DD 0
