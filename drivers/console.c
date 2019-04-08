#include "common.h"
#include "console.h"

static uint16_t *video_memory_start = (uint16_t *)0xB8000;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_cursor()
{
    uint16_t cursor_location = cursor_y * 80 + cursor_x;

    outb(0x3D4, 14);
    outb(0x3D5, cursor_location >> 8);

    outb(0x3D4, 15);
    outb(0x3D5, cursor_location);
}

void console_clear()
{
    uint8_t attribute_byte = ((uint8_t)rc_black << 4) | ((uint8_t)rc_white & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    for (uint16_t i = 0; i < 80 * 25; ++i)
        video_memory_start[i] = blank;

    cursor_x = 0;
    cursor_y = 0;

    move_cursor();
}

static void scroll()
{
    uint8_t attribute_byte = ((uint8_t)rc_black << 4) | ((uint8_t)rc_white & 0x0F);
    uint16_t blank = 0x20 | (attribute_byte << 8);

    if (cursor_y >= 25)
    {
        for (uint16_t i = 0; i < 24 * 80; ++i)
            video_memory_start[i] = video_memory_start[i + 80];

        for (uint16_t i = 24 * 80; i < 25 * 80; ++i)
            video_memory_start[i] = blank;
    }

    cursor_y = 24;
}

void console_putc_color(char c, real_color_t back, real_color_t fore)
{
    uint8_t back_color = (uint8_t)back;
    uint8_t fore_color = (uint8_t)fore;

    uint8_t attribute_byte = (back_color << 4) | (fore_color & 0x0F);
    uint16_t attribute_word = attribute_byte << 8;

    

}