#pragma once
#include "core/IInstruction.h"
#include <cstdint>

class AddInstruction : public IInstruction {
public:
    AddInstruction(uint8_t flag, uint8_t src, uint8_t dest);
    void execute(VMContext& context) override;
private:
    uint8_t m_flag, m_src, m_dest;
};