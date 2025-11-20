#include "instructions/MulInstruction.h"
#include "core/VMContext.h"

MulInstruction::MulInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : m_flag(flag), m_src(src), m_dest(dest) {}

void MulInstruction::execute(VMContext& context) {
    uint8_t val1 = context.getRegister(m_dest);
    uint8_t val2 = 0;
    FlagType flag = static_cast<FlagType>(m_flag);
    if (flag == FlagType::REG_REG) {
        val2 = context.getRegister(m_src);
    } else if (flag == FlagType::REG_VAL) {
        val2 = m_src;
    }

    uint16_t result = static_cast<uint16_t>(val1) * static_cast<uint16_t>(val2);

    uint8_t finalResult = static_cast<uint8_t>(result);
    bool carryOrOverflow = (result > 0xFF);

    context.setRegister(m_dest, finalResult);
    context.updateFlags(finalResult, carryOrOverflow, carryOrOverflow);
    context.incrementPC();
}
