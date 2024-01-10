#include "cpu.h"
#include "opcode.h"
#include "input.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>


Cpu_t*
cpu_initialize(Bus_t* bus)
{
    if (!bus)
        exit(EXIT_FAILURE);
    
    Cpu_t* cpu = malloc(sizeof(Cpu_t));
    if (!cpu)
        exit(EXIT_FAILURE);

    for(size_t i = 0; i < 0x10; i++) {
        cpu->V[i] = 0;
    }

    for (size_t i = 0; i < 16; i++) {
        cpu->STACK[i] = 0;
    }
    
    cpu->instruction = 0;
    cpu->PC = MEMORY_PROGRAM_START;
    cpu->SP = 0;
    cpu->INDEX = 0;
    cpu->delay_timer = 0;
    cpu->sound_timer = 0;
    cpu->bus = bus;

    cpu->fetch_instruction = &cpu_fetch_instruction;
    cpu->execute_instruction = &cpu_execute_instruction;
    cpu->decrease_timer = &cpu_decrease_timer;

    return cpu;
}

void
cpu_destroy(Cpu_t* cpu)
{
    free(cpu);
}

static void
cpu_fetch_instruction(Cpu_t* cpu)
{
    cpu->instruction = (cpu->bus->read(cpu->bus, cpu->PC++) << 8); //high byte first BIG ENDIAN
    cpu->instruction += cpu->bus->read(cpu->bus, cpu->PC++); //low byte and move PC to the next instruction
}

static void
cpu_decrease_timer(Cpu_t* cpu)
{
    if (!cpu)
        exit(EXIT_FAILURE);
    
    if (cpu->sound_timer) {
        cpu->sound_timer--;
    }
    
    if (cpu->delay_timer)
        cpu->delay_timer--;
}

static void
cpu_execute_instruction(Cpu_t* cpu, Ppu_t* ppu)
{
    uint8_t nibble1 = (cpu->instruction & 0xF000) >> 12;
    uint8_t nibble2 = (cpu->instruction & 0x0F00) >> 8;
    uint8_t nibble3 = (cpu->instruction & 0x00F0) >> 4;
    uint8_t nibble4 = (cpu->instruction & 0x000F);

    switch(nibble1)
    {
        case 0x0:
                    cpu_execute_instruction_0(cpu, ppu);
                    break;

        case 0x1:   //JP nnn
                    JP(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0x2:   //CALL nnn
                    CALL(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0x3:   //SE Vx, kk
                    SE_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x4:   //SNE Vx, kk
                    SNE_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x5:   //SE Vx, Vy
                    SE(cpu, nibble2, nibble3);
                    break;

        case 0x6:   //LD Vx, kk
                    LD_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x7:   //ADD Vx, kk
                    ADD_n8(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x8:
                    cpu_execute_instruction_8(cpu, nibble2, nibble3, nibble4);
                    break;

        case 0x9:   //SNE Vx, Vy
                    SNE(cpu, nibble2, nibble3);
                    break;

        case 0xA:   //LD I, address
                    LD(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0xB:   //JP V0, address
                    JP_v0(cpu, (nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0xC:   //RND Vx, kk
                    RND(cpu, nibble2, (nibble3 << 4) + nibble4);
                    break;
                    
        case 0xD:   //DRW Vx, Vy, nibble
                    DRW(cpu, ppu, nibble2, nibble3, nibble4);
                    break;

        case 0xE:
                    cpu_execute_instruction_E(cpu, nibble2, nibble3, nibble4);
                    break;

        case 0xF:
                    cpu_execute_instruction_F(cpu, nibble2, nibble3, nibble4);
                    break;

        default: break;
    }
}

static void
cpu_execute_instruction_0(Cpu_t* cpu, Ppu_t* ppu)
{
    if (cpu->instruction == 0x00E0)
        CLS(ppu);
    else if (cpu->instruction == 0x00EE)
        RET(cpu);
}

static void
cpu_execute_instruction_8(Cpu_t* cpu, uint8_t nibble2, uint8_t nibble3, uint8_t nibble4)
{
    switch(nibble4)
    {
        case 0x0:   //LD Vx, Vy
                    LD_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x1:   //OR Vx, Vy
                    OR(cpu, nibble2, nibble3);
                    break;

        case 0x2:   //AND Vx, Vy
                    AND(cpu, nibble2, nibble3);
                    break;

        case 0x3:   //XOR Vx, Vy
                    XOR(cpu, nibble2, nibble3);
                    break;

        case 0x4:   //ADD Vx, Vy
                    ADD_x_y(cpu, nibble2, nibble3);
                    break;

        case 0x5:   //SUB Vx, Vy
                    SUB(cpu, nibble2, nibble3);
                    break;

        case 0x6:   //SHR Vx {, Vy}
                    SHR(cpu, nibble2);
                    break;

        case 0x7:   //SUBN Vx, Vy
                    SUBN(cpu, nibble2, nibble3);
                    break;

        case 0xE:   //SHL Vx {, Vy}
                    SHL(cpu, nibble2);
                    break;

        default: break;
    }
}

static void
cpu_execute_instruction_E(Cpu_t* cpu, const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4)
{
    if ((nibble3 << 4) + nibble4 == 0x9E) {
        SKP(cpu, nibble2);

    } else if ((nibble3 << 4) + nibble4 == 0xA1) {
        SKNP(cpu, nibble2);
    }
}

static void
cpu_execute_instruction_F(Cpu_t* cpu, const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4)
{
    switch((nibble3 << 4) + nibble4)
    {
        case 0x07:  //LD Vx, DT
                    LD_x_dt(cpu, nibble2);
                    break;

        case 0x0A:  //LD Vx, K
                    LD_k(cpu, nibble2);
                    break;

        case 0x15:  //LD DT, Vx
                    LD_dt_x(cpu, nibble2);
                    break;

        case 0x18:  //LD ST, Vx
                    LD_st(cpu, nibble2);
                    break;

        case 0x1E:  //ADD I, Vx
                    ADD(cpu, nibble2);
                    break;

        case 0x29:  //LD F, Vx
                    LD_f(cpu, nibble2);
                    break;

        case 0x33:  //LD B, Vx
                    LD_b(cpu, nibble2);
                    break;

        case 0x55:  //LD [I], Vx
                    LD_mem_x(cpu, nibble2);
                    break;

        case 0x65:  //LD Vx, [I]
                    LD_x_mem(cpu, nibble2);
                    break;

        default: break;
    }
}

static void
CLS(Ppu_t* ppu)
{
    ppu->clear_screen(ppu);
}

static void
RET(Cpu_t* cpu)
{
    cpu->SP = (cpu->SP - 1) % 16;
    cpu->PC = cpu->STACK[cpu->SP];
}

static void
JP(Cpu_t* cpu, uint16_t address)
{
    cpu->PC = address;
}

static void
CALL(Cpu_t* cpu, uint16_t address)
{
    cpu->STACK[cpu->SP] = cpu->PC;
    cpu->SP = (cpu->SP + 1) % 16;
    cpu->PC = address;
}

static void
SE_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    if (cpu->V[x] == n8)
        cpu->PC = (cpu->PC + 2) % MEMORY_SIZE;
}

static void
SNE_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    if (cpu->V[x] != n8)
        cpu->PC = (cpu->PC + 2) % MEMORY_SIZE;
}

static void
SE(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (cpu->V[x] == cpu->V[y])
        cpu->PC = (cpu->PC + 2) % MEMORY_SIZE;
}

static void
LD_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    cpu->V[x] = n8;
}

static void
ADD_n8(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    cpu->V[x] += n8;
}

static void
LD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    cpu->V[x] = cpu->V[y];
}

static void
OR(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    cpu->V[x] |= cpu->V[y];
}

static void
AND(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    cpu->V[x] &= cpu->V[y];
}

static void
XOR(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    cpu->V[x] ^= cpu->V[y];
}

static void
ADD_x_y(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if ((cpu->V[x] + cpu->V[y]) & 0x100)
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;

    cpu->V[x] += cpu->V[y];
}

static void
SUB(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (cpu->V[x] > cpu->V[y])
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] -= cpu->V[y];
}

static void
SHR(Cpu_t* cpu, uint8_t x)
{
    if (cpu->V[x] & 1)
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] >>= 1; //divided by 2
}

static void
SUBN(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (cpu->V[y] > cpu->V[x])
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] -= cpu->V[x];
}

static void
SHL(Cpu_t* cpu, uint8_t x)
{
    if (cpu->V[x] & 0x80)
        cpu->V[0xF] = 1;
    else
        cpu->V[0xF] = 0;
    
    cpu->V[x] <<= 1; //mult by 2
}

static void
SNE(Cpu_t* cpu, uint8_t x, uint8_t y)
{
    if (cpu->V[x] != cpu->V[y])
        cpu->PC = (cpu->PC + 2) % MEMORY_SIZE;
}

static void
LD(Cpu_t* cpu, uint16_t address)
{
    cpu->INDEX = address;
}

static void
JP_v0(Cpu_t* cpu, uint16_t address)
{
    cpu->PC = (address + cpu->V[0]) % MEMORY_SIZE;
}

static void
RND(Cpu_t* cpu, uint8_t x, uint8_t n8)
{
    srand(time(NULL));

    cpu->V[x] = (rand() %0x100) & n8;
}

static void
DRW(Cpu_t* cpu, Ppu_t*ppu, uint8_t x, uint8_t y, uint8_t n4)
{
    cpu->V[0xF] = 0;
    uint8_t positionX = cpu->V[x] % WIDTH;
    uint8_t positionY = cpu->V[y] % HEIGHT;
    

    for (size_t h = 0; h < n4; h++) {
        positionX = cpu->V[x] % WIDTH;
        for (size_t i = 0; i < 8; i++) { //browse each bit of memory[INDEX]
            if (((cpu->bus->read(cpu->bus, cpu->INDEX + h) << i) & 0x80) && positionX < WIDTH && positionY < HEIGHT) {
                if (ppu->frameBuffer[positionY * WIDTH + positionX])
                    cpu->V[0xF] = 1;
                else
                    cpu->V[0xF] = 0;

                ppu->frameBuffer[positionY * WIDTH + positionX] = !ppu->frameBuffer[positionY * WIDTH + positionX];
            }
            positionX++;
        }
        positionY++;
    }
}

static void
SKP(Cpu_t* cpu, uint8_t x)
{
    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    if (!keyboard)
        return;
    
    if (keyboard[SDL_SCANCODE_3] && cpu->V[x] == INPUT_1) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_4] && cpu->V[x] == INPUT_2) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_5] && cpu->V[x] == INPUT_3) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_6] && cpu->V[x] == INPUT_C) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_E] && cpu->V[x] == INPUT_4) {
        printf("KEY PRESSED\n");
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_R] && cpu->V[x] == INPUT_5) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_T] && cpu->V[x] == INPUT_6) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_Y] && cpu->V[x] == INPUT_D) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_D] && cpu->V[x] == INPUT_7) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_F] && cpu->V[x] == INPUT_8) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_G] && cpu->V[x] == INPUT_9) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_H] && cpu->V[x] == INPUT_E) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_C] && cpu->V[x] == INPUT_A) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_V] && cpu->V[x] == INPUT_0) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_B] && cpu->V[x] == INPUT_B) {
        cpu->PC += 2;
    }
    else if (keyboard[SDL_SCANCODE_N] && cpu->V[x] == INPUT_F) {
        cpu->PC += 2;
    }
}

static void
SKNP(Cpu_t* cpu, uint8_t x)
{
    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    if (!keyboard) {
        cpu->PC += 2;
        return;
    }
    
    if (!keyboard[SDL_SCANCODE_3] && cpu->V[x] == INPUT_1) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_4] && cpu->V[x] == INPUT_2) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_5] && cpu->V[x] == INPUT_3) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_6] && cpu->V[x] == INPUT_C) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_E] && cpu->V[x] == INPUT_4) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_R] && cpu->V[x] == INPUT_5) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_T] && cpu->V[x] == INPUT_6) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_Y] && cpu->V[x] == INPUT_D) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_D] && cpu->V[x] == INPUT_7) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_F] && cpu->V[x] == INPUT_8) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_G] && cpu->V[x] == INPUT_9) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_H] && cpu->V[x] == INPUT_E) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_C] && cpu->V[x] == INPUT_A) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_V] && cpu->V[x] == INPUT_0) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_B] && cpu->V[x] == INPUT_B) {
        cpu->PC += 2;
    }
    else if (!keyboard[SDL_SCANCODE_N] && cpu->V[x] == INPUT_F) {
        cpu->PC += 2;
    }
}

static void
LD_x_dt(Cpu_t* cpu, uint8_t x)
{
    cpu->V[x] = cpu->delay_timer;
}

static void
LD_k(Cpu_t* cpu, uint8_t x)
{
    const uint8_t* keyboard = SDL_GetKeyboardState(NULL);
    if (!keyboard) {
        cpu->PC -= 2;
        return;
    }

    if (keyboard[SDL_SCANCODE_3]) {
        cpu->V[x] = INPUT_1;
    }
    else if (keyboard[SDL_SCANCODE_4]) {
        cpu->V[x] = INPUT_2;
    }
    else if (keyboard[SDL_SCANCODE_5]) {
        cpu->V[x] = INPUT_3;
    }
    else if (keyboard[SDL_SCANCODE_6]) {
        cpu->V[x] = INPUT_C;
    }
    else if (keyboard[SDL_SCANCODE_E]) {
        cpu->V[x] = INPUT_4;
    }
    else if (keyboard[SDL_SCANCODE_R]) {
        cpu->V[x] = INPUT_5;
    }
    else if (keyboard[SDL_SCANCODE_T]) {
        cpu->V[x] = INPUT_6;
    }
    else if (keyboard[SDL_SCANCODE_Y]) {
        cpu->V[x] = INPUT_D;
    }
    else if (keyboard[SDL_SCANCODE_D]) {
        cpu->V[x] = INPUT_7;
    }
    else if (keyboard[SDL_SCANCODE_F]) {
        cpu->V[x] = INPUT_8;
    }
    else if (keyboard[SDL_SCANCODE_G]) {
        cpu->V[x] = INPUT_9;
    }
    else if (keyboard[SDL_SCANCODE_H]) {
        cpu->V[x] = INPUT_E;
    }
    else if (keyboard[SDL_SCANCODE_C]) {
        cpu->V[x] = INPUT_A;
    }
    else if (keyboard[SDL_SCANCODE_V]) {
        cpu->V[x] = INPUT_0;
    }
    else if (keyboard[SDL_SCANCODE_B]) {
        cpu->V[x] = INPUT_B;
    }
    else if (keyboard[SDL_SCANCODE_N]) {
        cpu->V[x] = INPUT_F;
    } else {
        cpu->PC -= 2;
    }
}

static void
LD_dt_x(Cpu_t* cpu, uint8_t x)
{
    cpu->delay_timer = cpu->V[x];
}

static void
LD_st(Cpu_t* cpu, uint8_t x)
{
    cpu->sound_timer = cpu->V[x];
}

static void
ADD(Cpu_t* cpu, uint8_t x)
{
    cpu->INDEX = (cpu->INDEX + cpu->V[x]) % MEMORY_SIZE;
}

static void
LD_f(Cpu_t* cpu, uint8_t x)
{
    switch (cpu->V[x] & 0xF){
        case 0x0: cpu->INDEX = 0;
                    break;
        case 0x1: cpu->INDEX = 5;
                    break;
        case 0x2: cpu->INDEX = 10;
                    break;
        case 0x3: cpu->INDEX = 15;
                    break;
        case 0x4: cpu->INDEX = 20;
                    break;
        case 0x5: cpu->INDEX = 25;
                    break;
        case 0x6: cpu->INDEX = 30;
                    break;
        case 0x7: cpu->INDEX = 35;
                    break;
        case 0x8: cpu->INDEX = 40;
                    break;
        case 0x9: cpu->INDEX = 45;
                    break;
        case 0xA: cpu->INDEX = 50;
                    break;
        case 0xB: cpu->INDEX = 55;
                    break;
        case 0xC: cpu->INDEX = 60;
                    break;
        case 0xD: cpu->INDEX = 65;
                    break;
        case 0xE: cpu->INDEX = 70;
                    break;
        case 0xF: cpu->INDEX = 75;
                    break;
        default: break;
    }
}

static void
LD_b(Cpu_t* cpu, uint8_t x)
{
    uint8_t nbr = cpu->V[x];

    for (uint8_t i = 0; i < 3; i++) {
        cpu->bus->write(cpu->bus, cpu->INDEX + 2 - i, (nbr % 10));  //no need to check if overflow Memory_size because write do it
        nbr /= 10;
    }
}

static void
LD_mem_x(Cpu_t* cpu, uint8_t x)
{
    for (uint8_t i = 0; i <= x; i++) {
        cpu->bus->write(cpu->bus, cpu->INDEX + i, cpu->V[i]);//no need to check if overflow Memory_size because write do it
    }
}

static void
LD_x_mem(Cpu_t* cpu, uint8_t x)
{
    for (uint8_t i = 0; i <= x; i++) {
        cpu->V[i] = cpu->bus->read(cpu->bus, cpu->INDEX + i); //no need to check if overflow Memory_size because write do it
    }
}