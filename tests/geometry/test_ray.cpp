#include "geometry/ray.h"
#include "support/assertions.h"

#include <stdexcept>

TEST_CASE("a ray evaluates origin plus t times direction", "[ray]") {
    const Ray ray(Vector(0, 8, 1), Vector(3, -4, 2));
    require_vectors_equal(ray.at(0), Vector(0, 8, 1));
    require_vectors_equal(ray.at(2), Vector(6, 0, 5));
    require_vectors_equal(ray.origin(), Vector(0, 8, 1));
    require_vectors_equal(ray.direction(), Vector(3, -4, 2));
}

TEST_CASE("a ray rejects a direction with no length", "[ray]") {
    REQUIRE_THROWS_AS(Ray(Vector(0, 0, 0), Vector(0, 0, 0)), std::invalid_argument);
}
