#pragma once
#ifndef __OPCODE_H__
#define __OPCODE_H__

#include "cpu.h"

static void cpu_fetch_instruction(Cpu_t* cpu);
static void cpu_execute_instruction(Cpu_t* cpu, Ppu_t* ppu);
static void cpu_decrease_timer(Cpu_t* cpu);
static void cpu_execute_instruction_0(Cpu_t* cpu, Ppu_t* ppu);
static void cpu_execute_instruction_8(Cpu_t* cpu, const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4);
static void cpu_execute_instruction_E(Cpu_t* cpu, const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4);
static void cpu_execute_instruction_F(Cpu_t* cpu, const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4);

static void CLS(Ppu_t* ppu);
static void RET(Cpu_t* cpu);
static void JP(Cpu_t* cpu, uint16_t address);
static void CALL(Cpu_t* cpu, uint16_t address);
static void SE_n8(Cpu_t* cpu, uint8_t x, uint8_t n8);
static void SNE_n8(Cpu_t* cpu, uint8_t x, uint8_t n8);
static void SE(Cpu_t* cpu, uint8_t x, uint8_t y);
static void LD_n8(Cpu_t* cpu, uint8_t x, uint8_t n8); //LD Vx, n8
static void ADD_n8(Cpu_t* cpu, uint8_t x, uint8_t n8); //ADD Vx, byte
static void LD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y); //LD Vx, Vy
static void OR(Cpu_t* cpu, uint8_t x, uint8_t y); //OR Vx, Vy
static void AND(Cpu_t* cpu, uint8_t x, uint8_t y); //AND Vx, Vy
static void XOR(Cpu_t* cpu, uint8_t x, uint8_t y); //XOR Vx, Vy
static void ADD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y); //ADD Vx, Vy
static void SUB(Cpu_t* cpu, uint8_t x, uint8_t y); //SUB Vx, Vy
static void SHR(Cpu_t* cpu, uint8_t x); //SHR Vx {, Vy}
static void SUBN(Cpu_t* cpu, uint8_t x, uint8_t y); //SUBN Vx, Vy
static void SHL(Cpu_t* cpu, uint8_t x); //SHL Vx {, Vy}
static void SNE(Cpu_t* cpu, uint8_t x, uint8_t y);
static void LD(Cpu_t* cpu, uint16_t address); //LD I, address
static void JP_v0(Cpu_t* cpu, uint16_t address); //JP V0, addr
static void RND(Cpu_t* cpu, uint8_t x, uint8_t n8); //RND Vx, byte
static void DRW(Cpu_t* cpu,Ppu_t* ppu, uint8_t x, uint8_t y, uint8_t n4); //DRW Vx, Vy, nibble
static void SKP(Cpu_t* cpu, uint8_t x); //SKP Vx
static void SKNP(Cpu_t* cpu, uint8_t x); //SKNP Vx
static void LD_x_dt(Cpu_t* cpu, uint8_t x); //LD Vx, DT
static void LD_k(Cpu_t* cpu, uint8_t x); //LD Vx, K
static void LD_dt_x(Cpu_t* cpu, uint8_t x); //LD DT, Vx
static void LD_st(Cpu_t* cpu, uint8_t x); //LD ST, Vx
static void ADD(Cpu_t* cpu, uint8_t x); //ADD I, Vx
static void LD_f(Cpu_t* cpu, uint8_t x); //LD F, Vx
static void LD_b(Cpu_t* cpu, uint8_t x); //LD B, Vx
static void LD_mem_x(Cpu_t* cpu, uint8_t x); //LD [I], Vx
static void LD_x_mem(Cpu_t* cpu, uint8_t x); //LD Vx, [I]

#endif