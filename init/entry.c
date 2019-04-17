#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "time.h"
#include "pmm.h"

int kern_entry()
{
    init_debug();

    console_clear();

    print_cur_status();

    init_gdt();

    init_idt();

    console_write_color("check console function!\n", rc_black, rc_green);

    console_write_dec(32367, rc_black, rc_white);

    console_write_color("\n", rc_black, rc_white);

    console_write_hex(32367, rc_black, rc_white);

    int i = 1;
    char c = 'a';
    char s[20] = "Hello, World!";

    printk("\ntest int: %d\ntest char: %c\ntest string: %s\n", i, c, s);

    print_cur_status();

    asm volatile("int $0x3");
    asm volatile("int $0x4");

    //init_timer(200); // 开启中断

    asm volatile("sti");

    printk("Kernel in memory start: 0x%08x\n", kern_start);
    printk("Kernel in memory end: 0x%08x\n", kern_end);
    printk("kernel in memory used: %dKB", (kern_end - kern_start + 1023) / 1024); //取天棚

    //panic("It's a test!");

    return 0;
}