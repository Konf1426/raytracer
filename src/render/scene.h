#pragma once

#include "geometry/shape.h"

#include <limits>
#include <memory>
#include <vector>

class Scene {
public:
    void add(std::unique_ptr<Shape> shape);
    std::optional<Hit> closest_hit(const Ray& ray, double t_min = ray_epsilon,
                                 double t_max = std::numeric_limits<double>::infinity()) const;

private:
    std::vector<std::unique_ptr<Shape>> _shapes;
};
