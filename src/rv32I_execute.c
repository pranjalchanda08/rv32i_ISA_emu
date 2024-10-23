#include "rv32i_execute.h"

uint32_t execute_lui(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_u_j_t u_word;
    u_word._wordcode_u.wordcode = word;
    g_rv32i_ctx->cpu_r_u.xn[u_word._wordcode_u._rv_if_u.rd] = RV_MASK_U(u_word._wordcode_u.wordcode);
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_auipc(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_u_j_t u_word;
    u_word._wordcode_u.wordcode = word;
    g_rv32i_ctx->cpu_r_u.xn[u_word._wordcode_u._rv_if_u.rd] = g_rv32i_ctx->pc + RV_MASK_U(u_word._wordcode_u.wordcode);
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_jal(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_u_j_t u_word;
    u_word._wordcode_u.wordcode = word;
    g_rv32i_ctx->cpu_r_u.xn[u_word._wordcode_u._rv_if_u.rd] = g_rv32i_ctx->pc + RV32_PC_JUMP;
    return g_rv32i_ctx->pc + RV_MASK_J(u_word._wordcode_u.wordcode);
}

uint32_t execute_jalr(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->pc + RV32_PC_JUMP;
    return g_rv32i_ctx->pc + imm + g_rv32i_ctx->cpu_r_u.xn[rs1];
}

uint32_t execute_alur(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_r_t r_word;
    r_word._wordcode_u.wordcode = word;
    switch (r_word._wordcode_u._rv_if_r.fn3)
    {
    case 0x00:
        if (r_word._wordcode_u._rv_if_r.fn7 == 0)
        {
            /* Perfrm add op */
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
                g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] + g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        }
        else
        {
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
                g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] - g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        }
        break;
    case 0x01:
        /* SLL (rd = rs1 <<(rs2%XLEN)) */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] << g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] % RV32_XLEN_BIT;
        break;
    case 0x02:
        /* slt (rd = (rs1<rs2) ? 1: 0) */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            (int32_t)g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] < (int32_t)g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] ? 1 : 0;
        break;
    case 0x03:
        /* sltu (rd = (rs1<rs2) ? 1: 0) */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] < g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] ? 1 : 0;
        break;
    case 0x04:
        /* XOR (rd = rs1 ^ rs2) */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] ^ g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    case 0x05:
        /* srl/sra ( rd = rs1 >> (rs2%XLEN)) */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] >> g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] % RV32_XLEN_BIT;
        break;
    case 0x06:
        /* OR rd = rs1 | rs2 */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] | g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    case 0x07:
        /* and rd = rs1 & rs2 */
        g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] & g_rv32i_ctx->cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    default:
        break;
    }

    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1_add(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] + imm;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i1_slti(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = (int32_t)g_rv32i_ctx->cpu_r_u.xn[rs1] < (int32_t)imm ? 1 : 0;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i1_sltiu(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] < imm ? 1 : 0;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i1_xori(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] ^ imm;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i1_ori(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] | imm;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i1_andi(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] & imm;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i2_slli(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] << imm;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}
uint32_t execute_alui_i2_srli(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm)
{
    g_rv32i_ctx->cpu_r_u.xn[rd] = g_rv32i_ctx->cpu_r_u.xn[rs1] >> imm;
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_csr(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_i1_t i_word;
    i_word._wordcode_u.wordcode = word;
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
        /* CSR not supported yet */
    default:
        break;
    }
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_load(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_i1_t i_word;
    i_word._wordcode_u.wordcode = word;
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
    case 0x00:
        /* lb (rd = mmio8(rs1+imm_1)) */
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     8);
        break;
    case 0x04:
        /* lbu (rd = mmio8(rs1+imm_1)) */
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0x00000000;
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     8);
        break;
    case 0x01:
        /* lh (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     16);
        break;
    case 0x05:
        /* lhu (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0x00000000;
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     16);
        break;
    case 0x02:
        /* lw (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx->cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     32);
        break;
    default:
        break;
    }
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_store(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_i2_s_b_t s_word;
    s_word._wordcode_u.wordcode = word;
    switch (s_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x00:
        /* sb */
        ram_store(&g_dram_mem,
                  g_rv32i_ctx->cpu_r_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + RV_MASK_S(s_word._wordcode_u.wordcode),
                  8,
                  s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0x000000FF);
        break;
    case 0x01:
        /* shw */
        ram_store(&g_dram_mem,
                  g_rv32i_ctx->cpu_r_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + RV_MASK_S(s_word._wordcode_u.wordcode),
                  16,
                  s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0x0000FFFF);
        break;
    case 0x02:
        /* sw */
        ram_store(&g_dram_mem,
                  g_rv32i_ctx->cpu_r_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + RV_MASK_S(s_word._wordcode_u.wordcode),
                  32,
                  s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0xFFFFFFFF);
        break;
    default:
        break;
    }
    return g_rv32i_ctx->pc + RV32_PC_JUMP;
}

uint32_t execute_branch(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm)
{
    rv_if_i2_s_b_t b_word;
    b_word._wordcode_u.wordcode = word;
    uint32_t ret_pc = g_rv32i_ctx->pc;
    switch (b_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x00:
        /* beq (pc = pc + (rs1==rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] == g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x01:
        /* bne (pc = pc + (rs1!=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] != g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x04:
        /* blt (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] < (int32_t)g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x05:
        /* bgt (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] >= (int32_t)g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x06:
        /* bltu (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] < g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x07:
        /* bgtu (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] >= g_rv32i_ctx->cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    default:
        break;
    }
    return ret_pc;
}
