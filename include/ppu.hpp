#pragma once
#ifndef __PPU_HPP__
#define __PPU_HPP__

#include "sdlmanager.hpp"
#include <array>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PIXEL_SIZE 8;

class Ppu
{
    private:
        SdlManager sdlManager;
        std::array<uint32_t, SCREEN_HEIGHT * SCREEN_WIDTH> frameBuffer;
        uint32_t color1;
        uint
    public:
};

#endif //__PPU_HPP__