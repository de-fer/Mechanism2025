#include "Model.hpp"
#include "TextureMaker.hpp"

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
    this->t0 = textureCreateFromSurface(this->renderer, createBaseSurface());
    this->t_image = textureLoader(renderer, "assets/my_img.png");

    this->p1 = {100.f, 200.f};
    this->p2 = {300.f, 50.f};

    SDL_Log("[Model::initMechanism] The mechanism has been initialized");
}

void Model::renderMechanism()
{
    SDL_FRect r0 = {this->p1.x, this->p1.y, 100.f, 100.f};
    SDL_RenderTexture(
        this->renderer
        , this->t_image
        , nullptr
        , &r0
        );

    SDL_FRect r1 = {this->p2.x, this->p2.y, 100.f, 100.f};
    SDL_FPoint c1 = {50.f, 50.f};
    SDL_RenderTextureRotated(
        this->renderer
        , this->t0
        , nullptr
        , &r1
        , -90
        , &c1
        , SDL_FLIP_NONE
        );
}

SDL_Surface *createBaseSurface()
{
    SDL_Surface* surface = SDL_CreateSurface(
        100, 100
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);

    SDL_FPoint base[4];
    base[0] = base[3] = {50.f, 50.f};
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

    SDL_Log("[Model::createBaseSurface] Surface created: %p", surface);

    SDL_DestroyRenderer(renderer);
    return surface;
}
