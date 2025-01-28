#include "gdt.h" 
extern void setGdt(uint32_t p) ;
static struct gdt_entry_struct gdt_entries[6];
extern void flushTss();

struct tss_entry tss_entry_;
struct gdt_ptr_struct gdt_ptr;

void initGdt(){
    gdt_ptr.limit = sizeof(struct gdt_entry_struct) * 6 - 1;
    gdt_ptr.base=(uint32_t)&gdt_entries;

    setGdtGate(0,0,0,0,0); //Null segment
    setGdtGate(1,0,0xFFFFFFFF, 0x9A, 0xCF); //Kernel code segment
    setGdtGate(2,0,0xFFFFFFFF, 0x92, 0xCF); //Kernel data segment
    setGdtGate(3,0,0xFFFFFFFF, 0xFA, 0xCF); //User code segment
    setGdtGate(4,0,0xFFFFFFFF, 0xF2, 0xCF); //User data segment
    writeTss(5,0x10, 0x0);
    setGdt((uint32_t)&gdt_ptr);

    flushTss();
    

}
void writeTss (uint32_t location , uint32_t ss0, uint32_t esp0){
    uint32_t base = (uint32_t)&tss_entry_;
    uint32_t limit = sizeof (struct tss_entry);
    setGdtGate(location,base,limit,0xE9,0x00);
    memset(&tss_entry_, 0, sizeof (struct tss_entry));

	tss_entry_.ss0  = ss0;  // Set the kernel stack segment.
	tss_entry_.esp0 = esp0; // Set the kernel stack pointer.

    tss_entry_.cs = 0x08 | 0x3;
    tss_entry_.ss = tss_entry_.ds = tss_entry_.es = tss_entry_.fs = tss_entry_.gs = 0x10 | 0x3;

}
void setGdtGate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){

    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit = (limit & 0xFFFF);
    gdt_entries[num].flags = (limit >> 16) & 0x0F;
    gdt_entries[num].flags |= (gran & 0xF0);

    gdt_entries[num].access = access;

}
