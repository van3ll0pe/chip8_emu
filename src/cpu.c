#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Get the instruction stored in opcode, and move the PC register to the next instruction
 * 
 * @param cpu 
 */
static
void fetch_instruction(Cpu_t* cpu)
{
    if(!cpu)
        exit(EXIT_FAILURE);
    
    cpu->instruction = (cpu->bus->read(cpu->bus, cpu->PC++) << 8) | cpu->bus->read(cpu->bus, cpu->PC++);
}

