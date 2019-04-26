#include "keyboard.h"
#include "idt.h"
#include "debug.h"

typedef struct
{
    int8_t buf[INPUT_BUF];
    uint32_t r; // Read index
    uint32_t w; // Write index
    uint32_t e; // Edit index
} input;

static input kbd_buf;

void kbd_callback(pt_regs *regs)
{
    uint32_t st, data, c;

    st = inb(KB_CTR_PORT);
    if ((st & KB_DIB) == 0)
        printk("Data has not parpered!\n");
    data = inb(KB_DATA_PORT);

    if (kbd_buf.w == kbd_buf.r)
        printk("buffer is full!");

    else
    {
        kbd_buf.buf[kbd_buf.w++] = data;
        kbd_buf.w %= INPUT_BUF;
    }
}

void init_kbd()
{
    register_interrupt_handler((uint8_t)IRQ1, kbd_callback);

    kbd_buf.w = 1;
    kbd_buf.r = 0;
}

uint8_t get_scan_code()
{
    kbd_buf.r %= INPUT_BUF;
    if (kbd_buf.r == (kbd_buf.w - 1))
        return -1;
    else
        return kbd_buf.buf[kbd_buf.r++];
}