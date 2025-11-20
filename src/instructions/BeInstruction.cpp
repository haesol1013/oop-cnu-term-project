#include "instructions/BeInstruction.h"
#include "core/VMContext.h"

BeInstruction::BeInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : m_flag(flag), m_dest(dest) {}

void BeInstruction::execute(VMContext& context) {
    if (context.getFlag(RegisterID::ZF)) {
        uint8_t jumpAddress = 0;
        FlagType flag = static_cast<FlagType>(m_flag);
        if (flag == FlagType::SINGLE_REG) {
            jumpAddress = context.getRegister(m_dest);
        } else if (flag == FlagType::SINGLE_VAL) {
            jumpAddress = m_dest;
        }
        context.setPC(jumpAddress);
    } else {
        context.incrementPC();
    }
}
