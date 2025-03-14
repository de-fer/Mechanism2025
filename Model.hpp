#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

#include "Node.hpp"
#include "Texture.hpp"

class Model
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

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

    Texture createBaseTexture();

    flecs::world ecs;
    flecs::system renderMechanism;
    flecs::entity e0;
    flecs::entity e1;
};
