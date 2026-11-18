#include "render/camera.h"

#include <cmath>
#include <stdexcept>

Camera::Camera(int width, int height, const Vector& eye, double viewport_height, double focal_length)
    : _width(width), _height(height), _eye(eye), _viewport_height(viewport_height),
      _focal_length(focal_length) {
    if (width <= 0 || height <= 0 || viewport_height <= 0.0 || focal_length <= 0.0 ||
        !std::isfinite(viewport_height) || !std::isfinite(focal_length)) {
        throw std::invalid_argument("Camera dimensions, viewport and focal length must be positive");
    }
}

int Camera::width() const { return _width; }
int Camera::height() const { return _height; }

Ray Camera::ray_for_pixel(int x, int y) const {
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw std::out_of_range("Pixel coordinates are outside the camera");
    }
    const double viewport_width = _viewport_height * _width / _height;
    const double screen_x = ((x + 0.5) / _width - 0.5) * viewport_width;
    const double screen_y = (0.5 - (y + 0.5) / _height) * _viewport_height;
    return Ray(_eye, Vector(screen_x, screen_y, -_focal_length).normalised());
}
