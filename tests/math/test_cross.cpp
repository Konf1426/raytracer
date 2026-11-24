#include "math/vector.h"
#include "support/assertions.h"

TEST_CASE("cross products follow the right hand rule", "[vector][cross]") {
    require_vectors_equal(Vector(1, 0, 0).cross(Vector(0, 1, 0)), Vector(0, 0, 1));
    require_vectors_equal(Vector(0, 1, 0).cross(Vector(1, 0, 0)), Vector(0, 0, -1));
}

TEST_CASE("cross products use all coordinates without changing operands", "[vector][cross]") {
    const Vector a(1, 2, 3);
    const Vector b(4, 5, 6);
    // (2*6 - 3*5, 3*4 - 1*6, 1*5 - 2*4).
    require_vectors_equal(a.cross(b), Vector(-3, 6, -3));
    require_vectors_equal(a, Vector(1, 2, 3));
    require_vectors_equal(b, Vector(4, 5, 6));
}

TEST_CASE("parallel and zero vectors have zero cross products", "[vector][cross]") {
    require_vectors_equal(Vector(1, 2, 3).cross(Vector(2, 4, 6)), Vector(0, 0, 0));
    require_vectors_equal(Vector(0, 0, 0).cross(Vector(4, 5, 6)), Vector(0, 0, 0));
}
