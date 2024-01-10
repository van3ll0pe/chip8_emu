#include "bus.h"
#include <stdlib.h>
#include <stdio.h>

static void
bus_initialize_font(Bus_t* bus)
{
    if (!bus)
        exit(EXIT_FAILURE);
    
    //0 ascii
    bus->memory[0] = 0xF0;
    bus->memory[1] = 0x90;
    bus->memory[2] = 0x90;
    bus->memory[3] = 0x90;
    bus->memory[4] = 0xF0;

    //1 ascii
    bus->memory[5] = 0x20;
    bus->memory[6] = 0x60;
    bus->memory[7] = 0x20;
    bus->memory[8] = 0x20;
    bus->memory[9] = 0x70;

    //2 ascii
    bus->memory[10] = 0xF0;
    bus->memory[11] = 0x10;
    bus->memory[12] = 0xF0;
    bus->memory[13] = 0x80;
    bus->memory[14] = 0xF0;

    //3 ascii
    bus->memory[15] = 0xF0;
    bus->memory[16] = 0x10;
    bus->memory[17] = 0xF0;
    bus->memory[18] = 0x10;
    bus->memory[19] = 0xF0;

    //4 ascii
    bus->memory[20] = 0x90;
    bus->memory[21] = 0x90;
    bus->memory[22] = 0xF0;
    bus->memory[23] = 0x10;
    bus->memory[24] = 0x10;

    //5 ascii
    bus->memory[25] = 0xF0;
    bus->memory[26] = 0x80;
    bus->memory[27] = 0xF0;
    bus->memory[28] = 0x10;
    bus->memory[29] = 0xF0;

    //6 ascii
    bus->memory[30] = 0xF0;
    bus->memory[31] = 0x80;
    bus->memory[32] = 0xF0;
    bus->memory[33] = 0x90;
    bus->memory[34] = 0xF0;

    //7 ascii
    bus->memory[35] = 0xF0;
    bus->memory[36] = 0x10;
    bus->memory[37] = 0x20;
    bus->memory[38] = 0x40;
    bus->memory[39] = 0x40;

    //8 ascii
    bus->memory[40] = 0xF0;
    bus->memory[41] = 0x90;
    bus->memory[42] = 0xF0;
    bus->memory[43] = 0x90;
    bus->memory[44] = 0xF0;

    //9 ascii
    bus->memory[45] = 0xF0;
    bus->memory[46] = 0x90;
    bus->memory[47] = 0xF0;
    bus->memory[48] = 0x10;
    bus->memory[49] = 0xF0;

    //A ascii
    bus->memory[50] = 0xF0;
    bus->memory[51] = 0x90;
    bus->memory[52] = 0xF0;
    bus->memory[53] = 0x90;
    bus->memory[54] = 0x90;

    //B ascii
    bus->memory[55] = 0xE0;
    bus->memory[56] = 0x90;
    bus->memory[57] = 0xE0;
    bus->memory[58] = 0x90;
    bus->memory[59] = 0xE0;

    //C ascii
    bus->memory[60] = 0xF0;
    bus->memory[61] = 0x80;
    bus->memory[62] = 0x80;
    bus->memory[63] = 0x80;
    bus->memory[64] = 0xF0;

    //D ascii
    bus->memory[65] = 0xE0;
    bus->memory[66] = 0x90;
    bus->memory[67] = 0x90;
    bus->memory[68] = 0x90;
    bus->memory[69] = 0xE0;

    //E ascii
    bus->memory[70] = 0xF0;
    bus->memory[71] = 0x80;
    bus->memory[72] = 0xF0;
    bus->memory[73] = 0x80;
    bus->memory[74] = 0xF0;

    //F ascii
    bus->memory[75] = 0xF0;
    bus->memory[76] = 0x80;
    bus->memory[77] = 0xF0;
    bus->memory[78] = 0x80;
    bus->memory[79] = 0x80;
}

static void
bus_load_rom(Bus_t* bus, char* romTitle)
{
    if (!bus || !romTitle)
        exit(EXIT_FAILURE);
    
    FILE*rom = fopen(romTitle, "rb");
    fseek(rom, 0, SEEK_END);
    long romSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    fread(bus->memory + MEMORY_PROGRAM_START, 1, romSize, rom);
    fclose(rom);
}

static uint8_t
bus_read(Bus_t* bus, uint16_t address)
{
    if (!bus)
        exit(EXIT_FAILURE);

    address %= MEMORY_SIZE; //to be sure that address is only 12 bits
    return bus->memory[address];
}

static void
bus_write(Bus_t* bus, uint16_t address, uint8_t data)
{
    if (!bus)
        exit(EXIT_FAILURE);
        
    address %= MEMORY_SIZE; //to be sure that address is only 12 bits
    bus->memory[address] = data;
}

Bus_t*
bus_initialize()
{
    Bus_t* bus = (Bus_t*)malloc(sizeof(Bus_t));

    if (bus == NULL)
        exit(EXIT_FAILURE);
    

    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        bus->memory[i] = 0;
    }

    bus->read = &bus_read;
    bus->write = &bus_write;
    bus->load_rom = &bus_load_rom;

    bus_initialize_font(bus);
    
    return bus;
}

void
bus_destroy(Bus_t* bus)
{
    free(bus);
}