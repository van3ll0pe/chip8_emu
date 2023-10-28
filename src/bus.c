#include "bus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * @brief Function only used for the pointer of function in the Bus_t structure.
 * Put the data into the memory at the index address
 * 
 * @param bus 
 * @param address 
 * @param data 
 */
static void
write_memory(struct Bus_t* bus, uint16_t address, uint8_t data)
{
    if (!bus)
        exit(EXIT_FAILURE);

    address %= MEMORY_SIZE;
    bus->memory[address] = data;
}

/**
 * @brief Function only used for the pointer of function in the Bus_t structure.
 * Get the value from the memory at the address given
 * 
 * @param bus 
 * @param address 
 * @return uint8_t 
 */
static uint8_t
read_memory(struct Bus_t* bus, uint16_t address)
{
    if (!bus)
        exit(EXIT_FAILURE);
    
    address %= MEMORY_SIZE;
    return bus->memory[address];
}

/**
 * @brief Initialize a pointer of structure Bus_t
 * 
 * @return Bus_t* 
 */
Bus_t*
bus_initialize(void)
{
    Bus_t* bus = malloc(sizeof(Bus_t));
    if (!bus)
        exit(EXIT_FAILURE);
    
    memset(bus->memory, 0, MEMORY_SIZE);

    bus->read = &read_memory;
    bus->write = &write_memory;

    return bus;
}

/**
 * @brief Free the pointer malloced from bus_initialize
 * 
 * @param bus 
 */
void
bus_destroy(Bus_t* bus)
{
    free(bus);
}

/**
 * @brief Get all the program instructions and put these instructions in the memory at the index 0x200
 * 
 * @param bus 
 * @param program 
 * @param program_size 
 */
void
bus_loadProgram(Bus_t* bus, uint8_t* program, uint16_t program_size)
{
    if (!bus || !program)
        exit(EXIT_FAILURE);

    for (size_t i = 0; i < program_size; i++) {
        bus->write(bus, MEMORY_PROGRAM_START + i, program[i]);
    }
}
