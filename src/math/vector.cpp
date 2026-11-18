#include "math/vector.h"

#include <cmath>
#include <stdexcept>

Vector::Vector(double x, double y, double z) : _x(x), _y(y), _z(z) {}

double Vector::x() const { return _x; }
double Vector::y() const { return _y; }
double Vector::z() const { return _z; }

Vector Vector::operator+(const Vector& other) const {
    return Vector(_x + other._x, _y + other._y, _z + other._z);
}


Vector Vector::operator-(const Vector& other) const {
    return Vector(_x - other._x, _y - other._y, _z - other._z);
}

Vector operator*(double t, const Vector& vector) {
    return Vector(t * vector.x(), t * vector.y(), t * vector.z());
}

double Vector::dot(const Vector& other) const {
    return _x * other._x + _y * other._y + _z * other._z;
}

double Vector::length() const { return std::sqrt(dot(*this)); }

Vector Vector::normalised() const {
    const double magnitude = length();
    if (magnitude == 0.0 || !std::isfinite(magnitude)) {
        throw std::invalid_argument("Cannot normalise a zero or non-finite vector");
    }
    return (1.0 / magnitude) * (*this);
}
