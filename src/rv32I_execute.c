#include "rv32i_execute.h"

const char *reg_name_list[] =
    {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0",
     "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4",
     "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

static uint32_t get_i(uint32_t wc)
{
    wc = RV_MASK_I(wc);
    return wc & (1 << 12) ? wc | 0xFFFFF000 : wc & 0x00000FFF;
}

uint32_t execute_lui(uint32_t word)
{
    rv_if_u_j_t u_word;
    u_word._wordcode_u.wordcode = word;
    printf("lui\t%s, 0x%x", reg_name_list[u_word._wordcode_u._rv_if_u.rd], RV_MASK_U(u_word._wordcode_u.wordcode));
    g_rv32i_ctx.cpu_r_u.xn[u_word._wordcode_u._rv_if_u.rd] = RV_MASK_U(u_word._wordcode_u.wordcode);
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_auipc(uint32_t word)
{
    rv_if_u_j_t u_word;
    u_word._wordcode_u.wordcode = word;
    printf("auipc\t%s, 0x%x", reg_name_list[u_word._wordcode_u._rv_if_u.rd], RV_MASK_U(u_word._wordcode_u.wordcode));
    g_rv32i_ctx.cpu_r_u.xn[u_word._wordcode_u._rv_if_u.rd] = g_rv32i_ctx.pc + RV_MASK_U(u_word._wordcode_u.wordcode);
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_jal(uint32_t word)
{
    rv_if_u_j_t u_word;
    u_word._wordcode_u.wordcode = word;
    printf("jal\t%s, 0x%lx", reg_name_list[u_word._wordcode_u._rv_if_u.rd], g_rv32i_ctx.pc + RV32_PC_JUMP);
    g_rv32i_ctx.cpu_r_u.xn[u_word._wordcode_u._rv_if_u.rd] = g_rv32i_ctx.pc + RV32_PC_JUMP;
    return g_rv32i_ctx.pc + RV_MASK_J(u_word._wordcode_u.wordcode);
}

uint32_t execute_jalr(uint32_t word)
{
    rv_if_i1_t i_word;
    i_word._wordcode_u.wordcode = word;
    printf("jalr\t%s, 0x%x(%s)", reg_name_list[i_word._wordcode_u._rv_if_i1.rd],
           get_i(i_word._wordcode_u.wordcode),
           reg_name_list[i_word._wordcode_u._rv_if_i1.rs1]);
    g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = g_rv32i_ctx.pc + RV32_PC_JUMP;
    return g_rv32i_ctx.pc + get_i(i_word._wordcode_u.wordcode) + i_word._wordcode_u._rv_if_i1.rs1;
}

uint32_t execute_alur(uint32_t word)
{
    rv_if_r_t r_word;
    r_word._wordcode_u.wordcode = word;
    switch (r_word._wordcode_u._rv_if_r.fn3)
    {
    case 0x00:
        if (r_word._wordcode_u._rv_if_r.fn7 == 0)
        {
            /* Perfrm add op */
            printf("add\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
                   reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
                   reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
                g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] + g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        }
        else
        {
            printf("sub\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
                   reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
                   reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
                g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] - g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        }
        break;
    case 0x01:
        /* SLL (rd = rs1 <<(rs2%XLEN)) */
        printf("sll\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] << g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] % RV32_XLEN_BIT;
        break;
    case 0x02:
        /* slt (rd = (rs1<rs2) ? 1: 0) */
        printf("slt\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            (int32_t)g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] < (int32_t)g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] ? 1 : 0;
        break;
    case 0x03:
        /* sltu (rd = (rs1<rs2) ? 1: 0) */
        printf("slu\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] < g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] ? 1 : 0;
        break;
    case 0x04:
        /* XOR (rd = rs1 ^ rs2) */
        printf("xor\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] ^ g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    case 0x05:
        /* srl/sra ( rd = rs1 >> (rs2%XLEN)) */
        printf("slr\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] >> g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2] % RV32_XLEN_BIT;
        break;
    case 0x06:
        /* OR rd = rs1 | rs2 */
        printf("or\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] | g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    case 0x07:
        /* and rd = rs1 & rs2 */
        printf("and\t%s, %s, %s", reg_name_list[r_word._wordcode_u._rv_if_r.rd],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs1],
               reg_name_list[r_word._wordcode_u._rv_if_r.rs2]);
        g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rd] =
            g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs1] & g_rv32i_ctx.cpu_r_u.xn[r_word._wordcode_u._rv_if_r.rs2];
        break;
    default:
        break;
    }

    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i1(uint32_t word)
{
    rv_if_i1_t i_word;
    i_word._wordcode_u.wordcode = word;
    if ((i_word._wordcode_u._rv_if_i1.fn3 == 0x01 || i_word._wordcode_u._rv_if_i1.fn3 == 0x05))
    {
        return execute_alui_i2(i_word._wordcode_u.wordcode);
    }
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
    case 0x00:
        printf("addi\t%s, %s, 0x%x", reg_name_list[i_word._wordcode_u._rv_if_i1.rd],
               reg_name_list[i_word._wordcode_u._rv_if_i1.rs1],
               i_word._wordcode_u._rv_if_i1.imm);
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode);
        break;
    case 0x02:
        /* slti (rd = (rs1<imm) ? 1: 0) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            (int32_t)g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] < (int32_t)get_i(i_word._wordcode_u.wordcode) ? 1 : 0;
        break;
    case 0x03:
        /* sltiu (rd = (rs1<imm) ? 1: 0) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] < get_i(i_word._wordcode_u.wordcode) ? 1 : 0;
        break;
    case 0x04:
        /* XORi (rd = rs1 ^ imm) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] ^ get_i(i_word._wordcode_u.wordcode);
        break;
    case 0x06:
        /* ORi rd = rs1 | imm */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] | get_i(i_word._wordcode_u.wordcode);
        break;
    case 0x07:
        /* andi rd = rs1 & imm */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] & g_rv32i_ctx.cpu_r_u.xn[get_i(i_word._wordcode_u.wordcode)];
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_alui_i2(uint32_t word)
{
    rv_if_i2_s_b_t i_word;
    i_word._wordcode_u.wordcode = word;
    switch (i_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x01:
        /* slli rd = rs1 << shamt_i */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rd_imm] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rs1] << g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i2_s_b.shamt_rs2];
        break;
    case 0x05:
        /* srli rd = rs1 << shamt_i */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rd_imm] =
            g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i2_s_b.rs1] >> g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i2_s_b.shamt_rs2];
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_csr(uint32_t word)
{
    rv_if_i1_t i_word;
    i_word._wordcode_u.wordcode = word;
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
        /* CSR not supported yet */
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_load(uint32_t word)
{
    rv_if_i1_t i_word;
    i_word._wordcode_u.wordcode = word;
    switch (i_word._wordcode_u._rv_if_i1.fn3)
    {
    case 0x00:
        /* lb (rd = mmio8(rs1+imm_1)) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     8);
        break;
    case 0x04:
        /* lbu (rd = mmio8(rs1+imm_1)) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0x00000000;
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     8);
        break;
    case 0x01:
        /* lh (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     16);
        break;
    case 0x05:
        /* lhu (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0x00000000;
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     16);
        break;
    case 0x02:
        /* lw (rd = mmio16(rs1+imm_1)) */
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] = 0xFFFFFF00;
        g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rd] |=
            ram_load(&g_dram_mem,
                     g_rv32i_ctx.cpu_r_u.xn[i_word._wordcode_u._rv_if_i1.rs1] + get_i(i_word._wordcode_u.wordcode),
                     32);
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_store(uint32_t word)
{
    rv_if_i2_s_b_t s_word;
    s_word._wordcode_u.wordcode = word;
    switch (s_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x00:
        /* sb */
        ram_store(&g_dram_mem,
                  g_rv32i_ctx.cpu_r_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + RV_MASK_S(s_word._wordcode_u.wordcode),
                  8,
                  s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0x000000FF);
        break;
    case 0x01:
        /* shw */
        ram_store(&g_dram_mem,
                  g_rv32i_ctx.cpu_r_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + RV_MASK_S(s_word._wordcode_u.wordcode),
                  16,
                  s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0x0000FFFF);
        break;
    case 0x02:
        /* sw */
        ram_store(&g_dram_mem,
                  g_rv32i_ctx.cpu_r_u.xn[s_word._wordcode_u._rv_if_i2_s_b.rs1] + RV_MASK_S(s_word._wordcode_u.wordcode),
                  32,
                  s_word._wordcode_u._rv_if_i2_s_b.shamt_rs2 & 0xFFFFFFFF);
        break;
    default:
        break;
    }
    return g_rv32i_ctx.pc + RV32_PC_JUMP;
}

uint32_t execute_branch(uint32_t word)
{
    rv_if_i2_s_b_t b_word;
    b_word._wordcode_u.wordcode = word;
    uint32_t ret_pc = g_rv32i_ctx.pc;
    switch (b_word._wordcode_u._rv_if_i2_s_b.fn3)
    {
    case 0x00:
        /* beq (pc = pc + (rs1==rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] == g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x01:
        /* bne (pc = pc + (rs1!=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] != g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x04:
        /* blt (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] < (int32_t)g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x05:
        /* bgt (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 ((int32_t)g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] >= (int32_t)g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x06:
        /* bltu (pc = pc + (rs1<rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] < g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    case 0x07:
        /* bgtu (pc = pc + (rs1>=rs2 ? imm_i : 4)*/
        ret_pc = ret_pc +
                 (g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1] >= g_rv32i_ctx.cpu_r_u.xn[b_word._wordcode_u._rv_if_i2_s_b.rs1]
                      ? RV_MASK_B(b_word._wordcode_u.wordcode)
                      : RV32_PC_JUMP);
        break;
    default:
        break;
    }
    return ret_pc;
}
