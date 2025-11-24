#pragma once
#include <cstdint>

class VMContext;

class IInstruction {
public:
    IInstruction(uint8_t flag, uint8_t src, uint8_t dest)
        : m_flag(flag), m_src(src), m_dest(dest) {}
    virtual ~IInstruction() = default;
    virtual void execute(VMContext& context) = 0;

protected:
    uint8_t resolveValue(const VMContext& context, uint8_t operand) const;

    uint8_t m_flag;
    uint8_t m_src;
    uint8_t m_dest;
};
