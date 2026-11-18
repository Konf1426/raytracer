#include "image/image.h"
#include "image/ppm.h"
#include "support/assertions.h"

#include <filesystem>
#include <sstream>
#include <stdexcept>

TEST_CASE("an image stores independent pixels starting with black", "[image]") {
    Image image(2, 3);
    REQUIRE(image.width() == 2);
    REQUIRE(image.height() == 3);
    require_colours_equal(image.at(1, 2), Colour(0, 0, 0));
    image.set(1, 2, Colour(0.1, 0.2, 0.3));
    require_colours_equal(image.at(1, 2), Colour(0.1, 0.2, 0.3));
    require_colours_equal(image.at(0, 2), Colour(0, 0, 0));
}

TEST_CASE("an image validates its dimensions and pixel bounds", "[image]") {
    REQUIRE_THROWS_AS(Image(0, 1), std::invalid_argument);
    REQUIRE_THROWS_AS(Image(1, -1), std::invalid_argument);
    Image image(2, 2);
    REQUIRE_THROWS_AS(image.at(-1, 0), std::out_of_range);
    REQUIRE_THROWS_AS(image.at(2, 0), std::out_of_range);
    REQUIRE_THROWS_AS(image.set(0, 2, Colour()), std::out_of_range);
}

TEST_CASE("PPM output clamps linear colours and applies gamma two", "[image][ppm]") {
    Image image(2, 1);
    image.set(0, 0, Colour(1, 0.25, -1));
    image.set(1, 0, Colour(4, 0, 1));
    std::ostringstream output;
    write_ppm(image, output);
    REQUIRE(output.str() == "P3\n2 1\n255\n255 128 0\n255 0 255\n");
}

TEST_CASE("PPM output reports stream and file failures", "[image][ppm]") {
    const Image image(1, 1);
    std::ostringstream broken;
    broken.setstate(std::ios::badbit);
    REQUIRE_THROWS_AS(write_ppm(image, broken), std::runtime_error);
    REQUIRE_THROWS_AS(save_ppm(image, std::filesystem::temp_directory_path()), std::runtime_error);
}
