#ifndef __CPU_H__
#define __CPU_H__

#include <stdint.h>
#include "bus.h"

typedef struct Cpu_t {
    //a copy of bus to write/read on memory
    Bus_t* bus;

    //registers
    uint8_t V[0x10]; //V0 to VF
    uint16_t INDEX;
    uint16_t PC;
    uint8_t SP;

    uint8_t delay_timer;
    uint8_t sound_timer;

    uint16_t instruction;

    void (*fetch_instruction)(struct Cpu_t* cpu);
    void (*decode_instruction)(struct Cpu_t* cpu);
    void (*execute_instruction)(struct Cpu_t* cpu);
    
} Cpu_t;

#endif //__CPU_H__