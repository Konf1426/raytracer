#pragma once

#include "image/image.h"

#include <filesystem>
#include <iosfwd>

void write_ppm(const Image& image, std::ostream& output);
void save_ppm(const Image& image, const std::filesystem::path& path);
