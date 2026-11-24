#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "geometry/triangle.h"
#include "material/material.h"
#include "render/renderer.h"
#include "support/assertions.h"

#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

static constexpr double infinity = std::numeric_limits<double>::infinity();

TEST_CASE("a sphere owns its material independently of its constructor argument", "[composition]") {
    STATIC_REQUIRE_FALSE(std::is_constructible_v<Sphere, Vector, double, Colour>);
    Material source(Colour(0.8, 0.4, 0.2));
    const Sphere sphere(Vector(0, 0, -5), 1, source);
    source = Material(Colour(0, 0, 1));
    const auto hit = sphere.intersect(Ray(Vector(0, 0, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(hit.has_value());
    require_colours_equal(hit->material.colour(), Colour(0.8, 0.4, 0.2));
    // N=+Z, L=(0.6,0,0.8): cosine 0.8, applied to every material channel.
    require_colours_equal(hit->material.shade(hit->normal, Vector(0.6, 0, 0.8)),
                          Colour(0.64, 0.32, 0.16));
}

TEST_CASE("a plane returns its material with its geometric hit", "[composition]") {
    STATIC_REQUIRE_FALSE(std::is_constructible_v<Plane, Vector, Vector, Colour>);
    const Plane plane(Vector(0, 0, 0), Vector(0, 1, 0), Material(Colour(0.2, 0.8, 0.4)));
    const auto hit = plane.intersect(Ray(Vector(0, 1, 0), Vector(0, -1, 0)), 0, infinity);
    REQUIRE(hit.has_value());
    require_colours_equal(hit->material.colour(), Colour(0.2, 0.8, 0.4));
    require_colours_equal(hit->material.shade(hit->normal, Vector(0, 0.6, 0.8)),
                          Colour(0.12, 0.48, 0.24));
}

TEST_CASE("a triangle returns its material with its geometric hit", "[composition]") {
    STATIC_REQUIRE_FALSE(std::is_constructible_v<Triangle, Vector, Vector, Vector, Colour>);
    const Triangle triangle(Vector(0, 0, -5), Vector(2, 0, -5), Vector(0, 2, -5),
                            Material(Colour(0.2, 0.4, 0.8)));
    const auto hit = triangle.intersect(Ray(Vector(0.5, 0.5, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(hit.has_value());
    require_colours_equal(hit->material.colour(), Colour(0.2, 0.4, 0.8));
    require_colours_equal(hit->material.shade(hit->normal, Vector(0.6, 0, 0.8)),
                          Colour(0.16, 0.32, 0.64));
}

TEST_CASE("a returned hit keeps its material after the shape is destroyed", "[composition]") {
    std::optional<Hit> hit;
    {
        const Triangle triangle(Vector(0, 0, -5), Vector(2, 0, -5), Vector(0, 2, -5),
                                Material(Colour(0.8, 0.4, 0.2)));
        hit = triangle.intersect(Ray(Vector(0.5, 0.5, 0), Vector(0, 0, -1)), 0, infinity);
    }
    REQUIRE(hit.has_value());
    require_colours_equal(hit->material.shade(hit->normal, Vector(0, 0, 1)),
                          Colour(0.8, 0.4, 0.2));
}

TEST_CASE("the renderer uses the same material response for every shape", "[composition][render]") {
    const Material orange(Colour(0.8, 0.4, 0.2));
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Sphere>(Vector(0, 0, -5), 1, orange));
    shapes.push_back(std::make_unique<Plane>(Vector(0, 0, -4), Vector(0, 0, 1), orange));
    shapes.push_back(std::make_unique<Triangle>(Vector(-1, -1, -4), Vector(1, -1, -4),
                                               Vector(0, 1, -4), orange));
    for (auto& shape : shapes) {
        Scene scene;
        scene.add(std::move(shape));
        // Every hit has N=+Z. Brightness = 0.15 + 0.85*0.8 = 0.83.
        require_colours_equal(Renderer(Vector(0.6, 0, 0.8)).trace(
                                  scene, Ray(Vector(0, 0, 0), Vector(0, 0, -1))),
                              Colour(0.664, 0.332, 0.166));
    }
}
