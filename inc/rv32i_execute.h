#pragma once

#include "main.h"

#define RV32_PC_JUMP    sizeof(uint32_t)
#define RV32_XLEN_BIT   32

uint32_t execute_lui(_rv_if_u_j_t u_word);
uint32_t execute_auipc(_rv_if_u_j_t u_word);
uint32_t execute_jal(_rv_if_u_j_t u_word);
uint32_t execute_jalr(_rv_if_i1_t i_word);
uint32_t execute_load(_rv_if_i1_t i_word);
uint32_t execute_csr(_rv_if_i1_t i_word);
uint32_t execute_store(_rv_if_i2_s_b_t s_word);
uint32_t execute_branch(_rv_if_i2_s_b_t b_word);
uint32_t execute_alur(_rv_if_r_t r_word);
uint32_t execute_alui_i1(_rv_if_i1_t i_word);
uint32_t execute_alui_i2(_rv_if_i2_s_b_t i_word);