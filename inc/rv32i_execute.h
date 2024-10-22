#pragma once

#include "main.h"

uint32_t execute_lui(uint32_t word);
uint32_t execute_jal(uint32_t word);
uint32_t execute_csr(uint32_t word);
uint32_t execute_jalr(uint32_t word);
uint32_t execute_load(uint32_t word);
uint32_t execute_alur(uint32_t word);
uint32_t execute_auipc(uint32_t word);
uint32_t execute_store(uint32_t word);
uint32_t execute_branch(uint32_t word);
uint32_t execute_alui_i1(uint32_t word);
uint32_t execute_alui_i2(uint32_t word);