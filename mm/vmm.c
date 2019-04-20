#include "vmm.h"
#include "pmm.h"
#include "string.h"

// 内核页目录区域
pgd_t pgd_kern[PGD_SIZE] __attribute__((aligned(PAGE_SIZE)));

// 内核页表区域
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_vmm()
{
    uint32_t kern_index = PGD_INDEX(PAGE_OFFSET);

    // 映射页目录项
    for (uint32_t i = 0; kern_index < kern_index + PTE_COUNT; ++i)
        pgd_kern[kern_index++] = (uint32_t)(pte_kern[i] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;

    // 映射页表项
    uint32_t *pte = (uint32_t *)pte_kern;
    for (int i = 1; i < PTE_COUNT * PTE_SIZE; ++i)
        pte[i] = i << 12 | PAGE_PRESENT | PAGE_WRITE;

    uint32_t cr3_reg = (uint32_t)pgd_kern - PAGE_OFFSET;

    register_interrupt_handler(14, page_fault);

    switch_pgd(cr3_reg);
}

void switch_pgd(uint32_t pd)
{
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(pd));
}

void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags)
{
    uint32_t va_pgd_index = PGD_INDEX(va);
    uint32_t va_pte_index = PTE_INDEX(va);

    pte_t *temp = (pte_t *)(pgd_now[va_pgd_index] & PHY_PAGE_MASK);

    if (!temp)
    {
        temp = pmm_alloc_page();
        pgd_now[va_pgd_index] = (uint32_t)temp | PAGE_PRESENT | PAGE_WRITE;

        temp = (pte_t *)((uint32_t)temp + PAGE_OFFSET);
        bzero(temp, PAGE_SIZE);
    }
    else
        temp = (pte_t *)((uint32_t)temp + PAGE_OFFSET);

    temp[va_pte_index] = (pa & PHY_PAGE_MASK) | flags;

    // 通知 CPU 更新页表缓存
    asm volatile("invlpg (%0)"
                 :
                 : "a"(va));
}

void unmap(pgd_t *pgd_now, uint32_t va)
{
    uint32_t va_pgd_index = PGD_INDEX(va);
    uint32_t va_pte_index = PTE_INDEX(va);

    pte_t *temp = (pte_t *)(pgd_now[va_pgd_index] & PHY_PAGE_MASK);

    if (!temp)
        return;
    else
    {
        temp = (pte_t *)((uint32_t)temp + PAGE_OFFSET);

        uint32_t ret = temp[va_pte_index] & PHY_PAGE_MASK;
        if (!ret)
            return;
        else
        {
            temp[va_pte_index] = 0;
            pmm_free_page(ret);

            asm volatile("invlpg (%0)"
                         :
                         : "a"(va));
        }
    }
}

uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa)
{
    uint32_t va_pgd_index = PGD_INDEX(va);
    uint32_t va_pte_index = PTE_INDEX(va);

    pte_t *temp = (pte_t *)(pgd_now[va_pgd_index] & PHY_PAGE_MASK);

    if (!temp)
        return 0;
    else
    {
        temp = (pte_t *)((uint32_t)temp + PAGE_OFFSET);

        uint32_t ret = temp[va_pte_index] & PHY_PAGE_MASK;
        if ((ret != 0) && pa)
        {
            *pa = ret;
            return 1;
        }
        return 0;
    }
}