#include "memory.h"
#define NUM_PAGES_DIRS 256
#define NUM_PAGE_FRAMES (0x100000000 / 0x1000 / 8)
#define BLOCK_SIZE 4096

static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalAlloc;
int mem_num_vpages;
static int used_blocks;
uint8_t physicalMemoryBitmap[1024]; //Dynamically, bit array
// TODO: create all 1024 page tables
static uint32_t pageDirs[NUM_PAGES_DIRS][1024] __attribute__((aligned(4096)));
static uint8_t pageDirUsed[NUM_PAGES_DIRS];



static uint32_t heapStart;
static uint32_t heapSize;
static uint32_t threshold;
static bool kmallocInitalized = false;
uint32_t get_heap_size(){
    return heapSize;
}
void kmallocInit(uint32_t initialHeapSize){
    heapStart = KERNEL_MALLOC;
    heapSize = 0;
    threshold = 0;
    kmallocInitalized = true;

    changeHeapSize(initialHeapSize);
    *((uint32_t*)heapStart) = 0;
}

void changeHeapSize(int newSize){
    int oldPageTop = CEIL_DIV(heapSize, 0x1000);
    int newPageTop = CEIL_DIV(newSize, 0x1000);

    if (newPageTop > oldPageTop){
        int diff = newPageTop - oldPageTop;

        for (int i = 0; i < diff; i++){
            uint32_t phys = pmmAllocPageFrame();
            memMapPage(KERNEL_MALLOC + oldPageTop * 0x1000 + i * 0x1000, phys, PAGE_FLAG_WRITE);
        }
    }

    heapSize = newSize;
}



void pmm_init(uint32_t memLow, uint32_t memHigh)
{
    pageFrameMin = CEIL_DIV(memLow, 0x1000);
    pageFrameMax = memHigh / 0x1000;
    totalAlloc = 0;

    memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
} 


void initMemory(uint32_t memHigh, uint32_t physicalAllocStart){
    mem_num_vpages = 0;
    // initial_page_dir[0] = 0;
    // invalidate(0);
     
    initial_page_dir[1023] = ((uint32_t) initial_page_dir - KERNEL_START) | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    // this may seem strange, but what we are doing here is called recursive mapping which is really cool when you understand it,
    // lets act like the cpu for a second, INSERT EXPLENATION HERE
    invalidate(0xFFFFF000);

    pmm_init(physicalAllocStart, memHigh);
    memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
    used_blocks= NUM_PAGE_FRAMES;
    memset(pageDirUsed, 0, NUM_PAGES_DIRS);
}

void invalidate(uint32_t vaddr){
    asm volatile("invlpg %0" :: "m"(vaddr));
}



uint32_t* memGetCurrentPageDir(){
    uint32_t pd;
    asm volatile("mov %%cr3, %0": "=r"(pd));
    pd += KERNEL_START;

    return (uint32_t*) pd;
}

void memChangePageDir(uint32_t* pd){
    pd = (uint32_t*) (((uint32_t)pd)-KERNEL_START);
    asm volatile("mov %0, %%eax \n mov %%eax, %%cr3 \n" :: "m"(pd));
}

void syncPageDirs(){
    for (int i = 0; i < NUM_PAGES_DIRS; i++){
        if (pageDirUsed[i]){
            uint32_t* pageDir = pageDirs[i];

            for (int i = 768; i < 1023; i++){
                pageDir[i] = initial_page_dir[i] & ~PAGE_FLAG_OWNER;
            }
        }
    }
}
void memMapPage(uint64_t virutalAddr, uint64_t physAddr, uint32_t flags){
    uint32_t *prevPageDir = 0;

    if (virutalAddr >= KERNEL_START){
        prevPageDir = memGetCurrentPageDir();
        if (prevPageDir != initial_page_dir){
            memChangePageDir(initial_page_dir);
        }
    }

    uint32_t pdIndex = virutalAddr >> 22;
    uint32_t ptIndex = virutalAddr >> 12 & 0x3FF;
    
    uint32_t* pageDir = REC_PAGEDIR;
    uint32_t* pt = REC_PAGETABLE(pdIndex);

    if (!(pageDir[pdIndex] & PAGE_FLAG_PRESENT)){
        uint32_t ptPAddr = pmmAllocPageFrame();
        
        pageDir[pdIndex] = ptPAddr | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE | PAGE_FLAG_OWNER | flags;
        invalidate(virutalAddr);
 
        for (uint32_t i = 0; i < 1024; i++){
            pt[i] = 0;
        }
    }

    pt[ptIndex] = physAddr | PAGE_FLAG_PRESENT | flags;
    mem_num_vpages++;
    invalidate(virutalAddr);

    if (prevPageDir != 0){
        syncPageDirs();

        if (prevPageDir != initial_page_dir){
            memChangePageDir(prevPageDir);
        }
    }

}

uint32_t pmmAllocPageFrame(){
    uint32_t start = pageFrameMin / 8 + ((pageFrameMin & 7) != 0 ? 1 : 0);
    uint32_t end = pageFrameMax / 8 - ((pageFrameMax & 7) != 0 ? 1 : 0);

    for (uint32_t b = start; b < end; b++){
        uint8_t byte = physicalMemoryBitmap[b];
        if (byte == 0xFF){
            continue;
        }

        for (uint32_t i = 0; i < 8; i++){
            bool used = byte >> i & 1;

            if (!used){
                byte ^= (-1 ^ byte) & (1 << i);
                physicalMemoryBitmap[b] = byte;
                totalAlloc++;

                uint32_t addr = (b*8*i) * 0x1000;
                return addr;
            }
        }
        
    }

    return 0;
}

