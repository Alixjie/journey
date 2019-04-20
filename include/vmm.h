#ifndef INCLUDE_VMM_H
#define INCLUDE_VMM_H

#include "types.h" 
#include "idt.h"

// 内核的偏移地址
#define PAGE_OFFSET 0xC0000000

// 位 0 是存在 (Present) 标志，用于指明表项对地址转换是否有效
#define PAGE_PRESENT 0x1

// 位 1 是读 / 写 (Read/Write) 标志
#define PAGE_WRITE 0x2

// 位 2 是用户 / 超级用户 (User/Supervisor) 标志
#define PAGE_USER 0x4

// 虚拟分页大小
#define PAGE_SIZE 4096

// 获取一个地址的页目录项
#define PGD_INDEX(x) (((x) >> 22) & 0x3FF)

// 获取一个地址的页表项
#define PTE_INDEX(x) (((x) >> 12) & 0x3FF)

// 获取一个地址的页內偏移
#define OFFSET_INDEX(x) ((x) & 0xFFF)

// 页目录数据类型
typedef uint32_t pgd_t;

// 页表数据类型
typedef uint32_t pte_t;

// 页目录成员数
#define PGD_SIZE (PAGE_SIZE/sizeof(pgd_t))

// 页表成员数
#define PTE_SIZE (PAGE_SIZE/sizeof(pte_t))

// 映射 512MB 内存所需要的页表数
#define PTE_COUNT 128

// 内核页目录区域
extern pgd_t pgd_kern[PGD_SIZE];

// 初始化虚拟内存管理
void init_vmm();

// 更换当前的页目录
void switch_pgd(uint32_t pd);

// 使用 flags 指出的页权限，把物理地址 pa 映射到虚拟地址 va
void map(pgd_t *pgd_now, uint32_t va, uint32_t pa, uint32_t flags);

// 取消虚拟地址 va 的物理映射
void unmap(pgd_t *pgd_now, uint32_t va);

// 如果虚拟地址 va 映射到物理地址则返回 1
// 同时如果 pa 不是空指针则把物理地址写入 pa 参数
uint32_t get_mapping(pgd_t *pgd_now, uint32_t va, uint32_t *pa);

// 页错误中断的函数处理
void page_fault(pt_regs *regs);

#endif