#include "../../include/sdlmanager.hpp"

SdlManager::SdlManager(char* name= "default", unsigned int width = 900, unsigned int height = 600) : window(nullptr, SDL_DestroyWindow), render(nullptr, SDL_DestroyRenderer)
{
    this->window.reset(SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));
    this->render.reset(SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED));
}

SdlManager::~SdlManager()
{

}

SDL_Event
SdlManager::getEvent()
{
    return this->event;
}

SDL_Window*
SdlManager::getWindow()
{
    return this->window.get();
}

SDL_Renderer*
SdlManager::getRender()
{
    return this->render.get();
}