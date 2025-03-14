#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

struct Texture
{
    SDL_Texture *texture;
    SDL_FRect rect;
    SDL_FPoint center;
};
