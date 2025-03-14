#pragma once

#include <SDL3/SDL.h>

struct Texture
{
    SDL_Texture *texture;
    SDL_FRect rect;
    SDL_FPoint center;
};
