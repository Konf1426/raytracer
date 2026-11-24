#pragma once

#include <stb_image.h>

#include <filesystem>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <system_error>

class TemporaryJpeg {
public:
    TemporaryJpeg() : _directory(std::filesystem::temp_directory_path() /
                                ("raytracer-jpeg-" + std::to_string(std::random_device{}()))) {
        if (!std::filesystem::create_directory(_directory)) {
            throw std::runtime_error("Could not create a unique JPEG test directory");
        }
    }
    ~TemporaryJpeg() {
        std::error_code error;
        std::filesystem::remove_all(_directory, error);
    }
    TemporaryJpeg(const TemporaryJpeg&) = delete;
    TemporaryJpeg& operator=(const TemporaryJpeg&) = delete;

    std::filesystem::path path() const { return _directory / "image.jpg"; }
    const std::filesystem::path& directory() const { return _directory; }

private:
    std::filesystem::path _directory;
};

struct DecodedJpeg {
    int width = 0;
    int height = 0;
    int channels = 0;
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> pixels{nullptr, &stbi_image_free};

    explicit DecodedJpeg(const std::filesystem::path& path)
        : pixels(stbi_load(path.string().c_str(), &width, &height, &channels, 3), &stbi_image_free) {}

    int channel(int x, int y, int component) const {
        return pixels.get()[3 * (y * width + x) + component];
    }
};
