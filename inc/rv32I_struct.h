#pragma once

#include "inttypes.h"
#include "platmem.h"

#define RV_MASK_U(_wc)  (uint32_t)(_wc & 0xFFFFF000)
#define RV_MASK_J(_wc)  ((int32_t)(_wc & 0x80000000) >> 11) | (_wc & 0xFF000) | ((_wc >> 9) & 0x800) | ((_wc >> 20) & 0x7FE)
#define RV_MASK_I(_wc)  ((int32_t)(_wc & 0xFFF00000) >> 20)
#define RV_MASK_S(_wc)  ((int32_t)(_wc & 0xFE000000) >> 20) | ((_wc >> 7) & 0x1F)
#define RV_MASK_B(_wc)  ((int32_t)(_wc & 0x80000000) >> 19) | ((_wc & 0x80) << 4) | ((_wc >> 20) & 0x7E0) | ((_wc >> 7) & 0x1E)

typedef enum opcode
{
    RV32_OPCODE_LOAD        = 0x03,     // 5 different Load insructions
    RV32_OPCODE_ALUI        = 0x13,     // 9 different ALU immidiate insructions
    RV32_OPCODE_AUIPC       = 0x17,
    RV32_OPCODE_STORE       = 0x23,     // 3 different Store instructions
    RV32_OPCODE_ALUR        = 0x33,     // 10 different ALU reg instructions
    RV32_OPCODE_LUI         = 0x37,
    RV32_OPCODE_BRANCH      = 0x63,     // 6 different branch instructions
    RV32_OPCODE_JALR        = 0x67,
    RV32_OPCODE_JAL         = 0x6F,
    RV32_OPCODE_CSR         = 0x73,     // 8 different CSR instructions
}_rv32_base_opcode_t;

typedef struct _rv32i_ctx
{
    union cpu_r{
        uint32_t xn[RV32_XLEN_BIT];
        struct rv32I_gpr_struct
        {
            const uint32_t zero;
            uint32_t ra; uint32_t sp; uint32_t gp; uint32_t tp;
            uint32_t t0; uint32_t t1; uint32_t t2; uint32_t s0;
            uint32_t s1; uint32_t a0; uint32_t a1; uint32_t a2;
            uint32_t a3; uint32_t a4; uint32_t a5; uint32_t a6;
            uint32_t a7; uint32_t s2; uint32_t s3; uint32_t s4;
            uint32_t s5; uint32_t s6; uint32_t s7; uint32_t s8;
            uint32_t s9; uint32_t s10; uint32_t s11; uint32_t t3;
            uint32_t t4; uint32_t t5; uint32_t t6;
        }cpu_r_s;
    }cpu_r_u;
    uint32_t pc;
}rv32i_ctx_t;

typedef struct _rv_if_u_j
{
    union _wordcode_u
    {
        uint32_t wordcode;
        struct _rv_if_u_struct
        {
            uint32_t opcode:7;
            uint32_t rd:5;
            uint32_t imm:20;
        }_rv_if_u;
    }_wordcode_u;
}rv_if_u_j_t;

typedef struct _rv_if_r
{
    union _wordcode_r
    {
        uint32_t wordcode;
        struct _rv_if_r_struct
        {
            uint32_t opcode:7;
            uint32_t rd:5;
            uint32_t fn3:3;
            uint32_t rs1:5;
            uint32_t rs2:5;
            uint32_t fn7:7;
        }_rv_if_r;
    }_wordcode_u;
}rv_if_r_t;

typedef struct _rv_if_i1
{
    union _wordcode_i1
    {
        uint32_t wordcode;
        struct _rv_if_i1_struct
        {
            uint32_t opcode:7;
            uint32_t rd:5;
            uint32_t fn3:3;
            uint32_t rs1:5;
            uint32_t imm:12;
        }_rv_if_i1;
    }_wordcode_u;
}rv_if_i1_t;

typedef struct _rv_if_i2_s_b
{
    union _wordcode_i2_s_b
    {
        uint32_t wordcode;
        struct _rv_if_i2_s_b_struct
        {
            uint32_t opcode:7;
            uint32_t rd_imm:5;
            uint32_t fn3:3;
            uint32_t rs1:5;
            uint32_t shamt_rs2:5;
            uint32_t fn7_wc:7;
        }_rv_if_i2_s_b;
    }_wordcode_u;
}rv_if_i2_s_b_t;

typedef uint32_t (*exec)(uint32_t wcode);
typedef struct rv32_opcode_reg
{
    _rv32_base_opcode_t code;
    const char* name;
    exec exec_cb;
}rv32_opcode_reg_t;

#define REG_OPCODE(_opcode, _fn)  {_opcode, #_opcode, (exec)_fn}