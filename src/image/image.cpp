#include "image/image.h"

#include <stdexcept>

Image::Image(int width, int height) : _width(width), _height(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
    }
    _pixels.resize(static_cast<std::size_t>(width) * static_cast<std::size_t>(height));
}

int Image::width() const { return _width; }
int Image::height() const { return _height; }

std::size_t Image::index(int x, int y) const {
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        throw std::out_of_range("Pixel coordinates are outside the image");
    }
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(_width) +
           static_cast<std::size_t>(x);
}

const Colour& Image::at(int x, int y) const { return _pixels[index(x, y)]; }
void Image::set(int x, int y, const Colour& colour) { _pixels[index(x, y)] = colour; }
