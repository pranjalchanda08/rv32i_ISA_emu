.align 2

.section .text
.globl _start

_start:
    addi a0, zero, 0x13
    addi a1, zero, 0x23
    add a2, a0, a1
    la a4, msg

.section .data
msg:
    .string "Pranjal\n"
    .string "Chanda\n"
