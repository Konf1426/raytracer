#pragma once

#include "image/colour.h"
#include "math/vector.h"

class Material {
public:
    explicit Material(const Colour& colour);
    const Colour& colour() const;

    // Unit normal and light direction. With two arguments this is Lambert's law.
    // Optional ambient and visibility preserve the renderer's existing lighting.
    // A non-finite ambient or a value outside [0, 1] throws std::invalid_argument.
    Colour shade(const Vector& normal, const Vector& to_light,
                 double ambient = 0.0, bool in_shadow = false) const;

private:
    Colour _colour;
};
