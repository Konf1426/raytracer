#include "render/camera.h"
#include "support/assertions.h"

#include <stdexcept>

TEST_CASE("the camera sends rays through pixel centres with the top row first", "[camera]") {
    const Camera camera(3, 3, Vector(1, 2, 3));
    const Ray centre = camera.ray_for_pixel(1, 1);
    require_vectors_equal(centre.origin(), Vector(1, 2, 3));
    require_vectors_equal(centre.direction(), Vector(0, 0, -1));
    require_vectors_equal(camera.ray_for_pixel(0, 0).direction(),
                          Vector(-2.0 / 3, 2.0 / 3, -1).normalised());
    require_vectors_equal(camera.ray_for_pixel(2, 2).direction(),
                          Vector(2.0 / 3, -2.0 / 3, -1).normalised());
}

TEST_CASE("the camera preserves aspect ratio", "[camera]") {
    const Camera camera(4, 2);
    require_vectors_equal(camera.ray_for_pixel(0, 0).direction(),
                          Vector(-1.5, 0.5, -1).normalised());
}

TEST_CASE("the camera rejects invalid dimensions and pixel coordinates", "[camera]") {
    REQUIRE_THROWS_AS(Camera(0, 2), std::invalid_argument);
    REQUIRE_THROWS_AS(Camera(2, -1), std::invalid_argument);
    REQUIRE_THROWS_AS(Camera(2, 2, Vector(0, 0, 0), 0), std::invalid_argument);
    REQUIRE_THROWS_AS(Camera(2, 2, Vector(0, 0, 0), 2, 0), std::invalid_argument);
    const Camera camera(2, 2);
    REQUIRE_THROWS_AS(camera.ray_for_pixel(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(camera.ray_for_pixel(0, 2), std::out_of_range);
}
