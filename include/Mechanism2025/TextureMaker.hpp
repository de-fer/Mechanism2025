#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "Texture.hpp"

static Texture textureCreateFromSurface(SDL_Renderer* renderer, SDL_Surface* surface)
{
    Texture texture;
    texture.texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture.texture){
        SDL_Log("[TextureMaker::textureCreateFromSurface] The texture wasn't created from surface %p", surface);
    }
    else {
        SDL_Log("[TextureMaker::textureCreateFromSurface] The texture was created as %p from surface %p", texture.texture, surface);
    }
    SDL_SetTextureScaleMode(texture.texture, SDL_SCALEMODE_NEAREST);
    texture.rect = {0, 0, (float)surface->w, (float)surface->h};
    texture.center = {surface->w*0.5f, surface->h*0.5f};

    SDL_DestroySurface(surface);
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

static Texture textureLoader(SDL_Renderer* renderer, const char *file_name)
{
    Texture texture;
    SDL_Surface* surface = IMG_Load(file_name);
    if (!surface){
        SDL_Log("[TextureMaker::textureLoader] The %s texture wasn't loaded. Creating error texture!", file_name);
        surface = createErrorSurface();
    }
    else {
        SDL_Log("[TextureMaker::textureLoader] The %s texture was loaded as surface %p", file_name, surface);
    }
    texture = textureCreateFromSurface(renderer, surface);
    return texture;
}
