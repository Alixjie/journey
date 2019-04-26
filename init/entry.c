#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "time.h"
#include "pmm.h"
#include "vmm.h"
#include "keyboard.h"

// 内核初始化函数
void kern_init();

// 开启分页机制之后的 Multiboot 数据指针
multiboot_t *glb_mboot_ptr;

// 开启分页机制之后的内核栈
char kern_stack[STACK_SIZE];

// 临时页目录首址
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;

// 分页前线性地址映射
__attribute__((section(".init.data"))) pgd_t *pte_page_before = (pgd_t *)0x2000;

// 分页后线性地址映射
__attribute__((section(".init.data"))) pgd_t *pte_page_after = (pgd_t *)0x3000;

// 内核入口函数
__attribute__((section(".init.text"))) void kern_entry()
{
    // 原始页目录
    pgd_tmp[0] = (uint32_t)pte_page_before | PAGE_PRESENT | PAGE_WRITE;

    // 后续页目录
    pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_page_after | PAGE_PRESENT | PAGE_WRITE;

    for (int i = 0; i < 1024; ++i)
        pte_page_before[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;

    for (int i = 0; i < 1024; ++i)
        pte_page_after[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;

    // 设置临时页表
    asm volatile("mov %0, %%cr3"
                 :
                 : "r"(pgd_tmp));

    uint32_t reg_cr0;

    asm volatile("mov %%cr0, %0"
                 : "=r"(reg_cr0));

    reg_cr0 |= 0x80000000;

    asm volatile("mov %0, %%cr0"
                 :
                 : "r"(reg_cr0));

    // 切换内核栈
    uint32_t kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
    asm volatile("mov %0, %%esp\n\t"
                 "xor %%ebp, %%ebp"
                 :
                 : "r"(kern_stack_top));

    // 更新全局 multiboot_t 指针
    glb_mboot_ptr = (multiboot_t *)((uint32_t)temp_mboot_ptr + PAGE_OFFSET);

    kern_init();
}

void kern_init()
{
    init_debug();

    init_gdt();

    init_idt();

    init_pmm();

    console_clear();

    print_cur_status();

    asm volatile("int $0x3");
    asm volatile("int $0x4");

    //init_timer(200); // 开启中断

    asm volatile("sti");

    printk("Kernel in memory start: 0x%08x\n", kern_start);
    printk("Kernel in memory end: 0x%08x\n", kern_end);
    printk("kernel in memory used: %dKB\n\n", (kern_end - kern_start + 1023) / 1024); //取天棚

    show_memory_map();

    printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: % u\n\n", phy_page_count);

    uint32_t allc_addr = NULL;

    printk_color(rc_black, rc_yellow, "Test Physical Memory Alloc :\n");

    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_yellow, "Alloc Physical Addr: 0x%08X\n", allc_addr);

    init_vmm();

    map(pgd_kern, 1, allc_addr, PAGE_PRESENT | PAGE_USER | PAGE_WRITE);

    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_yellow, "Alloc Physical Addr: 0x%08X\n", allc_addr);

    uint32_t rec, bol;
    bol = get_mapping(pgd_kern, 1, &rec);

    if (bol)
        printk("rec = 0x%08x\n", rec);

    unmap(pgd_kern, 1);
    
    bol = get_mapping(pgd_kern, 1, &rec);

    if (bol)
        printk("rec = 0x%08x\n", rec);

    allc_addr = pmm_alloc_page();
    printk_color(rc_black, rc_yellow, "Alloc Physical Addr: 0x%08X\n", allc_addr);

    //panic("It's a test!");
    init_kbd();

    while (1)
        print_cur_status("%c", get_scan_code());
}