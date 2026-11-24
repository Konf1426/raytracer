#include "geometry/triangle.h"

Triangle::Triangle(const Vector& a, const Vector& b, const Vector& c, const Material& material)
    : _a(a), _edge1(b - a), _edge2(c - a),
      _normal(_edge1.cross(_edge2).normalised()), _material(material) {}

std::optional<Hit> Triangle::intersect(const Ray& ray, double t_min, double t_max) const {
    // Moller-Trumbore: solve origin + t*direction = a + u*edge1 + v*edge2.
    const Vector p = ray.direction().cross(_edge2);
    const double determinant = _edge1.dot(p);
    if (determinant == 0.0) {
        return std::nullopt;
    }

    const Vector offset = ray.origin() - _a;
    const double u = offset.dot(p) / determinant;
    if (!(u >= 0.0 && u <= 1.0)) {
        return std::nullopt;
    }
    const Vector q = offset.cross(_edge1);
    const double v = ray.direction().dot(q) / determinant;
    if (!(v >= 0.0 && u + v <= 1.0)) {
        return std::nullopt;
    }
    const double t = _edge2.dot(q) / determinant;
    if (!(t > t_min && t < t_max)) {
        return std::nullopt;
    }
    return Hit{t, ray.at(t), _normal, _material};
}
