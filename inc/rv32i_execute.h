#pragma once

#include "main.h"

uint32_t execute_lui(rv_if_u_j_t u_word);
uint32_t execute_auipc(rv_if_u_j_t u_word);
uint32_t execute_jal(rv_if_u_j_t u_word);
uint32_t execute_jalr(rv_if_i1_t i_word);
uint32_t execute_load(rv_if_i1_t i_word);
uint32_t execute_csr(rv_if_i1_t i_word);
uint32_t execute_store(rv_if_i2_s_b_t s_word);
uint32_t execute_branch(rv_if_i2_s_b_t b_word);
uint32_t execute_alur(rv_if_r_t r_word);
uint32_t execute_alui_i1(rv_if_i1_t i_word);
uint32_t execute_alui_i2(rv_if_i2_s_b_t i_word);