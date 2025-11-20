#include "instructions/PushInstruction.h"
#include "core/VMContext.h"

PushInstruction::PushInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : m_flag(flag), m_dest(dest) {}

void PushInstruction::execute(VMContext& context) {
    uint8_t value = context.getRegister(m_dest);
    FlagType flag = static_cast<FlagType>(m_flag);

    if (flag == FlagType::SINGLE_REG) {
        value = context.getRegister(m_dest);
    } else if (flag == FlagType::SINGLE_VAL) {
        value = m_dest;
    }

    context.pushStack(value);
    context.incrementPC();
}
