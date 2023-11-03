#include "cpu.hpp"
#include <ctime>
#include <cstdlib>

void
Cpu::CLS()
{
    //to do when SCREEN is done
}

void
Cpu::RET()
{
    this->PC = this->read(--this->SP);
    this->PC += (this->read(--this->SP) << 8);
}

void
Cpu::JP(uint16_t address)
{
    this->PC = address;
}

void
Cpu::CALL(uint16_t address)
{
    this->write(this->SP++, (this->PC & 0xFF00) >> 8);
    this->write(this->SP++, (this->PC & 0x00FF));

    this->PC = address;
}

void
Cpu::SE_n8(uint8_t x, uint8_t n8)
{
    if (this->V[x] == n8)
        this->PC = (this->PC + 2) % MEMORY_SIZE;
}

void
Cpu::SNE_n8(uint8_t x, uint8_t n8)
{
    if (this->V[x] != n8)
        this->PC = (this->PC + 2) % MEMORY_SIZE;
}

void
Cpu::SE(uint8_t x, uint8_t y)
{
    if (this->V[x] == this->V[y])
        this->PC = (this->PC + 2) % MEMORY_SIZE;
}

void
Cpu::LD_n8(uint8_t x, uint8_t n8)
{
    this->V[x] = n8;
}

void
Cpu::ADD_n8(uint8_t x, uint8_t n8)
{
    this->V[x] += n8; //no need to check if unbound value, because it's uint8
}

void
Cpu::LD(uint8_t x, uint8_t y)
{
    this->V[x] = this->V[y];
}

void
Cpu::OR(uint8_t x, uint8_t y)
{
    this->V[x] |= this->V[y];
}

void
Cpu::AND(uint8_t x, uint8_t y)
{
    this->V[x] &= this->V[y];
}

void
Cpu::XOR(uint8_t x, uint8_t y)
{
    this->V[x] ^= this->V[y];
}

void
Cpu::ADD(uint8_t x, uint8_t y)
{
    if ((this->V[x] + this->V[y]) & 0x100)
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;

    this->V[x] += this->V[y];
}

void
Cpu::SUB(uint8_t x, uint8_t y)
{
    if (this->V[x] > this->V[y])
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;
    
    this->V[x] -= this->V[y];
}

void
Cpu::SHR(uint8_t x)
{
    if (this->V[x] & 1)
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;
    
    this->V[x] >>= 1; //divided by 2
}

void
Cpu::SUBN(uint8_t x, uint8_t y)
{
    if (this->V[y] > this->V[x])
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;
    
    this->V[x] = this->V[y] - this->V[x];
}

void
Cpu::SHL(uint8_t x)
{
    if (this->V[x] & 0x80)
        this->V[0xF] = 1;
    else
        this->V[0xF] = 0;
    
    this->V[x] <<= 1; //mult by 2
}

void
Cpu::SNE(uint8_t x, uint8_t y)
{
    if (this->V[x] != this->V[y])
        this->PC = (this->PC + 2) % MEMORY_SIZE;
}

void
Cpu::LD(uint16_t address)
{
    this->INDEX = address;
}

void
Cpu::JP_v0(uint16_t address)
{
    this->PC = (address + this->V[0]) % MEMORY_SIZE;
}

void
Cpu::RND(uint8_t x, uint8_t n8)
{
    srand(time(NULL));

    this->V[x] = (rand() %0x100) & n8;
}

void
Cpu::DRW(uint8_t x, uint8_t y, uint8_t n4)
{
    //to do when PPu is done
}

void
Cpu::SKP(uint8_t x)
{
    //to do when Input is done
}

void
Cpu::SKNP(uint8_t x)
{
    //to do when Input is done
}

void
Cpu::LD_x_dt(uint8_t x)
{
    this->V[x] = this->delay_timer;
}

void
Cpu::LD_k(uint8_t x)
{
    //to do when Input is done
}

void
Cpu::LD_dt_x(uint8_t x)
{
    this->delay_timer = this->V[x];
}

void
Cpu::LD_st(uint8_t x)
{
    this->sound_timer = this->V[x];
}

void
Cpu::ADD(uint8_t x)
{
    this->INDEX = (this->INDEX + this->V[x]) % MEMORY_SIZE;
}

void
Cpu::LD_f(uint8_t x)
{
    //to do when PPu is done
}

void
Cpu::LD_b(uint8_t x)
{
    uint8_t nbr = this->V[x];

    for (uint8_t i = 0; i < 3; i++) {
        this->write(this->INDEX + 2 - i, (nbr % 10));  //no need to check if overflow Memory_size because write do it
        nbr /= 10;
    }
}

void
Cpu::LD_mem_x(uint8_t x)
{
    for (uint8_t i = 0; i <= x; i++) {
        this->write(this->INDEX + i, this->V[i]);//no need to check if overflow Memory_size because write do it
    }
}

void
Cpu::LD_x_mem(uint8_t x)
{
    for (uint8_t i = 0; i <= x; i++) {
        this->V[i] = this->read(this->INDEX + i); //no need to check if overflow Memory_size because write do it
    }
}