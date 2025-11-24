#include "instructions/MovInstruction.h"
#include "core/VMContext.h"

MovInstruction::MovInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

void MovInstruction::execute(VMContext& context) {
    uint8_t valueToMove = 0;
    FlagType flag = static_cast<FlagType>(m_flag);
    if (flag == FlagType::REG_REG) {
        valueToMove = context.getRegister(m_src);
    } else if (flag == FlagType::REG_VAL) {
        valueToMove = m_src;
    }
    context.setRegister(m_dest, valueToMove);
    context.incrementPC();
}
