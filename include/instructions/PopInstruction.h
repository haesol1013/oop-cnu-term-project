#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class PopInstruction : public IInstruction {
public:
    PopInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    void execute(VMContext& context) override;
};
