#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static SDL_Texture* TextureLoader(SDL_Renderer* renderer, const char *file_name)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, file_name);
    if (!texture){
        SDL_Log("[TextureLoader] The %s texture wasn't loaded", file_name);
    }
    else {
        SDL_Log("[TextureLoader] The %s texture was loaded as %p", file_name, texture);
    }
    return texture;
}
