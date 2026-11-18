#include "render/scene.h"

#include <stdexcept>
#include <utility>

void Scene::add(std::unique_ptr<Shape> shape) {
    if (!shape) {
        throw std::invalid_argument("A scene cannot contain a null shape");
    }
    _shapes.push_back(std::move(shape));
}

std::optional<Hit> Scene::closest_hit(const Ray& ray, double t_min, double t_max) const {
    std::optional<Hit> closest;
    for (const auto& shape : _shapes) {
        auto hit = shape->intersect(ray, t_min, t_max);
        if (hit) {
            t_max = hit->t;
            closest = hit;
        }
    }
    return closest;
}
