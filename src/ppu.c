#include "ppu.h"




static void
ppu_clear_screen(Ppu_t* ppu)
{
    if (!ppu)
        exit(EXIT_FAILURE);
    

    //SDL_SetRenderDrawColor(ppu->render, (complementaryColor & 0xFF000000) >> 24, (complementaryColor & 0xFF0000) >> 16, (complementaryColor & 0xFF00) >> 8, (complementaryColor & 0xFF));

    for (size_t i = 0; i < WIDTH * HEIGHT; i++) {
        ppu->frameBuffer[i] = 0;
    }
}

static void
ppu_display(Ppu_t* ppu)
{
    if (!ppu)
        exit(EXIT_FAILURE);

    SDL_RenderPresent(ppu->render);
}

static void
ppu_update(Ppu_t* ppu)
{
    if (!ppu)
        exit(EXIT_FAILURE);
    
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            SDL_Rect pixel = {.h = PIXEL_DIM, .w = PIXEL_DIM, .x = x * PIXEL_DIM, .y = y * PIXEL_DIM};
            if (ppu->frameBuffer[y * WIDTH + x]) {
                SDL_SetRenderDrawColor(ppu->render, (ppu->mainColor & 0xFF000000) >> 24, (ppu->mainColor & 0xFF0000) >> 16, (ppu->mainColor & 0xFF00) >> 8, (ppu->mainColor & 0xFF));
                SDL_RenderFillRect(ppu->render, &pixel);
            } else {
                uint32_t cmplColor = (~ppu->mainColor) | 0xFF;
                SDL_SetRenderDrawColor(ppu->render, (cmplColor & 0xFF000000) >> 24, (cmplColor & 0xFF0000) >> 16, (cmplColor & 0xFF00) >> 8, (cmplColor & 0xFF));
                SDL_RenderFillRect(ppu->render, &pixel);
            }
        }
    }
}

Ppu_t*
ppu_initialize(Bus_t* bus)
{
    if (!bus)
        exit(EXIT_FAILURE);
    
    Ppu_t* ppu = malloc(sizeof(Ppu_t));
    if (!ppu)
        exit(EXIT_FAILURE);
    
    ppu->bus = bus;

    ppu->window = SDL_CreateWindow("CHIP8 emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!ppu->window) {
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    ppu->render = SDL_CreateRenderer(ppu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!ppu->render) {
        SDL_DestroyWindow(ppu->window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    ppu->mainColor = 0x5343FFFF;
    ppu->clear_screen = &ppu_clear_screen;
    ppu->update = &ppu_update;
    ppu->display = &ppu_display;

    for(size_t i = 0; i < WIDTH*HEIGHT; i++) {
        ppu->frameBuffer[i] = 0;
    }

    return ppu;
}

void ppu_destroy(Ppu_t* ppu)
{
    if (ppu) {
        SDL_DestroyRenderer(ppu->render);
        SDL_DestroyWindow(ppu->window);
    }
    free(ppu);
}