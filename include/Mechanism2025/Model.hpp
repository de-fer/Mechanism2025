#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

#include "JsonLoader.hpp"
#include "Node.hpp"
#include "Texture.hpp"

class Model
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    json config;

public:
    Model();
    ~Model();

    SDL_AppResult init();
    SDL_AppResult onEvent(SDL_Event *event);
    SDL_AppResult iterate();

protected:
    void clearWindow();
    void initMechanism();
    flecs::system createRenderMechanismSystem();

    flecs::world world;
    flecs::system renderMechanism;
    flecs::entity e0;
    flecs::entity e1;
};

SDL_Surface* createBaseSurface();
