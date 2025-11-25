#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class CmpInstruction : public IInstruction {
public:
    CmpInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    ExecutionResult execute(VMContext& context) override;
};
