#pragma once

#include "main.h"


uint32_t execute_alui_i1_add (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i1_slti (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i1_sltiu (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i1_xori (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i1_ori (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i1_andi (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i2_slli (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);
uint32_t execute_alui_i2_srli (uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs3, uint32_t imm);

uint32_t execute_lui(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_jal(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_csr(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_jalr(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_load(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_alur(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_auipc(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_store(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_branch(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_alui_i1(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);
uint32_t execute_alui_i2(uint32_t word, uint32_t rd, uint32_t rs1, uint32_t rs2, uint32_t imm);