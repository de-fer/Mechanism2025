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

    this->world.progress();

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
    this->renderMechanism = this->createRenderMechanismSystem();

    this->e0 = this->world.entity()
        .insert([this](Node &n, Texture &t)
        {
            n.pos = {100.f, 200.f};
            n.angle = 0.;
            t = textureLoader(this->renderer, "assets/my_img.png");
        });

    this->e1 = this->world.entity()
        .insert([this](Node &n, Texture &t)
        {
            n.pos = {300.f, 100.f};
            n.angle = -90.;
            t = textureCreateFromSurface(this->renderer, createBaseSurface());
        });

    SDL_Log("[Model::initMechanism] The mechanism has been initialized");
}

flecs::system Model::createRenderMechanismSystem()
{
    return this->world.system<const Node, const Texture>()
    .each([this](const Node &n, const Texture &t)
    {
        SDL_FRect r = {
            n.pos.x - t.center.x
            , n.pos.y - t.center.y
            , t.rect.w
            , t.rect.h
            };
        if (n.angle)
        {
            SDL_RenderTextureRotated(
                this->renderer
                , t.texture
                , &t.rect
                , &r
                , n.angle
                , &t.center
                , SDL_FLIP_NONE
                );
        }
        else
        {
            SDL_RenderTexture(
                this->renderer
                , t.texture
                , &t.rect
                , &r
                );
        }
    });
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
