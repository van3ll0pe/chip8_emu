#pragma once
#ifndef __PPU_H__
#define __PPU_H__

#include "SDL2/SDL.h"
#include "bus.h"
#include <stdbool.h>

#define WIDTH 64
#define HEIGHT 32
#define PIXEL_DIM 8

#define SCREEN_WIDTH PIXEL_DIM * WIDTH
#define SCREEN_HEIGHT PIXEL_DIM * HEIGHT

typedef struct Ppu Ppu_t;

struct Ppu
{
    SDL_Window* window;
    SDL_Renderer* render;
    bool frameBuffer[WIDTH * HEIGHT];
    uint32_t mainColor;
    Bus_t* bus;

    void (*clear_screen)(Ppu_t* ppu);
    void (*display)(Ppu_t* ppu);
    void (*update)(Ppu_t* ppu);
};

Ppu_t* ppu_initialize(Bus_t* bus);
void ppu_destroy(Ppu_t* ppu);

#endif