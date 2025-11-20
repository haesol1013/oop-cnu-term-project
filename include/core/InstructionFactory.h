#pragma once
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <cstdint>
#include "core/IInstruction.h"

struct ParsedInstruction {
    uint8_t opcode;
    uint8_t flag;
    uint8_t src;
    uint8_t dest;
};

class InstructionFactory {
public:
    InstructionFactory();
    std::vector<std::unique_ptr<IInstruction>> createProgram(
        const std::vector<uint32_t>& rawByteStream
    );

private:
    static ParsedInstruction parseRaw(uint32_t raw);

    using CreateFunc = std::function<
        std::unique_ptr<IInstruction>(uint8_t, uint8_t, uint8_t)
    >;
    std::map<uint8_t, CreateFunc> m_registry;
};
