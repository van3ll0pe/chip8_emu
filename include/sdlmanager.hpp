#pragma once
#ifndef __SDL_MANAGER_HPP__
#define __SDL_NANAGER_HPP__

#include <SDL2/SDL.h>
#include <memory>

class SdlManager
{
    private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> render;
    SDL_Event event;

    public:
    SdlManager(char* name= "default", unsigned int width = 900, unsigned int height = 600);
    ~SdlManager();

    SDL_Event getEvent();
    SDL_Window* getWindow();
    SDL_Renderer* getRender();
};

#endif //__SDL_MANAGER_HPP__