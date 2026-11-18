#pragma once

#include "geometry/hit.h"
#include "geometry/ray.h"

#include <optional>

class Shape {
public:
    virtual ~Shape() = default;
    virtual std::optional<Hit> intersect(const Ray& ray, double t_min, double t_max) const = 0;
};
