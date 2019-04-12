#include "debug.h"

static void print_stack_trace();
static elf_t kernel_elf;

void init_debug()
{
    kernel_elf = elf_from_multiboot(glb_mboot_ptr);
}

void print_cur_status()
{
    static int level = 0;
    uint16_t cs_reg, ds_reg, es_reg, ss_reg;

    asm volatile(
        "movw %%cs, %0;"
        "movw %%ds, %1;"
        "movw %%es, %2;"
        "movw %%ss, %3;"
        : "=m"(cs_reg), "=m"(ds_reg), "=m"(es_reg), "=m"(ss_reg));

    printk("%d: machine_level: %d\n", level, (cs_reg & 0x03));

    printk("%d: cs: %x\n", level, cs_reg);
    printk("%d: ds: %x\n", level, ds_reg);
    printk("%d: ds: %x\n", level, es_reg);
    printk("%d: ds: %x\n", level, ss_reg);

    ++level;
}

void panic(const char *msg)
{
    printk("*** System panic: %s\n", msg);
    print_stack_trace();
    printk("***\n");

    while (1)
        ;
}

void print_stack_trace()
{
    uint32_t *eip_reg, *ebp_reg;

    asm volatile("mov %%ebp, %0"
                 : "=r"(ebp_reg));
    while (ebp_reg)
    {
        eip_reg = ebp_reg + 1;
        printk(" [0x%x] %s\n", *eip_reg, elf_lookup_symbol(*eip_reg, &kernel_elf));
        ebp_reg = (uint32_t *)*ebp_reg;
    }
}
