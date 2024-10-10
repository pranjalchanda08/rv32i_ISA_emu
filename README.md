# RV32I ISA Emulator

C based RISC-V 32i ISA emulator

## Prerequisits
```sh
$ sudo apt install gcc-riscv64-unknown-elf build-essential make
```

## Build Emmulator
* Build Emulator
```sh
$ make
```
* Clean
```sh
$ make clean
```
* Buid asm
```sh
$ make asm ASM={asm_name}
```

## Run
```sh
$ make run ARGS={path_to_asm.bin}
```

## Example run

#### add.asm
```asm
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

```
#### rv32.ld
```ld
OUTPUT_ARCH("riscv")
OUTPUT_FORMAT("elf32-littleriscv")

MEMORY
{
	/* VM Addresses */
	IRAM (rx)	: ORIGIN = 0x1000, LENGTH = 1k
	DRAM (rwx)	: ORIGIN = 0x2000, LENGTH = 4k
	STACK(rwx)	: ORIGIN = 0x4000, LENGTH = 4k
}

ENTRY(_start)
SECTIONS
{
    .text : {
        *(.text)
    } > IRAM
    .data : {
        *(.data)
    } > DRAM AT> DRAM
    .stack : 
    {
        *(.stack)
    } > STACK
    stack_top = .;
    _end = .;
}
```
#### Build add.bin and rv32I_emu
```sh
$ make clean
$ make
$ make asm ASM=add
```
#### Run rv32I_emu
```sh
$ make run ARGS=build/add.bin
```

#### Output
```sh
./build/rv32I_emu build/add.bin
rv32I emu Startup
RAM Init done
IRAM Space Allocation done: s:0x400, b:0x1000
DRAM Space Allocation done: s:0x1000, b:0x2000
IBin Path: build/add.bin
ibin found. Reading binary....
Reading binary Success. Max Addr: 0x2010
[0x00001000]: [0x01300513]: RV32_OPCODE_ALUI
[0x00001004]: [0x02300593]: RV32_OPCODE_ALUI
[0x00001008]: [0x00b50633]: RV32_OPCODE_ALUR
[0x0000100c]: [0x00001717]: RV32_OPCODE_AUIPC
[0x00001010]: [0xff470713]: RV32_OPCODE_ALUI
[0x00001014]: [0x00000000]: PC reached EOF. Exiting Fetching
Saving RAM Dump..
Saving REG Dump..
RAM De-Init done
```

#### RAM Dump
```sh
$ hexdump out/ram_dump.bin
0000000 0513 0130 0593 0230 0633 00b5 1717 0000
0000010 0713 ff47 0000 0000 0000 0000 0000 0000
0000020 0000 0000 0000 0000 0000 0000 0000 0000
*
0001000 7250 6e61 616a 0a6c 4300 6168 646e 0a61     Pranjal  Chanda 
0001010 0000 0000 0000 0000 0000 0000 0000 0000
*
0003000
```

#### CPU REG Dump
```sh
hexdump out/reg_dump.bin 
0000000 0000 0000 0000 0000 0000 0000 0000 0000
*
0000020 0000 0000 0000 0000 0013 0000 0023 0000
0000030 0036 0000 0000 0000 2000 0000 0000 0000
0000040 0000 0000 0000 0000 0000 0000 0000 0000
*
0000080 1014 0000
0000084
```