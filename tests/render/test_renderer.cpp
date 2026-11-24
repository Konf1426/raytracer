#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "render/renderer.h"
#include "support/assertions.h"

#include <cmath>
#include <memory>
#include <stdexcept>

TEST_CASE("the scene finds the nearest shape independently of insertion order", "[scene]") {
    for (const bool near_first : {false, true}) {
        Scene scene;
        auto near = std::make_unique<Sphere>(Vector(0, 0, -3), 0.5, Material(Colour(1, 0, 0)));
        auto far = std::make_unique<Sphere>(Vector(0, 0, -6), 1, Material(Colour(0, 1, 0)));
        if (near_first) {
            scene.add(std::move(near));
            scene.add(std::move(far));
        } else {
            scene.add(std::move(far));
            scene.add(std::move(near));
        }
        const auto hit = scene.closest_hit(Ray(Vector(0, 0, 0), Vector(0, 0, -1)));
        REQUIRE(hit.has_value());
        REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(2.5, tolerance));
        require_colours_equal(hit->material.colour(), Colour(1, 0, 0));
    }
}

TEST_CASE("a scene supports planes and rejects absent shapes", "[scene]") {
    Scene scene;
    REQUIRE_FALSE(scene.closest_hit(Ray(Vector(0, 1, 0), Vector(0, -1, 0))));
    scene.add(std::make_unique<Plane>(Vector(0, 0, 0), Vector(0, 1, 0), Material(Colour(1, 1, 1))));
    REQUIRE(scene.closest_hit(Ray(Vector(0, 1, 0), Vector(0, -1, 0))));
    REQUIRE_THROWS_AS(scene.add(nullptr), std::invalid_argument);
}

TEST_CASE("a ray missing the scene returns the sky colour", "[render]") {
    require_colours_equal(Renderer().trace(Scene(), Ray(Vector(0, 0, 0), Vector(0, 1, 0))),
                          Colour(0.5, 0.7, 1));
}

TEST_CASE("sunlight follows the cosine of the angle to the normal", "[render]") {
    Scene scene;
    scene.add(std::make_unique<Plane>(Vector(0, 0, 0), Vector(0, 1, 0),
                                      Material(Colour(0.8, 0.4, 0.2))));
    const Ray ray(Vector(0, 1, 0), Vector(0, -1, 0));
    require_colours_equal(Renderer(Vector(0, 1, 0)).trace(scene, ray), Colour(0.8, 0.4, 0.2));
    require_colours_equal(Renderer(Vector(1, 0, 0)).trace(scene, ray), Colour(0.12, 0.06, 0.03));
    require_colours_equal(Renderer(Vector(0, -1, 0)).trace(scene, ray), Colour(0.12, 0.06, 0.03));
    const double brightness = 0.15 + 0.85 / std::sqrt(2.0);
    require_colours_equal(Renderer(Vector(0, 1, 1)).trace(scene, ray),
                          Colour(0.8 * brightness, 0.4 * brightness, 0.2 * brightness));
}

TEST_CASE("a sphere between the floor and sun casts a shadow", "[render]") {
    Scene scene;
    scene.add(std::make_unique<Plane>(Vector(0, 0, 0), Vector(0, 1, 0),
                                      Material(Colour(0.8, 0.4, 0.2))));
    scene.add(std::make_unique<Sphere>(Vector(0, 2, 0), 0.5, Material(Colour(1, 0, 0))));
    require_colours_equal(Renderer(Vector(0, 1, 0)).trace(scene, Ray(Vector(0, 1, 0), Vector(0, -1, 0))),
                          Colour(0.12, 0.06, 0.03));
}

TEST_CASE("a complete image contains the sphere and background", "[render]") {
    Scene scene;
    scene.add(std::make_unique<Sphere>(Vector(0, 0, -3), 1, Material(Colour(0.8, 0.2, 0.1))));
    const Image image = Renderer(Vector(0, 0, 1)).render(scene, Camera(3, 3));
    REQUIRE(image.width() == 3);
    REQUIRE(image.height() == 3);
    require_colours_equal(image.at(1, 1), Colour(0.8, 0.2, 0.1));
    require_colours_equal(image.at(0, 0), Colour(0.5, 0.7, 1));
}

TEST_CASE("the renderer rejects undefined sun and ambient settings", "[render]") {
    REQUIRE_THROWS_AS(Renderer(Vector(0, 0, 0)), std::invalid_argument);
    REQUIRE_THROWS_AS(Renderer(Vector(0, 1, 0), -0.1), std::invalid_argument);
    REQUIRE_THROWS_AS(Renderer(Vector(0, 1, 0), 1.1), std::invalid_argument);
}
