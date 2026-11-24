#include "support/jpeg_fixture.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        if (argc != 4) {
            throw std::runtime_error("Expected JPEG path, width and height");
        }
        std::ifstream input(argv[1], std::ios::binary);
        if (input.get() != 0xff || input.get() != 0xd8) {
            throw std::runtime_error("Output does not have a JPEG signature");
        }
        const DecodedJpeg image(argv[1]);
        if (!image.pixels || image.channels != 3 || image.width != std::stoi(argv[2]) ||
            image.height != std::stoi(argv[3])) {
            throw std::runtime_error("Could not decode a JPEG with the expected dimensions");
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
