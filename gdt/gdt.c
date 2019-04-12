#include "gdt.h"
#include "string.h"

// 全局描述符表长度
#define GDT_LENGTH 5

extern uint32_t stack;

// 全局描述符表定义
gdt_entry_t gdt_entries[GDT_LENGTH];

//GDTR
gdt_ptr_t gdt_ptr;

static void gdt_set_gate(int32_t num, uint32_t base,
                         uint32_t limit, uint8_t access, uint8_t gran);

// 初始化全局描述符表
void init_gdt()
{
    gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);

    //内核代码段 内核数据段
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    //用户代码段 用户数据段 后期要调整为LDT描述符和TSS描述符
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    gdt_flush((uint32_t)&gdt_ptr);
}

// 全局描述符表构造函数，根据下标构造
// 参数分别是数组下标、基地址、限长、访问标志，其它访问标志
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt_entries[num].base_low = (base & 0x0FFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0x0FF;
    gdt_entries[num].base_high = (base >> 24);

    gdt_entries[num].limit_low = (limit & 0x0FFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;
    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}