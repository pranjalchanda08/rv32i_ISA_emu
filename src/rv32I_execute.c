#include "rv32i_execute.h"

static uint32_t get_i(uint32_t wc)
{
    wc = __I_MASK(wc);
    if(wc & (1 << 12))
    {
        wc |= 0xFFFFF000;
    }
    else
    {
        wc &= 0xFFF;
    }
    return wc;
}

uint32_t execute_lui(_rv_if_u_j_t u_word)
{
    g_rv32i_ctx.gpr_u.xn[u_word._wordcode_u._rv_if_u.rd] = __U_MASK(u_word._wordcode_u.wordcode);
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_auipc(_rv_if_u_j_t u_word)
{
    g_rv32i_ctx.gpr_u.xn[u_word._wordcode_u._rv_if_u.rd] = g_rv32i_ctx.pc + __U_MASK(u_word._wordcode_u.wordcode);
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_jal(_rv_if_u_j_t u_word)
{
    g_rv32i_ctx.gpr_u.xn[u_word._wordcode_u._rv_if_u.rd] = g_rv32i_ctx.pc + RV32_PC_JUMP;
    return g_rv32i_ctx.pc + __J_MASK(u_word._wordcode_u.wordcode);
}

uint32_t execute_jalr(_rv_if_i1_t i_word)
{
    g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] = g_rv32i_ctx.pc + RV32_PC_JUMP;
    return g_rv32i_ctx.pc + get_i(i_word._wordcode_u.wordcode) + i_word._wordcode_u._rv_if_i1.rs1;
}

uint32_t execute_alur(_rv_if_r_t r_word)
{
    switch (r_word._wordcode_u._rv_if_r.fn3)
    {
    case 0x00:
        if (r_word._wordcode_u._rv_if_r.fn7 == 0)
        {
            /* Perfrm add op */
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
                g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] + g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        }
        else
        {
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
                g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] - g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        }
        break;
    case 0x01:
        /* SLL (rd = rs1 <<(rs2%XLEN)) */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] << g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2] % RV32_XLEN_BIT;
        break;
    case 0x02:
        /* slt (rd = (rs1<rs2) ? 1: 0) */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            (int32_t)g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] < (int32_t)g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2] ? 1 : 0;
        break;
    case 0x03:
        /* sltu (rd = (rs1<rs2) ? 1: 0) */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] < g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2] ? 1 : 0;
        break;
    case 0x04:
        /* XOR (rd = rs1 ^ rs2) */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] ^ g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    case 0x05:
        /* slr/sra ( rd = rs1 >> (rs2%XLEN)) */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] >> g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2] % RV32_XLEN_BIT;
        break;
    case 0x06:
        /* OR rd = rs1 | rs2 */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] | g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    case 0x07:
        /* and rd = rs1 & rs2 */
        g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs1] & g_rv32i_ctx.gpr_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    default:
        break;
    }

    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1(_rv_if_i1_t i_word)
{
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
    case 0x00:
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode);
        break;
    case 0x02:
        /* slti (rd = (rs1<imm) ? 1: 0) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            (int32_t)g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] < (int32_t)get_i(i_word._wordcode_u.wordcode) ? 1 : 0;
        break;
    case 0x03:
        /* sltiu (rd = (rs1<imm) ? 1: 0) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] < get_i(i_word._wordcode_u.wordcode) ? 1 : 0;
        break;
    case 0x04:
        /* XORi (rd = rs1 ^ imm) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] ^ get_i(i_word._wordcode_u.wordcode);
        break;
    case 0x06:
        /* ORi rd = rs1 | imm */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] | get_i(i_word._wordcode_u.wordcode);
        break;
    case 0x07:
        /* andi rd = rs1 & imm */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] & g_rv32i_ctx.gpr_u.xn[get_i(i_word._wordcode_u.wordcode)];
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i2(_rv_if_i2_s_b_t i_word)
{
    switch (i_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x01:
        /* slli rd = rs1 << shamt_i */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rd_imm] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rs1] << g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i2_s_b.shamt_rs2];
        break;
    case 0x05:
        /* srli rd = rs1 << shamt_i */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rd_imm] =
            g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rs1] >> g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i2_s_b.shamt_rs2];
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_csr(_rv_if_i1_t i_word)
{
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
        /* CSR not supported yet */
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_load(_rv_if_i1_t i_word)
{
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
    case 0x00:
        /* lb (rd = mmio8(rs1+imm_1)) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                      g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                      8);
        break;
    case 0x04:
        /* lbu (rd = mmio8(rs1+imm_1)) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0x00000000;
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                      g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                      8);
        break;
    case 0x01:
        /* lh (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                      g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                      16);
        break;
    case 0x05:
        /* lhu (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0x00000000;
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                      g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                      16);
        break;
    case 0x02:
        /* lw (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                      g_rv32i_ctx.gpr_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                      32);
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_store(_rv_if_i2_s_b_t s_word)
{
    switch (s_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x00:
        /* sb */
        ram_store(&g_dram_mem,
                   g_rv32i_ctx.gpr_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + __S_MASK(s_word._wordcode_u.wordcode),
                   8,
                   s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0x000000FF);
        break;
    case 0x01:
        /* shw */
        ram_store(&g_dram_mem,
                   g_rv32i_ctx.gpr_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + __S_MASK(s_word._wordcode_u.wordcode),
                   16,
                   s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0x0000FFFF);
        break;
    case 0x02:
        /* sw */
        ram_store(&g_dram_mem,
                   g_rv32i_ctx.gpr_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + __S_MASK(s_word._wordcode_u.wordcode),
                   32,
                   s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0xFFFFFFFF);
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_branch(_rv_if_i2_s_b_t b_word)
{
    uint32_t ret_pc = g_rv32i_ctx.pc;
    switch (b_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x00:
        /* beq (pc = pc + (rs1==rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] == g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? __B_MASK(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x01:
        /* bne (pc = pc + (rs1!=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] != g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? __B_MASK(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x04:
        /* blt (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] < (int32_t)g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? __B_MASK(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x05:
        /* bgt (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] >= (int32_t)g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? __B_MASK(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x06:
        /* bltu (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] < g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? __B_MASK(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x07:
        /* bgtu (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] >= g_rv32i_ctx.gpr_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? __B_MASK(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    default:
        break;
    }
    return ret_pc;
}
