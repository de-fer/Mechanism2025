#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include "Model.hpp"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    auto app = new Model;
    *appstate = static_cast<void*>(app);
    return app->init();
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto app = static_cast<Model*>(appstate);
    if (app){
        return app->onEvent(event);
    }

    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto app = static_cast<Model*>(appstate);
    if (app){
        return app->iterate();
    }

    return SDL_APP_FAILURE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    auto app = static_cast<Model*>(appstate);
    if (app){
        delete app;
    }
}
