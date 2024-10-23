#include "main.h"

#define MAX_OUTPUT_SIZE 256

extern size_t opcode_list_len;
extern rv32_opcode_reg_t opcode_reg_list[];

const char *reg_name_list[] =
    {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0",
     "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4",
     "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

static uint32_t get_i(uint32_t wc)
{
    wc = RV_MASK_I(wc);
    return wc & (1 << 12) ? wc | 0xFFFFF000 : wc & 0x00000FFF;
}

static void decode_and_print(const char *template, uint32_t rd, uint32_t r1, uint32_t r2, uint32_t imm)
{
    char formatted[MAX_OUTPUT_SIZE];
    memset(formatted, 0, MAX_OUTPUT_SIZE); // Initialize the buffer

    const char *ptr = template;
    char *out = formatted;

    while (*ptr)
    {
        if (*ptr == '$')
        { // Found a placeholder
            if (strncmp(ptr, "$rd", 3) == 0)
            {
                out += sprintf(out, "%s", reg_name_list[rd]);
                ptr += 3;
            }
            else if (strncmp(ptr, "$r1", 3) == 0)
            {
                out += sprintf(out, "%s", reg_name_list[r1]);
                ptr += 3;
            }
            else if (strncmp(ptr, "$r2", 3) == 0)
            {
                out += sprintf(out, "%s", reg_name_list[r2]);
                ptr += 3;
            }
            else if (strncmp(ptr, "$i", 2) == 0)
            {
                out += sprintf(out, "0x%x", imm);
                ptr += 2;
            }
            else
            {
                *out++ = *ptr++; // Copy character as-is
            }
        }
        else
        {
            *out++ = *ptr++; // Copy character as-is
        }
    }

    *out = '\0'; // Null-terminate the string

    printf("%s\n", formatted); // Print the final formatted string
}

static uint32_t rd, rs1, rs2, imm;
static uint32_t fn3, fn7;

union type
{
    uint32_t wordcode;
    rv_if_u_j_t u_j_word;
    rv_if_r_t r_word;
    rv_if_i1_t i1_word;
    rv_if_i2_s_b_t i2_s_b_word;
} type_u;
void decode_type(uint32_t word, ins_type_t ins_type)
{
    switch (ins_type)
    {
    case RV_INS_TYPE_U:
        rd = type_u.u_j_word._wordcode_u._rv_if_u.rd;
        imm = RV_MASK_U(word);
        break;
    case RV_INS_TYPE_J:
        rd = type_u.u_j_word._wordcode_u._rv_if_u.rd;
        imm = RV_MASK_J(word);
        break;
    case RV_INS_TYPE_R:
        rd = type_u.r_word._wordcode_u._rv_if_r.rd;
        rs1 = type_u.r_word._wordcode_u._rv_if_r.rs1;
        rs2 = type_u.r_word._wordcode_u._rv_if_r.rs2;
        fn3 = type_u.r_word._wordcode_u._rv_if_r.fn3;
        fn7 = type_u.r_word._wordcode_u._rv_if_r.fn7;
        break;
    case RV_INS_TYPE_S:
        rs1 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rs1;
        rs2 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.shamt_rs2;
        fn3 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn3;
        imm = RV_MASK_S(word);
        break;
    case RV_INS_TYPE_B:
        rs1 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rs1;
        rs2 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.shamt_rs2;
        fn3 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn3;
        imm = RV_MASK_B(word);
        break;
    case RV_INS_TYPE_I1:
        rs1 = type_u.i1_word._wordcode_u._rv_if_i1.rs1;
        rd = type_u.i1_word._wordcode_u._rv_if_i1.rd;
        fn3 = type_u.i1_word._wordcode_u._rv_if_i1.fn3;
        imm = get_i(word);
        break;
    case RV_INS_TYPE_I2:
        rs1 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rs1;
        rd = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.rd_imm;
        fn3 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn3;
        fn7 = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.fn7_wc;
        imm = type_u.i2_s_b_word._wordcode_u._rv_if_i2_s_b.shamt_rs2;
    default:
        break;
    }
}

void rv32_decode(uint32_t word)
{
    type_u.wordcode = word;
    const char *dec_str;
    exec exec_cb;

    for (size_t i = 0; i < opcode_list_len; i++)
    {
        if (type_u.u_j_word._wordcode_u._rv_if_u.opcode == opcode_reg_list[i].code)
        {
            exec_cb = opcode_reg_list[i].exec_cb;
            if (!opcode_reg_list[i].subfn)
            {
                decode_type(word, opcode_reg_list[i].ins_type);
                dec_str = opcode_reg_list[i].dec_str;
            }
            else
            {
                for (size_t j = 0; j < opcode_reg_list[i].subfn; j++)
                {
                    decode_type(word, opcode_reg_list[i].sub_fn_list[j].ins_type);
                    if (opcode_reg_list[i].sub_fn_list[j].fn3 == fn3)
                    {
                        if ((opcode_reg_list[i].sub_fn_list[j].ins_type == RV_INS_TYPE_R || opcode_reg_list[i].sub_fn_list[j].ins_type == RV_INS_TYPE_I2))
                        {
                            if (opcode_reg_list[i].sub_fn_list[j].fn7 == fn7)
                            {
                                exec_cb = opcode_reg_list[i].sub_fn_list[j].exec_cb;
                                dec_str = opcode_reg_list[i].sub_fn_list[j].dec_str;
                            }
                            else
                                continue;
                        }
                        else
                        {
                            exec_cb = opcode_reg_list[i].sub_fn_list[j].exec_cb;
                            dec_str = opcode_reg_list[i].sub_fn_list[j].dec_str;
                        }
                    }
                }
            }

            break;
        }
        decode_and_print(dec_str, rd, rs1, rs2, imm);
        exec_cb(word, rd, rs1, rs2, imm);
    }
}

void rv32_fetch(ram_t *iram)
{
    /* Initialise PC */
    g_rv32i_ctx->pc = iram->base;
    uint32_t wordcode;
    while (1)
    {
        wordcode = ram_load(iram, g_rv32i_ctx->pc, 32);
        printf("\n[0x%08X]: [0x%08X]: ", g_rv32i_ctx->pc, wordcode);
        if (!wordcode)
        {
            printf("PC reached EOF\n");
            break;
        }
        rv32_decode(wordcode);
    }
}
