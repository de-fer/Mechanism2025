#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static SDL_Texture* textureLoader(SDL_Renderer* renderer, const char *file_name)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, file_name);
    if (!texture){
        SDL_Log("[TextureMaker::textureLoader] The %s texture wasn't loaded", file_name);
    }
    else {
        SDL_Log("[TextureMaker::textureLoader] The %s texture was loaded as %p", file_name, texture);
    }
    return texture;
}

static SDL_Texture* textureCreateFromSurface(SDL_Renderer* renderer, SDL_Surface* surface)
{
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture){
        SDL_Log("[TextureMaker::textureCreateFromSurface] The texture wasn't created from surface %p", surface);
    }
    else {
        SDL_Log("[TextureMaker::textureCreateFromSurface] The texture was created as %p from surface %p", texture, surface);
    }
    SDL_DestroySurface(surface);

    return texture;
}
