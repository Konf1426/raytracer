#include "image/ppm.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <ostream>
#include <stdexcept>

static int encode_channel(double linear) {
    return static_cast<int>(std::lround(255.0 * std::sqrt(std::clamp(linear, 0.0, 1.0))));
}

void write_ppm(const Image& image, std::ostream& output) {
    output << "P3\n" << image.width() << ' ' << image.height() << "\n255\n";
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            const Colour& colour = image.at(x, y);
            output << encode_channel(colour.r()) << ' ' << encode_channel(colour.g()) << ' '
                   << encode_channel(colour.b()) << '\n';
        }
    }
    if (!output) {
        throw std::runtime_error("Could not write the PPM image");
    }
}

void save_ppm(const Image& image, const std::filesystem::path& path) {
    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("Could not open output file: " + path.string());
    }
    write_ppm(image, output);
    output.close();
    if (!output) {
        throw std::runtime_error("Could not finish writing output file: " + path.string());
    }
}
