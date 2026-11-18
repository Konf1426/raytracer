#include "math/vector.h"
#include "support/assertions.h"

#include <catch2/catch_test_macros.hpp>
#include <stdexcept>

TEST_CASE("vectors add the client's 2D example", "[vector]") {
    require_vectors_equal(Vector(3, 4, 0) + Vector(4, -3, 0), Vector(7, 1, 0));
}

TEST_CASE("vectors add nonzero z coordinates without changing operands", "[vector]") {
    const Vector a(3, 4, 1);
    const Vector b(4, -3, 2);
    require_vectors_equal(a + b, Vector(7, 1, 3));
    require_vectors_equal(a, Vector(3, 4, 1));
    require_vectors_equal(b, Vector(4, -3, 2));
}

TEST_CASE("vectors subtract in two and three dimensions", "[vector]") {
    require_vectors_equal(Vector(4, 5, 0) - Vector(1, 1, 0), Vector(3, 4, 0));
    require_vectors_equal(Vector(4, 5, 6) - Vector(1, 1, 2), Vector(3, 4, 4));
}

TEST_CASE("a scalar on the left scales every coordinate", "[vector]") {
    require_vectors_equal(2.0 * Vector(3, -4, 1), Vector(6, -8, 2));
    require_vectors_equal(0.0 * Vector(3, -4, 1), Vector(0, 0, 0));
    require_vectors_equal(-1.0 * Vector(3, -4, 1), Vector(-3, 4, -1));
}

TEST_CASE("dot products compare directions in three dimensions", "[vector]") {
    REQUIRE_THAT(Vector(3, 4, 0).dot(Vector(4, 3, 0)), Catch::Matchers::WithinAbs(24, tolerance));
    REQUIRE_THAT(Vector(3, 4, 0).dot(Vector(4, -3, 0)), Catch::Matchers::WithinAbs(0, tolerance));
    REQUIRE_THAT(Vector(3, 4, 12).dot(Vector(4, 3, 1)), Catch::Matchers::WithinAbs(36, tolerance));
}

TEST_CASE("length follows the 3-4-5 and 3-4-12-13 triangles", "[vector]") {
    REQUIRE_THAT(Vector(3, -4, 0).length(), Catch::Matchers::WithinAbs(5, tolerance));
    REQUIRE_THAT(Vector(3, 4, 12).length(), Catch::Matchers::WithinAbs(13, tolerance));
    REQUIRE_THAT(Vector(0, 0, 0).length(), Catch::Matchers::WithinAbs(0, tolerance));
}

TEST_CASE("normalised returns a unit vector in the original direction", "[vector]") {
    const Vector unit = Vector(3, -4, 0).normalised();
    require_vectors_equal(unit, Vector(0.6, -0.8, 0));
    const Vector original(3, 4, 12);
    REQUIRE_THAT(original.normalised().length(), Catch::Matchers::WithinAbs(1, tolerance));
    require_vectors_equal(original, Vector(3, 4, 12));
}

TEST_CASE("vector arithmetic expresses the ray equation", "[vector]") {
    const Vector origin(0, 8, 0);
    const Vector direction(3, -4, 0);
    require_vectors_equal(origin + 2.0 * direction, Vector(6, 0, 0));
}

TEST_CASE("a zero vector cannot be normalised", "[vector]") {
    REQUIRE_THROWS_AS(Vector(0, 0, 0).normalised(), std::invalid_argument);
}
