#pragma once

#include "image/image.h"
#include "render/camera.h"
#include "render/scene.h"

class Renderer {
public:
    explicit Renderer(const Vector& to_sun = Vector(-1, 2, 1), double ambient = 0.15);
    Colour trace(const Scene& scene, const Ray& ray) const;
    Image render(const Scene& scene, const Camera& camera) const;

private:
    Vector _to_sun;
    double _ambient;
};
