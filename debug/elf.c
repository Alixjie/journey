#include "common.h"
#include "elf.h"
#include "string.h"
#include "vmm.h"

elf_t elf_from_multiboot(multiboot_t *mb)
{
    elf_t elf;
    elf_section_header_t *esh = (elf_section_header_t *)mb->addr;

    uint32_t shstrtab = esh[mb->shndx].addr; // 段名 字符串表首地址

    for (int i = 0; i < mb->num; ++i)
    {
        const char *name = (const char *)(shstrtab + esh[i].name);
        if (strcmp(name, ".strtab") == TRUE)
        {
            elf.strtab = (const char *)(esh[i].addr + PAGE_OFFSET);
            elf.strtabsz = esh[i].size;
        }
        if (strcmp(name, ".symtab") == TRUE)
        {
            elf.symtab = (elf_symbol_t *)(esh[i].addr + PAGE_OFFSET);
            elf.symtabsz = esh[i].size;
        }
    }

    return elf;
}

const char *elf_lookup_symbol(uint32_t addr, elf_t *elf)
{
    for (int i = 0; i < elf->symtabsz; ++i)
    {
        if (ELF32_ST_TYPE(elf->symtab[i].info) != 0x2)
            continue;

        else if ((addr >= elf->symtab[i].value) && (addr < (elf->symtab[i].value + elf->symtab[i].size)))
            return (const char *)((uint32_t)elf->strtab + elf->symtab[i].name);
    }

    return NULL;
}