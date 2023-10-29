#include "cpu.h"
#include "bus.h"
#include <stdlib.h>
#include <time.h>

void
instruction_CLS(Cpu_t* cpu)
{
    //to do
}

void
instruction_RET(Cpu_t* cpu)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->PC = cpu->bus->read(cpu->bus, MEMORY_STACK_START + (--cpu->SP)) | (cpu->bus->read(cpu->bus, MEMORY_STACK_START + (--cpu->SP)) << 8);
}

void
instruction_JP(Cpu_t* cpu, uint16_t address)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->PC = address;
}

void
instruction_CALL(Cpu_t* cpu, uint16_t address)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->bus->write(cpu->bus, MEMORY_STACK_START + cpu->SP++, (cpu->PC & 0xFF00) >> 8);
    cpu->bus->write(cpu->bus, MEMORY_STACK_START + cpu->SP++, cpu->PC & 0x00FF);
    cpu->PC = address;
}

void
instruction_SE_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[x] == n8)
        cpu->PC += 2;
}

void
instruction_SNE_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[x] != n8)
        cpu->PC += 2;
}

void
instruction_SE_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[x] == cpu->V[y])
        cpu->PC += 2;
}

void
instruction_LD_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] = n8;
}

void
instruction_ADD_x_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] += n8;
}

void
instruction_LD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] = cpu->V[y];
}

void
instruction_OR_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] |= cpu->V[y];
}

void
instruction_AND_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] &= cpu->V[y];
}

void
instruction_XOR_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] ^= cpu->V[y];
}

void
instruction_ADD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if ((cpu->V[x] + cpu->V[y]) & 0x100)
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;

    cpu->V[x] += cpu->V[y];
}

void
instruction_SUB_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);

    if (cpu->V[x] > cpu->V[y])
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] -= cpu->V[y];
}

void
instruction_SHR_x_y(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[x] & 0x1) //if the least bit of V[x] is 1 then VF is set to 1
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] >>= 1; //divided by 2
    

}

void
instruction_SUBN_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[y] > cpu->V[x])
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] = cpu->V[y] - cpu->V[x];
}

void
instruction_SHL_x_y(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[x] & 0x80)
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] <<= 1;
}

void
instruction_SNE_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->V[x] != cpu->V[y])
        cpu->PC += 2;
}

void
instruction_LD_i(Cpu_t* cpu, uint16_t nnn)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->INDEX = nnn;
}

void
instruction_JP_V0(Cpu_t* cpu, uint16_t nnn)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->PC = nnn + cpu->V[0];
}

void
instruction_RND_x_n8(Cpu_t* cpu, uint8_t x, uint8_t kk)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    srand(time(NULL));
    cpu->V[x] = (rand() % 0x100) & kk;
}

void
instruction_DRW_x_y_n8(Cpu_t* cpu, uint8_t x, uint8_t y, uint8_t n)
{
    if (!cpu)
        exit(EXIT_FAILURE);

    //to do
}

void
instruction_SKP_x(Cpu_t* cpu)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    //to do
}

void
instruction_SKNP_x(Cpu_t* cpu)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    //to do
}

void
instruction_LD_x_dt(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->V[x] = cpu->delay_timer;
}

void
instruction_LD_x_k(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    //to do
}

void
instruction_LD_dt_x(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->delay_timer = cpu->V[x];
}

void
instruction_LD_st_x(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->sound_timer = cpu->V[x];
}

void
instruction_ADD_I_x(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    cpu->INDEX += cpu->V[x];
}

void
instruction_LD_f_x(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    //to do
}

void
instruction_LD_b_x(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    uint8_t tmp_value = cpu->V[x];

    for (uint8_t i = 0; i < 3; i++) {
        cpu->bus->write(cpu->bus, cpu->INDEX + 2 - i, tmp_value % 10);
        tmp_value /= 10;
    }
}

void
instruction_mem_I_x(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    for (uint8_t i = 0; i <= x; i++) {
        cpu->bus->write(cpu->bus, cpu->INDEX + i, cpu->V[i]);
    }
}

void
instruction_x_mem_I(Cpu_t* cpu, uint8_t x)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    for (uint8_t i = 0; i <= x; i++) {
        cpu->V[i] = cpu->bus->read(cpu->bus, cpu->INDEX + i);
    }
}