#include "qemu.h"
// debugging paging
void print_page_mapping(uint64_t virt_addr){
    uint32_t pdIndex = virt_addr >> 22;
    uint32_t ptIndex = virt_addr >> 12 & 0x3FF;
    uint32_t offset = (virt_addr) & 0xFFF;
    uint32_t* pageDir = REC_PAGEDIR;
    uint32_t* pt = REC_PAGETABLE(pdIndex);
    
    printf("virtual address %x is mapped to physical adress %x , page table index : %i , offset : %x \n" , virt_addr ,pt[ptIndex] + offset , ptIndex , offset);
}