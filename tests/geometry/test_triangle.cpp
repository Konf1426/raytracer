#include "geometry/triangle.h"
#include "render/renderer.h"
#include "support/assertions.h"

#include <limits>
#include <memory>
#include <stdexcept>

static constexpr double infinity = std::numeric_limits<double>::infinity();
static const Colour red(1, 0, 0);

static Triangle example_triangle() {
    return Triangle(Vector(0, 0, -5), Vector(2, 0, -5), Vector(0, 2, -5), Material(red));
}

TEST_CASE("a triangle returns the hand calculated hit normal and surface colour", "[triangle]") {
    // Plane z=-5: -t=-5. Edges (2,0,0) cross (0,2,0) = (0,0,4).
    const auto hit = example_triangle().intersect(
        Ray(Vector(0.5, 0.5, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(5, tolerance));
    require_vectors_equal(hit->point, Vector(0.5, 0.5, -5));
    require_vectors_equal(hit->normal, Vector(0, 0, 1));
    require_colours_equal(hit->material.colour(), red);
}

TEST_CASE("a ray crossing the triangle plane outside its edges misses", "[triangle]") {
    const auto triangle = example_triangle();
    // u=v=0.75, so u+v>1; also check the two negative barycentric regions.
    for (const Vector origin : {Vector(1.5, 1.5, 0), Vector(-0.5, 0.5, 0), Vector(0.5, -0.5, 0)}) {
        REQUIRE_FALSE(triangle.intersect(Ray(origin, Vector(0, 0, -1)), 0, infinity));
    }
}

TEST_CASE("a triangle behind the ray and a parallel ray have no visible hit", "[triangle]") {
    const auto triangle = example_triangle();
    REQUIRE_FALSE(triangle.intersect(Ray(Vector(0.5, 0.5, 0), Vector(0, 0, 1)), 0, infinity));
    REQUIRE_FALSE(triangle.intersect(Ray(Vector(0.5, 0.5, 0), Vector(1, 0, 0)), 0, infinity));
    REQUIRE_FALSE(triangle.intersect(Ray(Vector(0.5, 0.5, -5), Vector(1, 0, 0)), 0, infinity));
}

TEST_CASE("triangle intersections preserve nonunit ray parameters and open bounds", "[triangle]") {
    const auto triangle = example_triangle();
    const Ray ray(Vector(0.5, 0.5, 0), Vector(0, 0, -2));
    const auto hit = triangle.intersect(ray, 0, infinity);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(2.5, tolerance));
    require_vectors_equal(hit->point, Vector(0.5, 0.5, -5));
    REQUIRE_FALSE(triangle.intersect(ray, 2.5, infinity));
    REQUIRE_FALSE(triangle.intersect(ray, 0, 2.5));
    REQUIRE_FALSE(triangle.intersect(ray, 3, 4));
}

TEST_CASE("triangle edges and vertices are included", "[triangle]") {
    const auto triangle = example_triangle();
    for (const Vector origin : {Vector(0, 0, 0), Vector(2, 0, 0), Vector(0, 2, 0),
                                Vector(1, 0, 0), Vector(0, 1, 0), Vector(1, 1, 0)}) {
        const auto hit = triangle.intersect(Ray(origin, Vector(0, 0, -1)), 0, infinity);
        REQUIRE(hit.has_value());
        REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(5, tolerance));
    }
}

TEST_CASE("triangles intersect from both sides and keep their winding normal", "[triangle]") {
    const Triangle triangle(Vector(0, 0, -5), Vector(0, 2, -5), Vector(2, 0, -5), Material(red));
    const auto hit = triangle.intersect(Ray(Vector(0.5, 0.5, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(hit.has_value());
    require_vectors_equal(hit->normal, Vector(0, 0, -1));
    const auto back = triangle.intersect(Ray(Vector(0.5, 0.5, -10), Vector(0, 0, 1)), 0, infinity);
    REQUIRE(back.has_value());
    REQUIRE_THAT(back->t, Catch::Matchers::WithinAbs(5, tolerance));
    require_vectors_equal(back->normal, Vector(0, 0, -1));
}

TEST_CASE("a tilted triangle normal uses all three axes", "[triangle]") {
    // (2,0,2) cross (0,2,2) = (-4,-4,4); at x=y=0.5 the plane has z=-4.
    const Triangle triangle(Vector(0, 0, -5), Vector(2, 0, -3), Vector(0, 2, -3), Material(red));
    const auto hit = triangle.intersect(Ray(Vector(0.5, 0.5, 0), Vector(0, 0, -1)), 0, infinity);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(4, tolerance));
    require_vectors_equal(hit->point, Vector(0.5, 0.5, -4));
    require_vectors_equal(hit->normal, Vector(-1, -1, 1).normalised());
}

TEST_CASE("triangles reject repeated collinear and nonfinite corners", "[triangle]") {
    REQUIRE_THROWS_AS(Triangle(Vector(0, 0, 0), Vector(0, 0, 0), Vector(1, 0, 0), Material(red)),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(Triangle(Vector(0, 0, 0), Vector(1, 1, 1), Vector(2, 2, 2), Material(red)),
                      std::invalid_argument);
    REQUIRE_THROWS_AS(Triangle(Vector(infinity, 0, 0), Vector(1, 0, 0), Vector(0, 1, 0), Material(red)),
                      std::invalid_argument);
}

TEST_CASE("scene selection and sunlight accept a triangle through Shape", "[triangle][render]") {
    Scene scene;
    // Far first, near second: both must participate in the existing nearest-hit query.
    scene.add(std::make_unique<Triangle>(Vector(0, 0, -8), Vector(2, 0, -8), Vector(0, 2, -8),
                                         Material(Colour(0, 0, 1))));
    scene.add(std::make_unique<Triangle>(example_triangle()));
    const Ray ray(Vector(0.5, 0.5, 0), Vector(0, 0, -1));
    const auto hit = scene.closest_hit(ray);
    REQUIRE(hit.has_value());
    REQUIRE_THAT(hit->t, Catch::Matchers::WithinAbs(5, tolerance));
    require_colours_equal(Renderer(Vector(0, 0, 1)).trace(scene, ray), red);
}
