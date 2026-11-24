#pragma once

#include "image/image.h"

#include <filesystem>

// Write the whole image as JPEG, or throw std::runtime_error for file errors.
// Linear RGB is clamped and gamma-2 encoded. Quality is in [1, 100].
// Invalid quality or non-finite channels throw std::invalid_argument.
void save_jpeg(const Image& image, const std::filesystem::path& path, int quality = 95);
