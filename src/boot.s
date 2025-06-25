.set MAGIC,    0x1BADB002
.set FLAGS,    (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .text
.global _start
_start:
    mov $kernel_stack, %esp
    call kmain
    cli
    hlt

.section .bss
.space 8192
kernel_stack:
