#pragma once

#include <SDL3/SDL_rect.h>
#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

#include "Node.hpp"

class Camera
{
public:
    Camera();
    void setRendererRect(const SDL_FRect &rect);
    void setSceneRect(const glm::dvec2 &pos,
                      const glm::dvec2 &size);

    SDL_FPoint toRenderer(const glm::dvec2 &pos) const;
    Node toRendererNode(const glm::dvec2 pos,
                double angle = 0.0) const;

private:
    SDL_FRect rect;
    glm::dvec2 pos, size;
    glm::dmat3 transform;

    void updateTransformMatrix();
};
