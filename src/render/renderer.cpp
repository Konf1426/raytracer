#include "render/renderer.h"

#include <cmath>
#include <stdexcept>

Renderer::Renderer(const Vector& to_sun, double ambient)
    : _to_sun(to_sun.normalised()), _ambient(ambient) {
    if (ambient < 0.0 || ambient > 1.0 || !std::isfinite(ambient)) {
        throw std::invalid_argument("Ambient light must be between zero and one");
    }
}

Colour Renderer::trace(const Scene& scene, const Ray& ray) const {
    const auto hit = scene.closest_hit(ray);
    if (!hit) {
        return Colour(0.5, 0.7, 1.0);
    }

    bool in_shadow = false;
    if (hit->normal.dot(_to_sun) > 0.0) {
        // Offset the new origin to avoid rounding errors shadowing the surface itself.
        const Ray shadow_ray(hit->point + ray_epsilon * hit->normal, _to_sun);
        in_shadow = scene.closest_hit(shadow_ray).has_value();
    }
    return hit->material.shade(hit->normal, _to_sun, _ambient, in_shadow);
}

Image Renderer::render(const Scene& scene, const Camera& camera) const {
    Image image(camera.width(), camera.height());
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set(x, y, trace(scene, camera.ray_for_pixel(x, y)));
        }
    }
    return image;
}
