#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <array>

#include "Node.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

class Model
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

public:
    Model();
    ~Model();

    SDL_AppResult init();
    SDL_AppResult onEvent(SDL_Event *event);
    SDL_AppResult onKeyDownEvent(SDL_KeyboardEvent &event);
    SDL_AppResult iterate();

protected:
    void clearWindow();
    void initMechanism();
    void solveMechanism();
    flecs::system createRenderMechanismSystem();

    Texture createBaseTexture();
    Texture createLinkTexture(double l);

    flecs::world ecs;
    flecs::system renderMechanism;
    flecs::entity e0;
    flecs::entity e1;

    double scale = 100.0;

    double phi; //угол звена 1, радианы

    double l1; //длина 1 звена, m
    double l2; //длина 1 звена, m
    glm::dvec2 p0;
    glm::dvec2 p1, p2;
};
