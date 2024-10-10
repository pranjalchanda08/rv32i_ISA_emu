#include "main.h"

rv32i_ctx_t g_rv32i_ctx;
ram_t g_dram_mem;
ram_t g_iram_mem;

static void rv32_ram_attach()
{
    init_ram(RAM_SIZE);
    printf("RAM Init done\n");
    ram_space_reg(&g_iram_mem, IRAM_SIZE, IRAM_BASE);
    printf("IRAM Space Allocation done: s:0x%x, b:0x%x \n", IRAM_SIZE, IRAM_BASE);
    ram_space_reg(&g_dram_mem, DRAM_SIZE, DRAM_BASE);
    printf("DRAM Space Allocation done: s:0x%x, b:0x%x \n", DRAM_SIZE, DRAM_BASE);
}

static void rv32_ram_detach()
{
    printf("RAM De-Init done\n");
    deinit_ram();
}

static uint32_t rv32_ram_store_ibin(char const * file_path)
{
    printf("IBin Path: %s\n", file_path);
    FILE *ibin = fopen(file_path, "rb");
    if(!ibin)
        printf("Error reading bin file \n");
    else
        printf("ibin found. Reading binary....\n");
    uint32_t iword = 0;
    uint32_t imem_addr = IRAM_BASE;

    while (fread(&iword, sizeof(uint32_t), 1, ibin))
    {
        // printf("0x%x: 0x%x\n", imem_addr, iword);
        ram_store(&g_iram_mem, imem_addr, 32, iword);
        imem_addr+=sizeof(uint32_t);
    }
    printf("Reading binary Success. Max Addr: 0x%0x\n", imem_addr);
    fclose(ibin);
    return imem_addr;
}

static void rv32_ram_dump()
{
    printf("Saving RAM Dump..\n");
    FILE *mem = fopen("out/ram_dump.bin", "wb");
    uint32_t buff;
    uint32_t imem_addr = IRAM_BASE;
    while (imem_addr < RAM_SIZE)
    {
        // printf("%d, %d\n", imem_addr, imem_max);
        buff = ram_load(&g_iram_mem, imem_addr, 8);
        imem_addr++;
        fwrite(&buff, sizeof(uint8_t), 1, mem);
    }
    fclose(mem);    
}
static void rv32_cpu_reg_dump()
{
    printf("Saving REG Dump..\n");
    FILE *mem = fopen("out/reg_dump.bin", "wb");
    fwrite(&g_rv32i_ctx, sizeof(rv32i_ctx_t), 1, mem);
    fclose(mem);
}

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        printf("Instuction Binary not provided!!\n");
        exit(1);
    }
    
    printf("rv32I emu Startup \n");
    rv32_ram_attach();
    /* Read instruction and data binary and save it to the ram instance */
    rv32_ram_store_ibin(argv[1]);
    // uint32_t imem_max = rv32_ram_store_ibin("rv32_asm/add.bin");
    
    rv32_fetch(&g_iram_mem);

    rv32_ram_dump();
    rv32_cpu_reg_dump();
    rv32_ram_detach();
    return 0;
}