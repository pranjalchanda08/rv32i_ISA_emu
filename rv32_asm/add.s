.align 2

.section .text
.globl _start
.globl stack_top

_start:
    la sp, stack_top
    li a0, 0x13
    li a1, 0x23
    add a2, a0, a1
    la a4, msg

.section .data
msg:
    .string "Pranjal\n"
    .string "Chanda\n"
