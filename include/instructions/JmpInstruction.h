#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class JmpInstruction : public IInstruction {
public:
    JmpInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    ExecutionResult execute(VMContext& context) override;
};
