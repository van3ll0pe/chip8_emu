#pragma once
#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>
#include "bus.h"
#include "ppu.h"

typedef struct Cpu Cpu_t;

struct Cpu
{
    uint8_t V[0x10];
    
    uint16_t PC;
    uint16_t INDEX;
    uint8_t SP;
    uint16_t STACK[16];

    uint16_t instruction;

    uint8_t delay_timer;
    uint8_t sound_timer;

    Bus_t* bus;

    void (*fetch_instruction)(Cpu_t* cpu);
    void (*execute_instruction)(Cpu_t* cpu, Ppu_t* ppu);
    void (*decrease_timer)(Cpu_t* cpu);
};

Cpu_t* cpu_initialize(Bus_t* bus);
void cpu_destroy(Cpu_t* cpu);



#endif //__CPU_H__