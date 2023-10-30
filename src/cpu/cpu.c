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
    
    cpu->instruction = (cpu->bus->read(cpu->bus, (cpu->PC)) << 8);
    cpu->PC++;
    cpu->instruction += cpu->bus->read(cpu->bus, (cpu->PC));
    cpu->PC++;
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
        case 0x0:   //LD Vx, Vy
                    instruction_LD_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x1:   //OR Vx, Vy
                    instruction_OR_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x2:   //AND Vx, Vy
                    instruction_AND_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x3:   //XOR Vx, Vy
                    instruction_XOR_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x4:   //ADD Vx, Vy
                    instruction_ADD_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x5:   //SUB Vx, Vy
                    instruction_SUB_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x6:   //SHR Vx {, Vy}
                    instruction_SHR_x_y(cpu, nibble2);
                    break;

        case 0x7:   //SUBN Vx, Vy
                    instruction_SUBN_x_y(cpu, nibble2, nibble3);
                    break;

        case 0xE:   //SHL Vx {, Vy}
                    instruction_SHL_x_y(cpu, nibble2);
                    break;

        default: break;
    }
    return;
}

static void
execute_instruction_nibble1_E(Cpu_t* cpu, uint8_t nibble2, uint8_t nibble3, uint8_t nibble4)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    switch((nibble3 << 4) + nibble4) {
        case 0x9E:  //SKP Vx
                    instruction_SKP_x(cpu);
                    break;
        
        case 0xA1:  //SKNP Vx
                    instruction_SKNP_x(cpu);
                    break;

        default: break;
    }
    return;
}

static void
execute_instruction_nibble1_F(Cpu_t* cpu, uint8_t nibble2, uint8_t nibble3, uint8_t nibble4)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    switch ((nibble3 << 4) + nibble4) {
        case 0x07:  //LD Vx, DT
                    instruction_LD_x_dt(cpu, nibble2);
                    break;

        case 0x0A:  //LD Vx, K
                    instruction_LD_x_k(cpu, nibble2);
                    break;

        case 0x15:  //LD DT, Vx
                    instruction_LD_dt_x(cpu, nibble2);
                    break;

        case 0x18:  //LD ST, Vx
                    instruction_LD_st_x(cpu, nibble2);
                    break;

        case 0x1E:  //ADD I, Vx
                    instruction_ADD_I_x(cpu, nibble2);
                    break;

        case 0x29:  //LD F, Vx
                    instruction_LD_f_x(cpu, nibble2);
                    break;

        case 0x33:  //LD B, Vx
                    instruction_LD_b_x(cpu, nibble2);
                    break;

        case 0x55:  //LD [I], Vx
                    instruction_LD_mem_I_x(cpu, nibble2);
                    break;

        case 0x65:  //LD Vx, [I]
                    instruction_LD_x_mem_I(cpu, nibble2);
                    break;

        default: break;
    }
    return;
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

        case 0x8:   //check the nibble4 to execute the good instruction
                    execute_instruction_nibble1_8(cpu, nibble2, nibble3, nibble4);
                    break;

        case 0x9:   //SNE Vx, Vy
                    instruction_SNE_x_y(cpu, nibble2, nibble3);
                    break;

        case 0xA:   //LD I, nnn
                    instruction_LD_i(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0xB:   //JP V0, nnn
                    instruction_JP_V0(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0xC:   //RND Vx, byte
                    instruction_RND_x_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0xD:   //DRW Vx, Vy, nibble
                    instruction_DRW_x_y_n8(cpu, nibble2, nibble3, nibble4);
                    break;

        case 0xE:   //check the nibble3 and nibble4 to execute the good instruction
                    execute_instruction_nibble1_E(cpu, nibble2, nibble3, nibble4);
                    break;

        case 0xF:   //check the nibble3 and nibble4 to execute the good instruction
                    execute_instruction_nibble1_F(cpu, nibble2, nibble3, nibble4);

        default: break;
    }

    return;
}

Cpu_t*
cpu_initialize()
{
    Cpu_t* cpu = malloc(sizeof(Cpu_t));
    if (!cpu)
        exit(EXIT_FAILURE);
    
    for (uint8_t i = 0; i < 0x10; i++) {
        cpu->V[i] = 0;
    }

    cpu->INDEX = 0x0;
    cpu->SP = 0x0;
    cpu->PC = 0x200;

    cpu->fetch_instruction = &fetch_instruction;
    cpu->execute_instruction = &execute_instruction;

    return cpu;
}

void
cpu_destroy(Cpu_t* cpu)
{
    free(cpu);
}