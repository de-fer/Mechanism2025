#pragma once

#include <SDL3/SDL.h>

struct Texture
{
    SDL_Texture *texture;
    SDL_FPoint center, size;
};
