#include "bus.h"
#include "ppu.h"
#include "cpu.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "SDL2/SDL.h"

int
main(int ac, char** av)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    if (ac != 2) {
        fprintf(stderr, "NO FILE GIVEN\n");
        return 1;
    }

    Bus_t* bus = bus_initialize();
    bus->load_rom(bus, av[1]);

    Cpu_t* cpu = cpu_initialize(bus);
    Ppu_t* ppu = ppu_initialize(bus);

    SDL_Event event;
    bool windowOpen = true;

    while(windowOpen) {
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT)
                windowOpen = false;
        }
        
        cpu->fetch_instruction(cpu);
        cpu->execute_instruction(cpu, ppu);
        cpu->decrease_timer(cpu);


        //set_pixels(ppu);
        ppu->update(ppu);
        ppu->display(ppu);
        SDL_Delay(5);
    }

    ppu_destroy(ppu);
    cpu_destroy(cpu);
    bus_destroy(bus);
    SDL_Quit();
    return 0;

}