#include "geometry/ray.h"

#include <cmath>
#include <stdexcept>

Ray::Ray(const Vector& origin, const Vector& direction) : _origin(origin), _direction(direction) {
    const double magnitude = direction.length();
    if (magnitude == 0.0 || !std::isfinite(magnitude)) {
        throw std::invalid_argument("A ray needs a nonzero finite direction");
    }
}

const Vector& Ray::origin() const { return _origin; }
const Vector& Ray::direction() const { return _direction; }
Vector Ray::at(double t) const { return _origin + t * _direction; }
