#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/matrix_transform_2d.hpp>
#include "Camera.hpp"

Camera::Camera()
    : rect({0.f, 0.f, 1.f, 1.f})
    , pos({0.0, 0.0})
    , size({1.0, 1.0})
    , transform({1.0})
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
    glm::dvec3 p = {pos.x, pos.y, 1.0};
    p = this->transform * p;
    return {static_cast<float>(p.x), static_cast<float>(p.y)};
}

void Camera::updateTransformMatrix()
{
    glm::dmat3 m = {1.0};
    m = glm::translate(m, -this->pos); //сдвигаем из центра в правый верхний угол
    m = glm::scale(m, glm::dvec2{
        static_cast<double>(this->rect.w) / this->size.x,
        -static_cast<double>(this->rect.h) / this->size.y
    });
    m = glm::translate(m, glm::dvec2{
        static_cast<double>(this->rect.x),
        static_cast<double>(this->rect.h)
    });
    this->transform = m;
}
