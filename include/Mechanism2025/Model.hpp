#pragma once

#include <SDL3/SDL.h>
#include "JsonLoader.hpp"

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
    void renderMechanism();

    SDL_Texture *t0;
    SDL_Texture *t_image;
    SDL_FPoint p1, p2;
};

SDL_Surface* createBaseSurface();
