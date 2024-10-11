#include "main.h"

void rv32_decode(uint32_t word)
{
    union type
    {
        uint32_t wordcode;
        rv_if_u_j_t u_j_word;
        rv_if_r_t r_word;
        rv_if_i1_t i1_word;
        rv_if_i2_s_b_t i2_s_b_word;
    } type_u;

    type_u.wordcode = word;

    switch (type_u.u_j_word._wordcode_u._rv_if_u.opcode)
    {
    case RV32_OPCODE_LUI:
        printf("RV32_OPCODE_LUI\n");
        g_rv32i_ctx.pc = execute_lui(type_u.u_j_word);
        break;
    case RV32_OPCODE_AUIPC:
        printf("RV32_OPCODE_AUIPC\n");
        g_rv32i_ctx.pc = execute_auipc(type_u.u_j_word);
        break;
    case RV32_OPCODE_JAL:
        printf("RV32_OPCODE_JAL\n");
        g_rv32i_ctx.pc = execute_jal(type_u.u_j_word);
        break;
    case RV32_OPCODE_JALR:
        printf("RV32_OPCODE_JALR\n");
        g_rv32i_ctx.pc = execute_jalr(type_u.i1_word);
        break;
    case RV32_OPCODE_ALUR:
        printf("RV32_OPCODE_ALUR\n");
        g_rv32i_ctx.pc = execute_alur(type_u.r_word);
        break;
    case RV32_OPCODE_ALUI:
        printf("RV32_OPCODE_ALUI\n");
        if (!(type_u.i1_word._wordcode_u._rv_if_i1.fn3 == 0x01 || type_u.i1_word._wordcode_u._rv_if_i1.fn3 == 0x05))
            g_rv32i_ctx.pc = execute_alui_i1(type_u.i1_word);
        else
            g_rv32i_ctx.pc = execute_alui_i2(type_u.i2_s_b_word);
        break;
    case RV32_OPCODE_LOAD:
        printf("RV32_OPCODE_LOAD\n");
        g_rv32i_ctx.pc = execute_load(type_u.i1_word);
        break;
    case RV32_OPCODE_STORE:
        printf("RV32_OPCODE_STORE\n");
        g_rv32i_ctx.pc = execute_store(type_u.i2_s_b_word);
        break;
    case RV32_OPCODE_BRANCH:
        printf("RV32_OPCODE_BRANCH\n");
        g_rv32i_ctx.pc = execute_branch(type_u.i2_s_b_word);
        break;
    case RV32_OPCODE_CSR:
        printf("RV32_OPCODE_CSR\n");
        g_rv32i_ctx.pc = execute_csr(type_u.i1_word);
        break;
    default:
        break;
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
        printf("[0x%08x]: [0x%08x]: ", g_rv32i_ctx.pc, wordcode);
        if (!wordcode)
        {
            printf("PC reached EOF\n");
            break;
        }
        rv32_decode(wordcode);
    }
}
