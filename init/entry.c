#include "console.h"

int kern_entry()
{
    console_clear();

    console_write_color("check console function!\n", rc_black, rc_green);

    console_write_dec(32367, rc_black, rc_white);

    console_write_hex(32367, rc_black, rc_white);
    
    return 0;
}