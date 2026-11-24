#include "geometry/sphere.h"

#include <cmath>
#include <stdexcept>

Sphere::Sphere(const Vector& centre, double radius, const Material& material)
    : _centre(centre), _radius(radius), _material(material) {
    if (radius <= 0.0 || !std::isfinite(radius)) {
        throw std::invalid_argument("A sphere needs a positive finite radius");
    }
}

std::optional<Hit> Sphere::intersect(const Ray& ray, double t_min, double t_max) const {
    const Vector offset = ray.origin() - _centre;
    const double a = ray.direction().dot(ray.direction());
    const double half_b = offset.dot(ray.direction());
    const double c = offset.dot(offset) - _radius * _radius;
    const double discriminant = half_b * half_b - a * c;
    if (discriminant < 0.0) {
        return std::nullopt;
    }

    const double root = std::sqrt(discriminant);
    double t = (-half_b - root) / a;
    if (t <= t_min || t >= t_max) {
        t = (-half_b + root) / a;
        if (t <= t_min || t >= t_max) {
            return std::nullopt;
        }
    }
    const Vector point = ray.at(t);
    return Hit{t, point, (1.0 / _radius) * (point - _centre), _material};
}
