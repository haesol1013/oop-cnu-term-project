#pragma once
#include <string>
#include <vector>
#include <cstdint>

class VmLoader {
public:
    VmLoader() = default;

    static std::vector<uint32_t> loadBinaryFile(const std::string& filePath);
};