#include "inttypes.h"
#include "ram.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

uint8_t *ram_mem;

static uint8_t dram_load_8(ram_t *ram, uint32_t addr)
{
    return (uint32_t)ram->mem[addr];
}
static uint16_t dram_load_16(ram_t *ram, uint32_t addr)
{
    return (uint32_t)ram->mem[addr] | (uint32_t)ram->mem[addr + 1] << 8;
}
static uint32_t dram_load_32(ram_t *ram, uint32_t addr)
{
    return (uint32_t)ram->mem[addr] |
           (uint32_t)ram->mem[addr + 1] << 8 |
           (uint32_t)ram->mem[addr + 2] << 16 |
           (uint32_t)ram->mem[addr + 3] << 24;
}

static void dram_store_8(ram_t *ram, uint32_t addr, uint8_t value)
{
    ram->mem[addr] = value;
}
static void dram_store_16(ram_t *ram, uint32_t addr, uint16_t value)
{
    ram->mem[addr] = (uint8_t)(value & 0xff);
    ram->mem[addr + 1] = (uint8_t)((value >> 8) & 0xff);
}
static void dram_store_32(ram_t *ram, uint32_t addr, uint32_t value)
{
    ram->mem[addr] = (uint8_t)(value & 0xff);
    ram->mem[addr + 1] = (uint8_t)((value >> 8) & 0xff);
    ram->mem[addr + 2] = (uint8_t)((value >> 16) & 0xff);
    ram->mem[addr + 3] = (uint8_t)((value >> 24) & 0xff);
}

void init_ram(size_t ram_size)
{
    ram_mem = (uint8_t*)malloc(ram_size);
}

void deinit_ram()
{
    free(ram_mem);
}

void ram_space_reg(ram_t *ram, size_t size, size_t base_addr)
{
    ram->mem_size = size;
    ram->base = base_addr;
    ram->mem = ram_mem;
}

void ram_space_free(ram_t *ram)
{
    ram->mem_size = 0;
    ram->base = 0;
    memset(ram->mem, 0, ram->mem_size);
}

uint32_t ram_load(ram_t *ram, uint32_t addr, uint32_t size)
{
    switch (size)
    {
    case 8:
        return dram_load_8(ram, addr);
        break;
    case 16:
        return dram_load_16(ram, addr);
        break;
    case 32:
        return dram_load_32(ram, addr);
        break;
    default:
        break;
    }
    return 1;
}

void ram_store(ram_t *ram, uint32_t addr, uint32_t size, uint32_t value)
{
    switch (size)
    {
    case 8:
        dram_store_8(ram, addr, value);
        break;
    case 16:
        dram_store_16(ram, addr, value);
        break;
    case 32:
        dram_store_32(ram, addr, value);
        break;
    default:;
    }
}

// void ram_store_ibin(FILE *mem, size_t ram_base, size_t ram_size)
// {
//     uint32_t iword;
//     while (fread(&iword, sizeof(uint32_t), 1, mem))
//     {
//         // printf("0x%x: 0x%x\n", imem_addr, iword);
//         ram_store(&g_iram_mem, imem_addr, 32, iword);
//         imem_addr+=sizeof(uint32_t);
//     }
// }
