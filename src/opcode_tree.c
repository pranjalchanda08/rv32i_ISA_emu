#include "rv32i_fetch_decode.h"

rv32_opcode_sub_fn_t alu_sub_list[9] = 
{
    RV32_SUB_FUNC_ENTRY(0, 0, RV_INS_TYPE_I1, execute_alui_i1_add, "addi\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(2, 0, RV_INS_TYPE_I1, execute_alui_i1_slti, "slti\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(3, 0, RV_INS_TYPE_I1, execute_alui_i1_sltiu, "sltiu\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(4, 0, RV_INS_TYPE_I1, execute_alui_i1_xori, "xori\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(6, 0, RV_INS_TYPE_I1, execute_alui_i1_ori, "ori\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(7, 0, RV_INS_TYPE_I1, execute_alui_i1_andi, "andi\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(1, 0, RV_INS_TYPE_I2, execute_alui_i2_slli, "slli\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(5, 0, RV_INS_TYPE_I2, execute_alui_i2_srli, "srli\t$rd, $rs1, $i"),
    RV32_SUB_FUNC_ENTRY(5, 32, RV_INS_TYPE_I2, execute_alui_i2_srli, "srai\t$rd, $rs1, $i"),
};

rv32_opcode_reg_t opcode_reg_list[] = 
{
    RV32_OPCODE_NO_SUB(RV32_OPCODE_JAL, RV_INS_TYPE_J, execute_jal, "jal\t$rd, $i"),
    RV32_OPCODE_NO_SUB(RV32_OPCODE_LUI, RV_INS_TYPE_U, execute_lui, "lui\t$rd, $i"),
    RV32_OPCODE_NO_SUB(RV32_OPCODE_JALR, RV_INS_TYPE_I1, execute_jalr, "jalr\t$rd, $i($r1)"),
    RV32_OPCODE_NO_SUB(RV32_OPCODE_AUIPC, RV_INS_TYPE_U, execute_auipc, "auipc\t$rd, $i"),
    RV32_OPCODE_WITH_SUB(RV32_OPCODE_ALUI, alu_sub_list, sizeof(alu_sub_list)),
};



size_t opcode_list_len = sizeof(opcode_reg_list)/sizeof(rv32_opcode_reg_t);
