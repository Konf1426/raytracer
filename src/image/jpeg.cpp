#include "image/jpeg.h"

#include <stb_image_write.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <vector>

static unsigned char encode_channel(double linear) {
    if (!std::isfinite(linear)) {
        throw std::invalid_argument("JPEG channels must be finite");
    }
    return static_cast<unsigned char>(std::lround(255.0 * std::sqrt(std::clamp(linear, 0.0, 1.0))));
}

static void write_bytes(void* context, void* bytes, int size) {
    auto& output = *static_cast<std::ofstream*>(context);
    output.write(static_cast<const char*>(bytes), size);
}

void save_jpeg(const Image& image, const std::filesystem::path& path, int quality) {
    if (quality < 1 || quality > 100) {
        throw std::invalid_argument("JPEG quality must be between 1 and 100");
    }
    std::vector<unsigned char> pixels;
    pixels.reserve(static_cast<std::size_t>(image.width()) * image.height() * 3);
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const Colour& colour = image.at(x, y);
            pixels.push_back(encode_channel(colour.r()));
            pixels.push_back(encode_channel(colour.g()));
            pixels.push_back(encode_channel(colour.b()));
        }
    }

    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Could not open output file: " + path.string());
    }
    const int result = stbi_write_jpg_to_func(write_bytes, &output, image.width(), image.height(),
                                              3, pixels.data(), quality);
    output.close();
    if (!result || !output) {
        throw std::runtime_error("Could not finish writing JPEG file: " + path.string());
    }
}
