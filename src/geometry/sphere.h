#pragma once

#include "geometry/shape.h"

class Sphere final : public Shape {
public:
    Sphere(const Vector& centre, double radius, const Colour& colour);
    std::optional<Hit> intersect(const Ray& ray, double t_min, double t_max) const override;

private:
    Vector _centre;
    double _radius;
    Colour _colour;
};
