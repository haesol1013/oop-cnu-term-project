#include "instructions/JmpInstruction.h"
#include "core/VMContext.h"

JmpInstruction::JmpInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

void JmpInstruction::execute(VMContext& context) {
    uint8_t jumpAddress = 0;
    FlagType flag = static_cast<FlagType>(m_flag);
    
    if (flag == FlagType::SINGLE_REG) {
        jumpAddress = context.getRegister(m_dest);
    } else if (flag == FlagType::SINGLE_VAL) {
        jumpAddress = m_dest;
    }
    
    context.setPC(jumpAddress);
}
