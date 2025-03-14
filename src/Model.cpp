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
    this->config = loadJson("configs/config.json");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("[Model::init] Couldn't init SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
            this->config["window_settings"]["name"].get<std::string>().c_str()
            , this->config["window_settings"]["width"].get<int>()
            , this->config["window_settings"]["height"].get<int>()
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
    float colors[] = {
        this->config["window_settings"]["bg_color"]["r"].get<float>(),
        this->config["window_settings"]["bg_color"]["g"].get<float>(),
        this->config["window_settings"]["bg_color"]["b"].get<float>()
    };
    SDL_SetRenderDrawColorFloat(
        this->renderer
        ,colors[0], colors[1], colors[2]
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
    SDL_FRect r0 = {this->p1.x, this->p1.y, static_cast<float>(this->t_image.rect.w), static_cast<float>(this->t_image.rect.h)};
    SDL_RenderTexture(
        this->renderer
        , this->t_image.texture
        , nullptr
        , &r0
        );

    SDL_FRect r1 = {this->p2.x, this->p2.y, static_cast<float>(this->t0.rect.w), static_cast<float>(this->t0.rect.h)};
    SDL_RenderTextureRotated(
        this->renderer
        , this->t0.texture
        , nullptr
        , &r1
        , -90
        , &t0.center
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
