#include "material/material.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

Material::Material(const Colour& colour) : _colour(colour) {}

const Colour& Material::colour() const { return _colour; }

Colour Material::shade(const Vector& normal, const Vector& to_light,
                       double ambient, bool in_shadow) const {
    if (ambient < 0.0 || ambient > 1.0 || !std::isfinite(ambient)) {
        throw std::invalid_argument("Ambient light must be between zero and one");
    }
    const double direct = in_shadow ? 0.0 : std::max(0.0, normal.dot(to_light));
    return (ambient + (1.0 - ambient) * direct) * _colour;
}
