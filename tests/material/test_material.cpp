#include "material/material.h"
#include "support/assertions.h"

#include <limits>
#include <stdexcept>
#include <type_traits>

TEST_CASE("a material stores its colour and requires explicit composition", "[material]") {
    STATIC_REQUIRE_FALSE(std::is_convertible_v<Colour, Material>);
    const Material orange(Colour(1, 0.5, 0));
    require_colours_equal(orange.colour(), Colour(1, 0.5, 0));
}

TEST_CASE("a matte material facing the light returns its whole colour", "[material]") {
    const Material orange(Colour(1, 0.5, 0));
    require_colours_equal(orange.shade(Vector(0, 0, -1), Vector(0, 0, -1)),
                          Colour(1, 0.5, 0));
}

TEST_CASE("a matte material scales every channel by the hand calculated cosine", "[material]") {
    const Material orange(Colour(1, 0.5, 0));
    // Both vectors are unit length: N dot L = (-1)*(-0.8) = 0.8.
    require_colours_equal(orange.shade(Vector(0, 0, -1), Vector(0.6, 0, -0.8)),
                          Colour(0.8, 0.4, 0));
    require_colours_equal(orange.colour(), Colour(1, 0.5, 0));
}

TEST_CASE("a matte material at grazing incidence receives no direct light", "[material]") {
    const Material orange(Colour(1, 0.5, 0));
    require_colours_equal(orange.shade(Vector(0, 0, -1), Vector(1, 0, 0)), Colour(0, 0, 0));
}

TEST_CASE("a matte material facing away clamps negative direct light to zero", "[material]") {
    const Material orange(Colour(1, 0.5, 0));
    require_colours_equal(orange.shade(Vector(0, 0, -1), Vector(0, 0, 1)), Colour(0, 0, 0));
}

TEST_CASE("a material keeps the existing ambient contribution in shade and shadow", "[material]") {
    const Material material(Colour(0.8, 0.4, 0.2));
    const Vector normal(0, 1, 0);
    // At ambient 0.15 and cosine 0.6: brightness = 0.15 + 0.85*0.6 = 0.66.
    require_colours_equal(material.shade(normal, Vector(0.8, 0.6, 0), 0.15),
                          Colour(0.528, 0.264, 0.132));
    require_colours_equal(material.shade(normal, Vector(1, 0, 0), 0.15),
                          Colour(0.12, 0.06, 0.03));
    require_colours_equal(material.shade(normal, Vector(0, -1, 0), 0.15),
                          Colour(0.12, 0.06, 0.03));
    require_colours_equal(material.shade(normal, Vector(0, 1, 0), 0.15, true),
                          Colour(0.12, 0.06, 0.03));
    require_colours_equal(material.shade(normal, Vector(0, 1, 0), 0, true), Colour(0, 0, 0));
    require_colours_equal(material.shade(normal, Vector(1, 0, 0), 1), Colour(0.8, 0.4, 0.2));
}

TEST_CASE("a material rejects invalid ambient fractions", "[material]") {
    const Material material(Colour(1, 0.5, 0));
    for (const double invalid : {-0.1, 1.1, std::numeric_limits<double>::infinity(),
                                 std::numeric_limits<double>::quiet_NaN()}) {
        REQUIRE_THROWS_AS(material.shade(Vector(0, 1, 0), Vector(0, 1, 0), invalid),
                          std::invalid_argument);
    }
}
