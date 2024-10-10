#pragma once

#define _K(_x)   (_x*1024)

#define RAM_SIZE  _K(16)

#define IRAM_SIZE _K(1)
#define IRAM_BASE 0x1000

#define DRAM_SIZE _K(4)
#define DRAM_BASE 0x2000