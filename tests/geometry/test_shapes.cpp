#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "support/assertions.h"

#include <limits>
#include <stdexcept>

static constexpr double infinity = std::numeric_limits<double>::infinity();
static const Colour red(1, 0, 0);

TEST_CASE("a sphere returns the first visible hit and outward normal", "[sphere]") {
    const Sphere sphere(Vector(0, 0, -5), 1, Material(red));
    const auto hit = sphere.intersect(Ray(Vector(0, 0, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(4, tolerance));
    require_vectors_equal(hit->point, Vector(0, 0, -4));
    require_vectors_equal(hit->normal, Vector(0, 0, 1));
    require_colours_equal(hit->material.colour(), red);
}

TEST_CASE("sphere intersections support nonunit directions and clipped roots", "[sphere]") {
    const Sphere sphere(Vector(0, 0, -5), 1, Material(red));
    const auto scaled = sphere.intersect(Ray(Vector(0, 0, 0), Vector(0, 0, -2)), 0, infinity);
    REQUIRE(scaled.has_value());
    REQUIRE_THAT(scaled->t, Catch::Matchers::WithinAbs(2, tolerance));
    const Ray ray(Vector(0, 0, 0), Vector(0, 0, -1));
    const auto far = sphere.intersect(ray, 4.5, infinity);
    REQUIRE(far.has_value());
    REQUIRE_THAT(far->t, Catch::Matchers::WithinAbs(6, tolerance));
    REQUIRE_FALSE(sphere.intersect(ray, 0, 3).has_value());
}

TEST_CASE("a ray inside a sphere finds its exit", "[sphere]") {
    const Sphere sphere(Vector(1, 2, 3), 2, Material(red));
    const auto hit = sphere.intersect(Ray(Vector(1, 2, 3), Vector(1, 0, 0)), 0, infinity);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(2, tolerance));
    require_vectors_equal(hit->normal, Vector(1, 0, 0));
}

TEST_CASE("a sphere handles tangents misses and objects behind the eye", "[sphere]") {
    const Sphere sphere(Vector(0, 0, -5), 1, Material(red));
    const auto tangent = sphere.intersect(Ray(Vector(1, 0, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(tangent.has_value());
    REQUIRE_THAT(tangent->t, Catch::Matchers::WithinAbs(5, tolerance));
    REQUIRE_FALSE(sphere.intersect(Ray(Vector(2, 0, 0), Vector(0, 0, -1)), 0, infinity));
    REQUIRE_FALSE(sphere.intersect(Ray(Vector(0, 0, 0), Vector(0, 0, 1)), 0, infinity));
}

TEST_CASE("a sphere must have a positive finite radius", "[sphere]") {
    REQUIRE_THROWS_AS(Sphere(Vector(0, 0, 0), 0, Material(red)), std::invalid_argument);
    REQUIRE_THROWS_AS(Sphere(Vector(0, 0, 0), -1, Material(red)), std::invalid_argument);
    REQUIRE_THROWS_AS(Sphere(Vector(0, 0, 0), infinity, Material(red)), std::invalid_argument);
}

TEST_CASE("the client's floor example hits at t two", "[plane]") {
    const Plane floor(Vector(0, 0, 0), Vector(0, 2, 0), Material(red));
    const auto hit = floor.intersect(Ray(Vector(0, 8, 0), Vector(3, -4, 0)), 0, infinity);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(2, tolerance));
    require_vectors_equal(hit->point, Vector(6, 0, 0));
    require_vectors_equal(hit->normal, Vector(0, 1, 0));
}

TEST_CASE("a plane ignores parallel backward and clipped intersections", "[plane]") {
    const Plane floor(Vector(0, 0, 0), Vector(0, 1, 0), Material(red));
    REQUIRE_FALSE(floor.intersect(Ray(Vector(0, 8, 0), Vector(5, 0, 0)), 0, infinity));
    REQUIRE_FALSE(floor.intersect(Ray(Vector(0, 8, 0), Vector(3, 4, 0)), 0, infinity));
    REQUIRE_FALSE(floor.intersect(Ray(Vector(0, 8, 0), Vector(3, -4, 0)), 0, 1));
    REQUIRE_FALSE(floor.intersect(Ray(Vector(0, 0, 0), Vector(0, -1, 0)), 0, infinity));
    REQUIRE_THROWS_AS(Plane(Vector(0, 0, 0), Vector(0, 0, 0), Material(red)), std::invalid_argument);
}
