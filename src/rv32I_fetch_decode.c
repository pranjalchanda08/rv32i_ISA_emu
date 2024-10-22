#include "main.h"

rv32_opcode_reg_t opcode_reg_list[] = 
{
    REG_OPCODE(RV32_OPCODE_LUI, execute_lui),
    REG_OPCODE(RV32_OPCODE_AUIPC, execute_auipc),
    REG_OPCODE(RV32_OPCODE_JAL, execute_jal),
    REG_OPCODE(RV32_OPCODE_JALR, execute_jalr),
    REG_OPCODE(RV32_OPCODE_ALUR, execute_alur),
    REG_OPCODE(RV32_OPCODE_ALUI, execute_alui_i1),
    REG_OPCODE(RV32_OPCODE_LOAD, execute_load),
    REG_OPCODE(RV32_OPCODE_STORE, execute_store),
    REG_OPCODE(RV32_OPCODE_BRANCH, execute_branch),
    REG_OPCODE(RV32_OPCODE_CSR, execute_csr),
};

size_t opcode_list_len = sizeof(opcode_reg_list)/sizeof(rv32_opcode_reg_t);

void rv32_decode(uint32_t word)
{
    union type
    {
        uint32_t wordcode;
        rv_if_u_j_t u_j_word;
    } type_u;

    type_u.wordcode = word;
    for (size_t i = 0; i < opcode_list_len; i++)
    {
        if (type_u.u_j_word._wordcode_u._rv_if_u.opcode == opcode_reg_list[i].code)
        {
            printf("%-20s: ", opcode_reg_list[i].name);
            g_rv32i_ctx.pc = opcode_reg_list[i].exec_cb(word);
            break;
        }
    }
}

void rv32_fetch(ram_t *iram)
{
    /* Initialise PC */
    g_rv32i_ctx.pc = iram->base;
    uint32_t wordcode;
    while (1)
    {
        wordcode = ram_load(iram, g_rv32i_ctx.pc, 32);
        printf("\n[0x%08X]: [0x%08X]: ", g_rv32i_ctx.pc, wordcode);
        if (!wordcode)
        {
            printf("PC reached EOF\n");
            break;
        }
        rv32_decode(wordcode);
    }
}
