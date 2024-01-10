#pragma once
#ifndef __BUS_H__
#define __BUS_H__

#include <stdint.h>

#define MEMORY_SIZE             0x1000
#define MEMORY_PROGRAM_START    0x200

typedef struct Bus Bus_t;

struct Bus{
    uint8_t memory[MEMORY_SIZE];

    uint8_t (*read)(Bus_t* bus, uint16_t address);
    void (*write)(Bus_t* bus, uint16_t address, uint8_t data);
    void (*load_rom)(Bus_t* bus, char* romTitle);
};

Bus_t* bus_initialize();
void bus_destroy(Bus_t* bus);

#endif //__BUS_HPP__