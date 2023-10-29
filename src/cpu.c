#include "cpu.h"
#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Get the instruction stored in opcode, and move the PC register to the next instruction
 * 
 * @param cpu 
 */
static void
fetch_instruction(Cpu_t* cpu)
{
    if(!cpu)
        exit(EXIT_FAILURE);
    
    cpu->instruction = (cpu->bus->read(cpu->bus, cpu->PC++) << 8) | cpu->bus->read(cpu->bus, cpu->PC++);
}

static void
decode_instruction(Cpu_t* cpu, uint8_t* nibble1, uint8_t* nibble2, uint8_t* nibble3, uint8_t* nibble4)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    *nibble1 = (cpu->instruction & 0xF000) >> 12;   //4 upper bits of the upper bytes
    *nibble2 = (cpu->instruction & 0x0F00) >> 8;    //4 lower bits of the upper bytes
    *nibble3 = (cpu->instruction & 0x00F0) >> 4;    //4 upper bits of the lower bytes
    *nibble4 = cpu->instruction & 0x000F;           //4 lower bits of the lower bytes

    return;
}

static void
execute_instruction_nibble1_8(Cpu_t* cpu, uint8_t nibble2, uint8_t nibble3, uint8_t nibble4)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    switch (nibble4) {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:
        default: break;
    }

}


static void
execute_instruction(Cpu_t* cpu)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    uint8_t nibble1 = 0;
    uint8_t nibble2 = 0;
    uint8_t nibble3 = 0;
    uint8_t nibble4 = 0;

    decode_instruction(cpu, &nibble1, &nibble2, &nibble3, &nibble4);

    switch(nibble1) {
        case 0x0:
        case 0x1:   //JP nnn
                    instruction_JP(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0x2:   //CALL nnn
                    instruction_CALL(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0x3:   //SE Vx, kk
                    instruction_SE_x_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x4:   //SNE Vx, kk
                    instruction_SNE_x_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x5:   //SE Vx, Vy
                    instruction_SE_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x6:   //LD Vx, kk
                    instruction_LD_x_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x7:   //ADD Vx, kk
                    instruction_ADD_x_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x8:   //
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF:
        default: break;
    }


}

