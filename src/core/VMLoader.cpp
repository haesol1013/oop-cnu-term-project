#include "core/VMLoader.h"
#include <fstream>
#include <stdexcept>

std::vector<uint32_t> VMLoader::loadBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        throw std::runtime_error("Error: Cannot open file " + filePath);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size % 4 != 0) {
        file.close();
        throw std::runtime_error("Error: File size is not a multiple of 4 bytes.");
    }

    if (size == 0) {
        file.close();
        return {};
    }

    std::vector<uint32_t> rawProgram(size / 4);

    if (!file.read(reinterpret_cast<char*>(rawProgram.data()), size)) {
        file.close();
        throw std::runtime_error("Error: Failed to read file " + filePath);
    }

    file.close();

    return rawProgram;
}
