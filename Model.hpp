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
    void updateCamera();
    void updateNodes();
    flecs::system createRenderMechanismSystem();

    Texture createBaseTexture();
    Texture createLinkTexture(double l);
    Texture createSliderTexture();
    Texture create5LinkTexture();

    // ECS
    flecs::world ecs;
    flecs::system renderMechanism;
    flecs::entity e0; // База
    flecs::entity e1; // Первичное звено
    flecs::entity e2; // 2 звено
    flecs::entity e2n; // ответвление 2 звена
    flecs::entity e3; // 3 звено
    flecs::entity e4; // 4 звено
    flecs::entity e5; // 5 звено

    double scale = 300.0;

    Camera camera;

    //Mechanism
    double phi; //угол звена 1, радианы

    double l1; //длина 1 звена, m
    double a1; //угол поворота 1 звена, рад
    double l2; //длина 2 звена, m
    double a2; //угол поворота 2 звена, рад
    double l2n; //длина до 2n звена, m
    double l2d; //длина ND звена, m
    double a2n; //угол поворота 2n звена, рад
    double a3; //угол поворота 3 звена, рад
    double a4; //угол поворота 4 звена, рад
    double a5; //угол поворота 5 звена, рад

    glm::dvec2 p0; //База
    glm::dvec2 p1, p2, p2n, p3, p4, p5;
};
