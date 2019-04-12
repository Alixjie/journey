#include "console.h"
#include "debug.h"

int kern_entry()
{
    init_debug();

    console_clear();

    print_cur_status();

    console_write_color("check console function!\n", rc_black, rc_green);

    console_write_dec(32367, rc_black, rc_white);

    console_write_color("\n", rc_black, rc_white);

    console_write_hex(32367, rc_black, rc_white);

    int i = 1;
    char c= 'a';
    char s[20] = "Hello, World!";

    printk("\ntest int: %d\ntest char: %c\ntest string: %s\n", i, c, s);

    print_cur_status();

    panic("It's a test!");

    return 0;
}