#include "cpu.hpp"

Cpu::Cpu() : Component()
{
    this->V.fill(0);
    this->instruction = 0;
    this->PC = MEMORY_PROGRAM_START;
    this->SP = 0;
    this->INDEX = 0;
    this->delay_timer = 0;
    this->sound_timer = 0;
}


Cpu::~Cpu()
{ }

void
Cpu::fetch_instruction()
{
    this->instruction = (read(this->PC++) << 8); //high byte first BIG ENDIAN
    this->instruction += read(this->PC++); //low byte and move PC to the next instruction
}

void
Cpu::decode_instruction(uint8_t& nibble1, uint8_t& nibble2, uint8_t& nibble3, uint8_t& nibble4)
{
    nibble1 = (this->instruction & 0xF000) >> 12;
    nibble2 = (this->instruction & 0x0F00) >> 8;
    nibble3 = (this->instruction & 0x00F0) >> 4;
    nibble4 = (this->instruction & 0x000F);
}

void
Cpu::execute_instruction()
{
    uint8_t nibble1;
    uint8_t nibble2;
    uint8_t nibble3;
    uint8_t nibble4;

    this->decode_instruction(nibble1, nibble2, nibble3, nibble4);

    switch(nibble1)
    {
        case 0x0:
                    this->execute_instruction_0();
                    break;

        case 0x1:   //JP nnn
                    this->JP((nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0x2:   //CALL nnn
                    this->CALL((nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0x3:   //SE Vx, kk
                    this->SE_n8(nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x4:   //SNE Vx, kk
                    this->SNE_n8(nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x5:   //SE Vx, Vy
                    this->SE(nibble2, nibble3);
                    break;

        case 0x6:   //LD Vx, kk
                    this->LD_n8(nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x7:   //ADD Vx, kk
                    this->ADD_n8(nibble2, (nibble3 << 4) + nibble4);
                    break;

        case 0x8:
                    this->execute_instruction_8(nibble2, nibble3, nibble3);
                    break;

        case 0x9:   //SNE Vx, Vy
                    this->SNE(nibble2, nibble3);
                    break;

        case 0xA:   //LD I, address
                    this->LD((nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0xB:   //JP V0, address
                    this->JP_v0((nibble2 << 8) + (nibble3 << 4) + nibble4);
                    break;

        case 0xC:   //RND Vx, kk
                    this->RND(nibble2, (nibble3 << 4) + nibble4);
                    break;
                    
        case 0xD:   //DRW Vx, Vy, nibble
                    this->DRW(nibble2, nibble3, nibble4);
                    break;

        case 0xE:
                    this->execute_instruction_E(nibble2, nibble3, nibble4);
                    break;

        case 0xF:
                    this->execute_instruction_F(nibble2, nibble3, nibble4);
                    break;

        default: break;
    }
}

void
Cpu::execute_instruction_0()
{
    if (this->instruction == 0x00E0)
        this->CLS();
    else if (this->instruction == 0x00EE)
        this->RET();
}

void
Cpu::execute_instruction_8(uint8_t nibble2, uint8_t nibble3, uint8_t nibble4)
{
    switch(nibble4)
    {
        case 0x0:   //LD Vx, Vy
                    this->LD(nibble2, nibble3);
                    break;

        case 0x1:   //OR Vx, Vy
                    this->OR(nibble2, nibble3);
                    break;

        case 0x2:   //AND Vx, Vy
                    this->AND(nibble2, nibble3);
                    break;

        case 0x3:   //XOR Vx, Vy
                    this->XOR(nibble2, nibble3);
                    break;

        case 0x4:   //ADD Vx, Vy
                    this->ADD(nibble2, nibble3);
                    break;

        case 0x5:   //SUB Vx, Vy
                    this->SUB(nibble2, nibble3);
                    break;

        case 0x6:   //SHR Vx {, Vy}
                    this->SHR(nibble2);
                    break;

        case 0x7:   //SUBN Vx, Vy
                    this->SUBN(nibble2, nibble3);
                    break;

        case 0xE:   //SHL Vx {, Vy}
                    this->SHL(nibble2);
                    break;

        default: break;
    }
}

void
Cpu::execute_instruction_E(const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4)
{
    if ((nibble3 << 4) + nibble4 == 0x9E) {
        this->SKP(nibble2);

    } else if ((nibble3 << 4) + nibble4 == 0xA1) {
        this->SKNP(nibble2);
    }
}

void
Cpu::execute_instruction_F(const uint8_t nibble2, const uint8_t nibble3, const uint8_t nibble4)
{
    switch((nibble3 << 4) + nibble4)
    {
        case 0x07:  //LD Vx, DT
                    this->LD_x_dt(nibble2);
                    break;

        case 0x0A:  //LD Vx, K
                    this->LD_k(nibble2);
                    break;

        case 0x15:  //LD DT, Vx
                    this->LD_dt_x(nibble2);
                    break;

        case 0x18:  //LD ST, Vx
                    this->LD_st(nibble2);
                    break;

        case 0x1E:  //ADD I, Vx
                    this->ADD(nibble2);
                    break;

        case 0x29:  //LD F, Vx
                    this->LD_f(nibble2);
                    break;

        case 0x33:  //LD B, Vx
                    this->LD_b(nibble2);
                    break;

        case 0x55:  //LD [I], Vx
                    this->LD_mem_x(nibble2);
                    break;

        case 0x65:  //LD Vx, [I]
                    this->LD_x_mem(nibble2);
                    break;

        default: break;
    }
}