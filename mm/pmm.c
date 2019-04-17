#include "multiboot.h"
#include "debug.h"
#include "pmm.h"

// 输出 BIOS 提供的物理内存布局
void show_memory_map()
{
    uint32_t mmap_addr = glb_mboot_ptr->addr;
    uint32_t mmap_length = glb_mboot_ptr->mmap_length;

    printk("Memory map:\n");

    for (mmap_entry_t *mmap_entry = mmap_addr; mmap_entry < mmap_addr + mmap_length; ++mmap_addr)
    {
        printk("base_addr = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",
               (uint32_t)mmap_entry−> base_addr_high, (uint32_t)mmap_entry−> base_addr_low,
               (uint32_t)mmap_entry−> length_high, (uint32_t)mmap_entry−> length_low, 
               (uint32_t)mmap_entry−> type);
    }
}