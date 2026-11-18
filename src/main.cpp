#include "geometry/plane.h"
#include "geometry/sphere.h"
#include "image/ppm.h"
#include "render/renderer.h"

#include <charconv>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>

static int parse_dimension(std::string_view text) {
    int value = 0;
    const auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), value);
    if (error != std::errc() || end != text.data() + text.size() || value < 1 || value > 4096) {
        throw std::invalid_argument("Width and height must be integers between 1 and 4096");
    }
    return value;
}

static void print_usage() {
    std::cout << "Usage: raytracer [output.ppm [width height]]\n"
                 "Defaults: render.ppm, 640 x 360. Maximum dimension: 4096.\n";
}

int main(int argc, char* argv[]) {
    try {
        if (argc == 2 && std::string_view(argv[1]) == "--help") {
            print_usage();
            return 0;
        }
        if (argc != 1 && argc != 2 && argc != 4) {
            print_usage();
            throw std::invalid_argument("Supply an output path, or a path followed by width and height");
        }

        const std::filesystem::path output = argc >= 2 ? argv[1] : "render.ppm";
        const int width = argc == 4 ? parse_dimension(argv[2]) : 640;
        const int height = argc == 4 ? parse_dimension(argv[3]) : 360;

        Scene scene;
        scene.add(std::make_unique<Sphere>(Vector(0, 1, 0), 1, Colour(0.75, 0.12, 0.06)));
        scene.add(std::make_unique<Plane>(Vector(0, 0, 0), Vector(0, 1, 0), Colour(0.55, 0.6, 0.48)));
        const Camera camera(width, height, Vector(0, 1.4, 4), 2.0, 1.5);
        save_ppm(Renderer().render(scene, camera), output);
        std::cout << "Rendered " << width << " x " << height << " to " << output.string() << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
