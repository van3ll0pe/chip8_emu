#ifndef __PPU_H__
#define __PPU_H__

#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 5
#define SCREEN_HEIGHT 4

#define PIXEL_WIDTH 20
#define PIXEL_HEIGHT 20

#define PRIMARY_COLOR 0xC100FF //default color



typedef struct Ppu_t {
    SDL_Window* window;
    SDL_Renderer* render;
    uint32_t frameBuffer[SCREEN_WIDTH * SCREEN_HEIGHT]; //the array of pixels (RGBA)
    uint32_t sizeFrameBuffer;
    uint32_t mainColor;
    bool windowOpen;


    void (*clear_screen)(struct Ppu_t* ppu);
    void (*update_screen)(struct Ppu_t* ppu);
    void (*set_color)(struct Ppu_t* ppu, uint32_t newColor);

}Ppu_t;

Ppu_t* ppu_initialize();
void ppu_destroy(Ppu_t* ppu);
void ppu_test_screen(Ppu_t* ppu);

#endif //__PPU_H__