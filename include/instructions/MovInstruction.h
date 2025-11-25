#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class MovInstruction : public IInstruction {
public:
    MovInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    ExecutionResult execute(VMContext& context) override;
};
