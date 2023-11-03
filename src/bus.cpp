#include "bus.hpp"

Bus::Bus()
{
    this->memory.fill(0);
}

Bus::~Bus()
{ }

uint8_t
Bus::read(uint16_t address)
{
    address %= MEMORY_SIZE; //to be sure that address is only 12 bits
    return this->memory[address];
}

void
Bus::write(uint16_t address, uint8_t data)
{
    address %= MEMORY_SIZE; //to be sure that address is only 12 bits
    this->memory[address] = data;
}