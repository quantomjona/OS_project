
#ifndef MEMORY_H
#define KERNEL_START 0xC0000000
#define REC_PAGEDIR ((uint32_t *)0xFFFFF000)
#define REC_PAGETABLE(i) ((uint32_t *)(0xFFC00000 + ((i) << 12)))
#define CEIL_DIV(a, b) (((a + b) - 1) / b)
#define PAGE_FLAG_PRESENT (1 << 0)
#define PAGE_FLAG_WRITE (1 << 1)
#define PAGE_FLAG_OWNER (1 << 9)
#define MEMORY_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../stl/string.h"
#include "../debugging/qemu.h"
#include "../boot/multiboot.h"
extern uint32_t initial_page_dir[1024];
extern int mem_num_vpages;

void pmm_init(uint32_t memLow, uint32_t memHigh);
void initMemory(uint32_t memHigh, uint32_t physicalAllocStart);
void invalidate(uint32_t vaddr);
uint32_t pmmAllocPageFrame();
void syncPageDirs();
uint32_t *memGetCurrentPageDir();
void memMapPage(uint64_t virutalAddr, uint64_t physAddr, uint32_t flags);
void pmm_set(uint32_t frame, size_t count, bool avail);

void map_addr_fixed(uint32_t *pgd, uintptr_t vaddr, uintptr_t pstart,
                    size_t npages, bool user, bool overwrite);
static inline size_t pde_index(uintptr_t addr)
{
  return addr / (4096 * 1024);
}
static inline size_t pte_index(uintptr_t addr)
{
  return ((addr / 4096) % 1024);
}
#define PDE_ADDR_SHIFT 12
#define PDE_ADDR_MASK 0xFFFFF
#define PDE_EXTRACT_ADDR(v) \
  ((((v) >> PDE_ADDR_SHIFT) & PDE_ADDR_MASK) * 4096)

#define VIRT_BASE 0xC0000000
#define ADDR_TO_PHYS(addr) ((uintptr_t)addr - VIRT_BASE)
#define ADDR_TO_VIRT(addr) ((uintptr_t)addr + VIRT_BASE)
#endif