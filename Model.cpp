#include "Model.hpp"

Model::Model()
{
    SDL_Log("[Model::Model] %p", this);
}

Model::~Model()
{
    SDL_Log("[Model::~Model] %p", this);
}

SDL_AppResult Model::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("[Model::init] Couldn't init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Model"
            , 480, 360
            , 0
            , &this->window
            , &this->renderer
            )
        )
    {
        SDL_Log("[Model::init] Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    this->initMechanism();

    SDL_Log("[Model::init] The Model has been initialized");
    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onEvent(SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::iterate()
{
    this->clearWindow();
    this->renderMechanism();

    SDL_RenderPresent(this->renderer);
    return SDL_APP_CONTINUE;
}

void Model::clearWindow()
{
    SDL_SetRenderDrawColorFloat(
        this->renderer
        , 0.7f, 0.7f, 0.7f
        , SDL_ALPHA_OPAQUE_FLOAT
        );

    SDL_RenderClear(this->renderer);
}

void Model::initMechanism()
{
    this->n0.position = {100.f, 200.f};
    this->n0.angle = 0.0;
    this->t0 = this->createBaseTexture();

    SDL_Log("[Model::initMechanism] The mechanism has been initialized");
}

void Model::renderMechanism()
{
    SDL_FRect r0 = {
        this->n0.position.x - this->t0.center.x
        , this->n0.position.y - this->t0.center.y
        , this->t0.rect.w
        , this->t0.rect.h
        };
    if (this->n0.angle)
    {
        SDL_RenderTextureRotated(
            this->renderer
            , this->t0.texture
            , &this->t0.rect
            , &r0
            , this->n0.angle
            , &this->t0.center
            , SDL_FLIP_NONE
            );
    }
    else
    {
        SDL_RenderTexture(
            this->renderer
            , this->t0.texture
            , &this->t0.rect
            , &r0
            );
    }
}

Texture Model::createBaseTexture()
{
    Texture result;
    result.rect = {0.f, 0.f, 100.f, 100.f};

    SDL_Surface* surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);

    SDL_FPoint base[4];
    result.center = base[0] = base[3] = {50.f, 50.f};
    base[1] = {10.f, 90.f};
    base[2] = {90.f, 90.f};

    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.f, 0.f, 0.f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , base
        , SDL_arraysize(base)
        );
    SDL_RenderPresent(renderer);
    result.texture = SDL_CreateTextureFromSurface(
        this->renderer
        , surface
        );

    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    SDL_Log("[Model::createBaseTexture] Texture created: %p [%i x %i]", result.texture, result.rect.w, result.rect.h);
    return result;
}
