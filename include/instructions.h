#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

#include "cpu.h"

void instruction_CLS(Cpu_t* cpu);
void instruction_RET(Cpu_t* cpu);
void instruction_JP(Cpu_t* cpu, uint16_t address);
void instruction_CALL(Cpu_t* cpu, uint16_t address);
void instruction_SE_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8);
void instruction_SNE_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8);
void instruction_SE_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_LD_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8);
void instruction_ADD_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8);
void instruction_LD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_OR_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_AND_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_XOR_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_ADD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_SUB_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_SHR_x_y(Cpu_t* cpu, uint8_t x);
void instruction_SUBN_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_SHL_x_y(Cpu_t* cpu, uint8_t x);
void instruction_SNE_x_y(Cpu_t* cpu, uint8_t x, uint8_t y);
void instruction_LD_i(Cpu_t* cpu, uint16_t nnn);
void instruction_JP_V0(Cpu_t* cpu, uint16_t nnn);
void instruction_RND_x_n8(Cpu_t* cpu, uint8_t x, uint8_t kk);
void instruction_DRW_x_y_n8(Cpu_t* cpu, uint8_t x, uint8_t y, uint8_t n);
void instruction_SKP_x(Cpu_t* cpu);
void instruction_SKNP_x(Cpu_t* cpu);
void instruction_LD_x_dt(Cpu_t* cpu, uint8_t x);
void instruction_LD_x_k(Cpu_t* cpu, uint8_t x);
void instruction_LD_dt_x(Cpu_t* cpu, uint8_t x);
void instruction_LD_st_x(Cpu_t* cpu, uint8_t x);
void instruction_ADD_I_x(Cpu_t* cpu, uint8_t x);
void instruction_LD_f_x(Cpu_t* cpu, uint8_t x);
void instruction_LD_b_x(Cpu_t* cpu, uint8_t x);
void instruction_LD_mem_I_x(Cpu_t* cpu, uint8_t x);
void instruction_LD_x_mem_I(Cpu_t* cpu, uint8_t x);


#endif //__INSTRUCTIONS_H__
