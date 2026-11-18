#include "geometry/plane.h"

Plane::Plane(const Vector& point, const Vector& normal, const Colour& colour)
    : _point(point), _normal(normal.normalised()), _colour(colour) {}

std::optional<Hit> Plane::intersect(const Ray& ray, double t_min, double t_max) const {
    const double denominator = _normal.dot(ray.direction());
    if (denominator == 0.0) {
        return std::nullopt;
    }
    const double t = _normal.dot(_point - ray.origin()) / denominator;
    if (t <= t_min || t >= t_max) {
        return std::nullopt;
    }
    return Hit{t, ray.at(t), _normal, _colour};
}
