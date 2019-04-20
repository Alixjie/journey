MBOOT_HEADER_MAGIC	equ 0x1BADB002

MBOOT_PAGE_ALIGN equ 1 << 0

MBOOT_MEM_INFO equ 1 << 1

MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

MBOOT_CHECKSUM	equ - (MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]
section .init.text

dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start]
[GLOBAL temp_mboot_ptr]
[EXTERN kern_entry]

start:
    cli

    mov esp, temp_mboot_ptr - 1
    mov ebp, 0
    and esp, 0FFFFFFF0H
    mov [temp_mboot_ptr], ebx
    call kern_entry

stop:
    hlt
    jmp stop

section .init.data

stack: times 1024 db 0 

temp_mboot_ptr: dd 0