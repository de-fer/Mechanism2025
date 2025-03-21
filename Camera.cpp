#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Camera.hpp"

Camera::Camera()
    : rect({0.f, 0.f, 1.f, 1.f})
    , pos({0.0, 0.0})
    , size({1.0, 1.0})
{}

void Camera::setRendererRect(const SDL_FRect &rect)
{
    this->rect = rect;
}

void Camera::setSceneRect(const glm::dvec2 &pos, const glm::dvec2 &size)
{
    this->pos = pos;
    this->size = size;
}

SDL_FPoint Camera::toRenderer(const glm::dvec2 &pos) const
{
    return {static_cast<float>(pos.x), static_cast<float>(pos.y)};
}
