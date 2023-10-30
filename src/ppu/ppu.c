#include "ppu.h"
#include "color.h"
#include <stdlib.h>

static void
clear_screen (struct Ppu_t* ppu)
{
    if (!ppu)
        exit(EXIT_FAILURE);
    
    for (size_t i = 0; i < ppu->sizeFrameBuffer; i++) {         //clear the frameBuffer
        ppu->frameBuffer[i] = get_complementary(ppu->mainColor);
    }

    //clear the renderer
    //SDL_SetRenderDrawColor(ppu->render, (get_complementary(ppu->mainColor)  & 0xFF0000) >> 16, (get_complementary(ppu->mainColor) & 0xFF00) >> 8, (get_complementary(ppu->mainColor) & 0xFF), 255);
    //SDL_RenderClear(ppu->render);

    return;
}

static void
update_screen (struct Ppu_t* ppu)
{
    if (!ppu)
        exit(EXIT_FAILURE);
    
    SDL_Rect pixel = {.x = 0, .y = 0, .w = PIXEL_WIDTH, .h = PIXEL_HEIGHT};

    for (size_t i = 0; i < ppu->sizeFrameBuffer; i++) {
        //SDL_SetRenderDrawColor(ppu->render, (ppu->frameBuffer[i]  & 0xFF0000) >> 16, (ppu->frameBuffer[i] & 0xFF00) >> 8, (ppu->frameBuffer[i] & 0xFF), 255);
        //SDL_RenderFillRect(ppu->render, &pixel);

        //Set the next position of the pixel rect on the screen then (i+1)
        if (i != 0 && i % SCREEN_WIDTH == 0) {
            pixel.x = 0;
            pixel.y += PIXEL_HEIGHT;
        } else {
            pixel.x += PIXEL_WIDTH;
        }
    }

    SDL_RenderPresent(ppu->render);
    return;
}

static void
set_color(struct Ppu_t* ppu, uint32_t newColor)
{
    if (!ppu)
        exit(EXIT_FAILURE);
    
    ppu->mainColor = newColor;
}

Ppu_t*
ppu_initialize()
{
    Ppu_t* ppu = malloc(sizeof(Ppu_t));
    if (!ppu)
        exit(EXIT_FAILURE);
    
    ppu->mainColor = PRIMARY_COLOR;
    ppu->windowOpen = true;
    ppu->sizeFrameBuffer = SCREEN_HEIGHT * SCREEN_WIDTH;

    for (size_t i = 0; i < ppu->sizeFrameBuffer; i++) {
        ppu->frameBuffer[i] = get_complementary(ppu->mainColor);
    }
    
    ppu->set_color = &set_color;
    ppu->clear_screen = &clear_screen;
    ppu->update_screen = &update_screen;

    ppu->window = SDL_CreateWindow("CHIP8_emu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * PIXEL_WIDTH, SCREEN_HEIGHT * PIXEL_HEIGHT, SDL_WINDOW_SHOWN);
    if (!ppu->window) {
        free(ppu);
        exit(EXIT_FAILURE);
    }

    ppu->render = SDL_CreateRenderer(ppu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!ppu->render) {
        SDL_DestroyWindow(ppu->window);
        free(ppu);
        exit(EXIT_FAILURE);
    }

    return ppu;
}

void
ppu_destroy(Ppu_t* ppu)
{
    if (ppu) {
        SDL_DestroyRenderer(ppu->render);
        SDL_DestroyWindow(ppu->window);
    }

    free(ppu);
}

void ppu_test_screen(Ppu_t* ppu)
{
   for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
    for (size_t x = 0; x < SCREEN_WIDTH; x++) {
        if ((y * SCREEN_WIDTH + x) % 2 == 0)
            ppu->frameBuffer[y * SCREEN_WIDTH + x] = ppu->mainColor;
    }
   }
}
