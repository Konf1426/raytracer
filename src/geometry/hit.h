#pragma once

#include "image/colour.h"
#include "math/vector.h"

inline constexpr double ray_epsilon = 1e-6;

struct Hit {
    double t;
    Vector point;
    Vector normal;
    Colour colour;
};
