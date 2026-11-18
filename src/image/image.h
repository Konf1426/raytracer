#pragma once

#include "image/colour.h"

#include <cstddef>
#include <vector>

class Image {
public:
    Image(int width, int height);
    int width() const;
    int height() const;
    const Colour& at(int x, int y) const;
    void set(int x, int y, const Colour& colour);

private:
    std::size_t index(int x, int y) const;
    int _width;
    int _height;
    std::vector<Colour> _pixels;
};
