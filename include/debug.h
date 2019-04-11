#ifndef INCLUDE_DEBUG_H
#define INCLUDE_DUBUG_H

#include "console.h"
#include "vargs.h"
#include "elf.h"

#define assert(x, info)  \
    do                   \
    {                    \
        if (!(x))        \
        {                \
            panic(info); \
        }                \
    } while (0)

#define static_assert(x) switch (x){case 0 : case (x):; }

void init_debug();

void panic(const char *msg);
// 打印当前的段存器值
void print_cur_status();

//内核打印函数
void printk(const char *format, ...);

void printk_color(real_color_t back, real_color_t fore, const char *format, ...);

#endif