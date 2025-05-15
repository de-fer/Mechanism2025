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

    this->phi = glm::radians(90.0);;

    this->p0 =  {0.0, 0.0};
    this->p03 = {-0.5, 0.09};
    this->p05 = {0.0, 0.25};

    this->l1  = 0.12;
    this->l2  = 0.50;
    this->l2n = 0.255;
    this->l2d = 0.2;
    this->a3  = glm::radians(0.0);
    this->a4  = glm::radians(90.0);
    this->a5  = glm::radians(-90.0);

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
    this->e2n = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l2d);
        });
    this->e3 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createSliderTexture();
        });
    this->e03 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createBaseSliderTexture();
        });
    this->e4 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createSliderTexture();
        });
    this->e5 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->create5LinkTexture();
        });
    this->e05 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->create5BaseTexture();
        });

    SDL_Log("[Model::initMechanism] The mechanism has been initialized");
}

void Model::solveMechanism()
{
    // Ведущее звено
    this->a1 = this->phi;
    this->p1 = {this->l1, 0.0};
    this->p1 = glm::rotate(this->p1, this->a1);
    this->p1 = this->p0 + this->p1;
    // 2 звено
    this->p2 = {0.0, 0.05};  //Yc = 0.05m
    this->p2.y = this->p0.y-this->p1.y+ this->p2.y;
    this->p2.x = -glm::sqrt(glm::pow2(this->l2) - glm::pow2(this->p2.y));
    this->a2 = glm::atan2(this->p2.y, this->p2.x);
    this->p2 = this->p1 + this->p2;
    // 3 звено (ползун)
    this->p3 = this->p2;
    // ответвление 2 звена
    this->p2n = this->p1 + (this->p2-this->p1)*(this->l2n/this->l2);
    this->a2n = this->a2 + glm::radians(-90.0);
    // 4 звено (ползун)
    this->p4 = (this->p2-this->p1)*(this->l2d/this->l2); //отрезок длиной ND в направлении BC (основной части 2 звена)
    this->p4 = glm::rotate(this->p4, glm::radians(-90.0)); //отрезок ND в нужном направлении, угол не меняется
    this->p4 = this->p2n + this->p4;
    // 5 звено
    this->p5 = {this->p4.x, this->p0.y + 0.25}; //Ye = 0.25m
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

    this->e2n.set<Node>(this->camera.toRendererNode(
        this->p2n,
        this->a2n
        ));

    this->e3.set<Node>(this->camera.toRendererNode(
        this->p3,
        this->a3
        ));

    this->e03.set<Node>(this->camera.toRendererNode(
        this->p03
        ));

    this->e4.set<Node>(this->camera.toRendererNode(
        this->p4,
        this->a4
        ));

    this->e5.set<Node>(this->camera.toRendererNode(
        this->p5,
        this->a5
        ));

    this->e05.set<Node>(this->camera.toRendererNode(
        this->p05
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
    glm::dvec2 pos = {-1.0, -0.3};
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
    result.rect = {0.f, 0.f, 60.f, 60.f};

    SDL_Surface* surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(surface);

    SDL_FPoint base[4];
    result.center = base[0] = base[3] = {result.rect.w/2.f, result.rect.h/2.f};
    base[1] = { 0.f,          result.rect.h-1.f};
    base[2] = {result.rect.w, result.rect.h-1.f};

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

Texture Model::createSliderTexture()
{
    Texture result;

    Camera camera;
    glm::dvec2 pos =  {0.0, 0.0};
    glm::dvec2 size = {0.1, 0.05};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast <float>(size.x/2. * this->scale),
        static_cast <float>(size.y/2. * this->scale)
    };
    result.rect = {
        0.f, 0.f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 5> vertex;
    //нижняя
    vertex[0] = camera.toRenderer({0.0       , .01   });
    //правая
    vertex[1] = camera.toRenderer({size.x-.01, .01   });
    //верхняя
    vertex[2] = camera.toRenderer({size.x-.01, size.y});
    //левая
    vertex[3] = camera.toRenderer({0.0       , size.y});
    vertex[4] = camera.toRenderer({0.0       , .01   });

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
        , vertex.data()
        , vertex.size()
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

Texture Model::create5LinkTexture()
{
    Texture result;

    Camera camera;
    glm::dvec2 pos =  {0.0, 0.0};
    double width = 0.08;
    glm::dvec2 size = {0.3, width+0.3*2};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast <float>(size.x/2. * this->scale),
        static_cast <float>(size.y/2. * this->scale)
    };
    result.rect = {
        0.f, 0.f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 8> vertex;
    vertex[0] = camera.toRenderer({size.x/2   , .01   });
    vertex[1] = camera.toRenderer({size.x/2   , .3    });
    //нижняя
    vertex[2] = camera.toRenderer({size.x-0.01, .3    });
    vertex[3] = camera.toRenderer({0.0        , .3    });
    //левая
    vertex[4] = camera.toRenderer({0.0        , .3+width});
    //верхняя
    vertex[5] = camera.toRenderer({size.x-0.01, .3+width});
    //
    vertex[6] = camera.toRenderer({size.x/2   , .3+width});
    vertex[7] = camera.toRenderer({size.x/2   , size.y-0.01});

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
        , vertex.data()
        , vertex.size()
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

Texture Model::create5BaseTexture()
{
    Texture result;

    Camera camera;
    glm::dvec2 pos =  {0.0, 0.0};
    glm::dvec2 size = {0.1, 0.05};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast <float>(size.x/2. * this->scale),
        static_cast <float>(size.y/2. * this->scale)
    };
    result.rect = {
        0.f, 0.f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 4> vertex;
    //нижняя
    vertex[0] = camera.toRenderer({0.0       , 0.01   });
    vertex[1] = camera.toRenderer({size.x-.01, 0.01   });
    //верхняя
    vertex[2] = camera.toRenderer({size.x-.01, size.y-.01});
    vertex[3] = camera.toRenderer({0.0       , size.y-.01});

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
    SDL_RenderLine(renderer, vertex[0].x, vertex[0].y, vertex[1].x, vertex[1].y);
    SDL_RenderLine(renderer, vertex[2].x, vertex[2].y, vertex[3].x, vertex[3].y);
    SDL_RenderPresent(renderer);
    result.texture = SDL_CreateTextureFromSurface(
        this->renderer
        , surface
        );

    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    return result;
}

Texture Model::createBaseSliderTexture()
{
    Texture result;

    Camera camera;
    glm::dvec2 pos =  {0.0, 0.0};
    glm::dvec2 size = {0.5, 0.05};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast <float>(size.x/2. * this->scale),
        static_cast <float>(-0.06 * this->scale)
    };
    result.rect = {
        0.f, 0.f,
        static_cast<float>(size.x * this->scale),
        1.f
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 2> vertex;
    //нижняя
    vertex[0] = camera.toRenderer({0.0       , .01   });
    vertex[1] = camera.toRenderer({size.x-.01, .01   });

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
        , vertex.data()
        , vertex.size()
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
