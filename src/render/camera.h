#pragma once

#include "geometry/ray.h"

class Camera {
public:
    Camera(int width, int height, const Vector& eye = Vector(0, 0, 0),
           double viewport_height = 2.0, double focal_length = 1.0);
    int width() const;
    int height() const;
    Ray ray_for_pixel(int x, int y) const;

private:
    int _width;
    int _height;
    Vector _eye;
    double _viewport_height;
    double _focal_length;
};
