#pragma once

#include <SDL3/SDL.h>

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
    void renderMechanism();

    SDL_Texture* createBaseTexture();

    SDL_Texture *t0;
    SDL_Texture *t_image;
    SDL_FPoint p1, p2;
};
