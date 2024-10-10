#ifndef RAM_H
#define RAM_H

#include "inttypes.h"
#include "stdlib.h"

typedef struct ram {
	uint8_t *mem;     // ram memory of RAM_SIZE
    size_t mem_size;
    size_t base;
} ram_t;

void init_ram(size_t ram_size);
void deinit_ram();
void ram_space_reg(ram_t * ram, size_t size, size_t base_addr);
void ram_space_free(ram_t * ram);
void ram_store(ram_t* ram, uint32_t addr, uint32_t size, uint32_t value);
uint32_t ram_load(ram_t* ram, uint32_t addr, uint32_t size);

#endif