#ifndef QEMU_DEBUG_H
#define QEMU_DEBUG_H
#include <stdint.h>
#include <stdbool.h>
#include "../stl/printf.h"
#include "../kernel/memory.h"
extern uint32_t initial_page_dir[1024];

static inline void outportw(uint16_t port, uint16_t val)
{
    __asm__ volatile ( "outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}
static inline void outportb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}
#define BochsBreak() outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);

void inline QemuConsolePrintChar(char c){
  outportb(0x3f8, c);
} 

void inline QemuConsolePrintStr(char * s){
   while(*s) {

    QemuConsolePrintChar(*s); 
    s++;
   }
}
void print_page_mapping(uint64_t virt_addr);
#endif