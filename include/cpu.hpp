#pragma once
#ifndef __CPU_HPP__
#define __CPU_HPP__

#include <stdint.h>
#include <array>
#include "component.hpp"


class Cpu : public Component
{
    private:
    std::array<uint8_t, 0x10> V;
    
    uint16_t PC;
    uint16_t INDEX;
    uint8_t SP;

    uint16_t instruction;

    uint8_t delay_timer;
    uint8_t sound_timer;

    public:
    Cpu();
    ~Cpu();

    void fetch_instruction();
    void execute_instruction();

    private:
    void decode_instruction(uint8_t& nibble1, uint8_t& nibble2, uint8_t& nibble3, uint8_t& nibble4);
    void execute_instruction_0();
    void execute_instruction_8(const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4);
    void execute_instruction_E(const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4);
    void execute_instruction_F(const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4);

    void CLS();
    void RET();
    void JP(uint16_t address);
    void CALL(uint16_t address);
    void SE_n8(uint8_t x, uint8_t n8);
    void SNE_n8(uint8_t x, uint8_t n8);
    void SE(uint8_t x, uint8_t y);
    void LD_n8(uint8_t x, uint8_t n8); //LD Vx, n8
    void ADD_n8(uint8_t x, uint8_t n8); //ADD Vx, byte
    void LD(uint8_t x, uint8_t y); //LD Vx, Vy
    void OR(uint8_t x, uint8_t y); //OR Vx, Vy
    void AND(uint8_t x, uint8_t y); //AND Vx, Vy
    void XOR(uint8_t x, uint8_t y); //XOR Vx, Vy
    void ADD(uint8_t x, uint8_t y); //ADD Vx, Vy
    void SUB(uint8_t x, uint8_t y); //SUB Vx, Vy
    void SHR(uint8_t x); //SHR Vx {, Vy}
    void SUBN(uint8_t x, uint8_t y); //SUBN Vx, Vy
    void SHL(uint8_t x); //SHL Vx {, Vy}
    void SNE(uint8_t x, uint8_t y);
    void LD(uint16_t address); //LD I, address
    void JP_v0(uint16_t address); //JP V0, addr
    void RND(uint8_t x, uint8_t n8); //RND Vx, byte
    void DRW(uint8_t x, uint8_t y, uint8_t n4); //DRW Vx, Vy, nibble
    void SKP(uint8_t x); //SKP Vx
    void SKNP(uint8_t x); //SKNP Vx
    void LD_x_dt(uint8_t x); //LD Vx, DT
    void LD_k(uint8_t x); //LD Vx, K
    void LD_dt_x(uint8_t x); //LD DT, Vx
    void LD_st(uint8_t x); //LD ST, Vx
    void ADD(uint8_t x); //ADD I, Vx
    void LD_f(uint8_t x); //LD F, Vx
    void LD_b(uint8_t x); //LD B, Vx
    void LD_mem_x(uint8_t x); //LD [I], Vx
    void LD_x_mem(uint8_t x); //LD Vx, [I]
};

#endif //__CPU_HPP__