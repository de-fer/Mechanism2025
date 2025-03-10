#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return texture;
}

static SDL_Surface* createErrorSurface()
{
    SDL_Surface* surface = SDL_CreateSurface(
        2, 2
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);

    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.f, 0.f, 0.f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderPoint(renderer, 0.f,0.f);
    SDL_RenderPoint(renderer, 1.f,1.f);

    SDL_SetRenderDrawColorFloat(
        renderer
        , 1.f, 0.f, 1.f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderPoint(renderer, 1.f,0.f);
    SDL_RenderPoint(renderer, 0.f,1.f);
    SDL_RenderPresent(renderer);

    SDL_Log("[TextureMaker::createErrorSurface] Surface created: %p", surface);

    SDL_DestroyRenderer(renderer);
    return surface;
}

static SDL_Texture* textureLoader(SDL_Renderer* renderer, const char *file_name)
{
    SDL_Texture* texture = IMG_LoadTexture(renderer, file_name);
    if (!texture){
        SDL_Log("[TextureMaker::textureLoader] The %s texture wasn't loaded. Creating error texture!", file_name);
        texture = textureCreateFromSurface(renderer, createErrorSurface());
    }
    else {
        SDL_Log("[TextureMaker::textureLoader] The %s texture was loaded as %p", file_name, texture);
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return texture;
}
