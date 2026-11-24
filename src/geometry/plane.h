#pragma once

#include "geometry/shape.h"

class Plane final : public Shape {
public:
    Plane(const Vector& point, const Vector& normal, const Material& material);
    std::optional<Hit> intersect(const Ray& ray, double t_min, double t_max) const override;

private:
    Vector _point;
    Vector _normal;
    Material _material;
};
