#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class PrintInstruction : public IInstruction {
public:
    PrintInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    void execute(VMContext& context) override;
};
