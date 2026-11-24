#pragma once

#include "geometry/shape.h"

class Triangle final : public Shape {
public:
    // Both sides intersect. The normal follows (b - a) cross (c - a).
    // Collinear or non-finite vertices cannot define a normal and are rejected.
    Triangle(const Vector& a, const Vector& b, const Vector& c, const Colour& colour);
    std::optional<Hit> intersect(const Ray& ray, double t_min, double t_max) const override;

private:
    Vector _a;
    Vector _edge1;
    Vector _edge2;
    Vector _normal;
    Colour _colour;
};
