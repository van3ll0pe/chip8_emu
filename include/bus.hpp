#pragma once
#ifndef __BUS_HPP__
#define __BUS_HPP__

#include <array>
#include <stdint.h>

#define MEMORY_SIZE             0x1000
#define MEMORY_PROGRAM_START    0x200
#define MEMORY_STACK_START      0xEA0
#define MEMORY_DISPLAY_START    0xF00

class Bus {
    private:
    std::array<uint8_t, MEMORY_SIZE> memory;

    public:
    Bus();
    ~Bus();
    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t data);
};

#endif //__BUS_HPP__