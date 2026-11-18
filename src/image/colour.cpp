#include "image/colour.h"

Colour::Colour(double red, double green, double blue)
    : _red(red), _green(green), _blue(blue) {}

double Colour::r() const { return _red; }
double Colour::g() const { return _green; }
double Colour::b() const { return _blue; }

Colour operator*(double intensity, const Colour& colour) {
    return Colour(intensity * colour.r(), intensity * colour.g(), intensity * colour.b());
}
