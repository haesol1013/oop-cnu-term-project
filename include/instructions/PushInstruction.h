#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class PushInstruction : public IInstruction {
public:
    PushInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    void execute(VMContext& context) override;
};
