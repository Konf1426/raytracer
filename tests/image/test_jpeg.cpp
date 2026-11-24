#include "image/jpeg.h"
#include "support/jpeg_fixture.h"

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <fstream>
#include <limits>
#include <stdexcept>

TEST_CASE("a saved JPEG decodes with odd rectangular dimensions and gamma two", "[jpeg]") {
    TemporaryJpeg file;
    Image image(17, 9);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set(x, y, Colour(0.25, 0.25, 0.25));
        }
    }
    save_jpeg(image, file.path());
    const DecodedJpeg decoded(file.path());
    REQUIRE(decoded.pixels);
    REQUIRE(decoded.width == 17);
    REQUIRE(decoded.height == 9);
    REQUIRE(decoded.channels == 3);
    // sqrt(0.25)*255 = 127.5, rounded to 128 before lossy compression.
    // A uniform neutral block avoids chroma mixing; allow two 8-bit levels
    // for JPEG quantisation and decoder rounding, not arbitrary scene error.
    for (int component = 0; component < 3; ++component) {
        REQUIRE(std::abs(decoded.channel(8, 4, component) - 128) <= 2);
    }
    std::ifstream input(file.path(), std::ios::binary);
    REQUIRE(input.get() == 0xff);
    REQUIRE(input.get() == 0xd8);
}

TEST_CASE("JPEG saving preserves RGB order top rows and channel clamping", "[jpeg]") {
    TemporaryJpeg file;
    Image image(17, 25);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            image.set(x, y, y < 16 ? Colour(4, 0.25, -1) : Colour(0, 0, 1));
        }
    }
    // Blocks meet at a JPEG block boundary. Quality 100 still loses precision;
    // allow three levels for RGB/YCrCb conversion and transform rounding.
    save_jpeg(image, file.path(), 100);
    const DecodedJpeg decoded(file.path());
    REQUIRE(decoded.pixels);
    REQUIRE(decoded.width == 17);
    REQUIRE(decoded.height == 25);
    REQUIRE(std::abs(decoded.channel(16, 0, 0) - 255) <= 3);
    REQUIRE(std::abs(decoded.channel(16, 0, 1) - 128) <= 3);
    REQUIRE(decoded.channel(16, 0, 2) <= 3);
    REQUIRE(decoded.channel(0, 24, 0) <= 3);
    REQUIRE(decoded.channel(0, 24, 1) <= 3);
    REQUIRE(std::abs(decoded.channel(0, 24, 2) - 255) <= 3);
}

TEST_CASE("JPEG saving reports output open failures", "[jpeg]") {
    TemporaryJpeg file;
    const Image image(1, 1);
    REQUIRE_THROWS_AS(save_jpeg(image, file.directory()), std::runtime_error);
    REQUIRE_THROWS_AS(save_jpeg(image, file.directory() / "missing" / "image.jpg"),
                      std::runtime_error);
}

TEST_CASE("JPEG quality is validated and both supported endpoints decode", "[jpeg]") {
    TemporaryJpeg file;
    const Image image(1, 1);
    REQUIRE_THROWS_AS(save_jpeg(image, file.path(), 0), std::invalid_argument);
    REQUIRE_THROWS_AS(save_jpeg(image, file.path(), 101), std::invalid_argument);
    for (const int quality : {1, 100}) {
        save_jpeg(image, file.path(), quality);
        const DecodedJpeg decoded(file.path());
        REQUIRE(decoded.pixels);
        REQUIRE(decoded.width == 1);
        REQUIRE(decoded.height == 1);
    }
}

TEST_CASE("JPEG saving rejects nonfinite channels instead of converting them to bytes", "[jpeg]") {
    TemporaryJpeg file;
    Image image(1, 1);
    for (const double invalid : {std::numeric_limits<double>::infinity(),
                                 -std::numeric_limits<double>::infinity(),
                                 std::numeric_limits<double>::quiet_NaN()}) {
        image.set(0, 0, Colour(invalid, 0, 0));
        REQUIRE_THROWS_AS(save_jpeg(image, file.path()), std::invalid_argument);
    }
}

TEST_CASE("JPEG saving reports delayed stream failures", "[jpeg]") {
    // Linux /dev/full opens successfully but rejects writes, unlike a bad path.
    if (std::filesystem::exists("/dev/full")) {
        REQUIRE_THROWS_AS(save_jpeg(Image(1, 1), "/dev/full"), std::runtime_error);
    } else {
        SKIP("No full-device fixture on this platform");
    }
}
