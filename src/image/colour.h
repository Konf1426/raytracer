#pragma once

class Colour {
public:
    Colour(double red = 0, double green = 0, double blue = 0);
    double r() const;
    double g() const;
    double b() const;

private:
    double _red;
    double _green;
    double _blue;
};

Colour operator*(double intensity, const Colour& colour);
