#pragma once
#include <string>
#include <vector>
#include <cstdint>

class VMLoader {
public:
    VMLoader() = default;

    static std::vector<uint32_t> loadBinaryFile(const std::string& filePath);
};
