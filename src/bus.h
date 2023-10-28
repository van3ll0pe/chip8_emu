#ifndef __BUS_H__
#define __BUS_H__

#include <stdint.h>

#define MEMORY_SIZE             0x1000
#define MEMORY_PROGRAM_START    0x200

#define MEMORY_STACK_START      0xEA0
#define MEMORY_STACK_END        0xEFF

#define MEMORY_SCREEN_START     0xF00
#define MEMORY_SCREEN_END       0xFFF

typedef struct Bus_t {
    uint8_t memory[MEMORY_SIZE];

    void (*write)(struct Bus_t* bus, uint16_t address, uint8_t data);
    uint8_t (*read)(struct Bus_t* bus, uint16_t address);
}Bus_t;

Bus_t* bus_initialize(void);
void bus_destroy(Bus_t* bus);
void bus_loadProgram(Bus_t* bus, uint8_t* program, uint16_t program_size);

#endif //__BUS_H__