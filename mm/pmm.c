#include "debug.h"
#include "pmm.h"

// 物理内存页面管理的栈
static uint32_t pmm_stack[PAGE_MAX_SIZE + 1];

// 物理内存管理的栈指针
static uint32_t pmm_stack_top;

// 物理内存页的数量
uint32_t phy_page_count;

// 输出 BIOS 提供的物理内存布局
void show_memory_map()
{
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_length = glb_mboot_ptr->mmap_length;

    printk("Memory map:\n");

    for (mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr; (uint32_t)mmap < mmap_addr + mmap_length; ++mmap)
    {
        printk("base_addr = 0x%X%08X, length = 0x%X%08X, type = 0x%X\n",
               (uint32_t)mmap->base_addr_high, (uint32_t)mmap->base_addr_low,
               (uint32_t)mmap->length_high, (uint32_t)mmap->length_low,
               (uint32_t)mmap->type);
    }
}

void init_pmm()
{
    uint32_t mmap_addr = glb_mboot_ptr->mmap_addr;
    uint32_t mmap_length = glb_mboot_ptr->mmap_length;

    for (mmap_entry_t *mmap = (mmap_entry_t *)mmap_addr; (uint32_t)mmap < mmap_addr + mmap_length; ++mmap)
    {
        if ((mmap->type == 1) && (mmap->base_addr_low == 0x100000))
        {
            uint32_t page_addr = (mmap->base_addr_low + (kern_end - kern_start) + PMM_PAGE_SIZE - 1) & PHY_PAGE_MASK;
            uint32_t phy_page_end = (mmap->base_addr_low + mmap->length_low) & PHY_PAGE_MASK;

            for (; (page_addr < phy_page_end) && (page_addr < PMM_MAX_SIZE); page_addr += PMM_PAGE_SIZE)
            {
                pmm_free_page(page_addr);
                phy_page_count++;
            }
        }
    }
}

uint32_t pmm_alloc_page()
{
    assert(pmm_stack_top != 0, "out of memory");

    uint32_t page = pmm_stack[pmm_stack_top--];
    return page;
}

void pmm_free_page(uint32_t p)
{
    assert(pmm_stack_top != PAGE_MAX_SIZE, "out of pmm_stack stack");
    assert((p & 0x0FFF) == 0, "page base addr are not alignment");

    pmm_stack[++pmm_stack_top] = p;
}