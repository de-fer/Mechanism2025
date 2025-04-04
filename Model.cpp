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
            , SDL_WINDOW_RESIZABLE
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
    case SDL_EVENT_KEY_DOWN:
        return this->onKeyDownEvent(event->key);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onKeyDownEvent(SDL_KeyboardEvent &event)
{
    switch(event.key) {
    case SDLK_SPACE:
        this->phi -= glm::radians(5.0);
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::iterate()
{
    this->clearWindow();

    this->solveMechanism();

    this->updateCamera();

    this->updateNodes();

    this->ecs.progress();

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
    this->renderMechanism = this->createRenderMechanismSystem();

    this->phi = 0.0;

    this->p0 = {0.0, 0.0};
    this->l1 = 1.0;
    this->l2 = 2.0;

    this->e0 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createBaseTexture();
        });
    this->e1 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l1);
        });
    this->e2 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l2);
        });

    SDL_Log("[Model::initMechanism] The mechanism has been initialized");
}

void Model::solveMechanism()
{
    this->a1 = this->phi;
    this->p1 = {this->l1, 0.0};
    this->p1 = glm::rotate(this->p1, this->a1);
    this->p1 = this->p0 + this->p1;

    this->a2 = -glm::asin(
        this->l1 / this->l2 * glm::sin(a1)
        );
    this->p2 = {this->l2, 0.0};
    this->p2 = glm::rotate(this->p2, this->a2);
    this->p2 = this->p1 + this->p2;
}

void Model::updateNodes()
{
    this->e0.set<Node>(this->camera.toRendererNode(
        this->p0
        ));

    this->e1.set<Node>(this->camera.toRendererNode(
        this->p0,
        this->a1
        ));

    this->e2.set<Node>(this->camera.toRendererNode(
        this->p1,
        this->a2
        ));
}

void Model::updateCamera()
{
    int w, h;
    if (!SDL_GetWindowSize(this->window, &w, &h)){
        SDL_GetError();
        return;
    }
    SDL_FRect rect = {
        0.0f, 0.0f,
        static_cast<float>(w),
        static_cast<float>(h),
        };
    this->camera.setRendererRect(rect);
    glm::dvec2 pos = {-1.0, -1.0};
    glm::dvec2 size = {
        static_cast<float>(w) / this->scale,
        static_cast<float>(h) / this->scale,
        };
    this->camera.setSceneRect(pos, size);
}

flecs::system Model::createRenderMechanismSystem()
{
    return this->ecs.system<const Node, const Texture>()
    .each([this](const Node &n, const Texture &t)
    {
        SDL_FRect r = {
            n.position.x - t.center.x
            , n.position.y - t.center.y
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

    SDL_Log("[Model::createBaseTexture] Texture created: %p [%i x %i]", result.texture, static_cast<int>(result.rect.w), static_cast<int>(result.rect.h));
    return result;
}

Texture Model::createLinkTexture(double l)
{
    Texture result;

    Camera camera;
    glm::dvec2 pos = {-1.0, -1.0};
    glm::dvec2 size = {l + 2.0, 2.0};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast <float>(-pos.x * this->scale),
        static_cast <float>(-pos.y * this->scale)
    };
    result.rect = {
        0.f, 0.f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 2> link;
    link[0] = camera.toRenderer({0.0, 0.0});
    link[1] = camera.toRenderer({l, 0.0});

    SDL_Surface* surface = SDL_CreateSurface(
          static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);

    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.f, 0.f, 0.f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , link.data()
        , link.size()
        );
    SDL_RenderPresent(renderer);
    result.texture = SDL_CreateTextureFromSurface(
        this->renderer
        , surface
        );

    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    return result;
}
