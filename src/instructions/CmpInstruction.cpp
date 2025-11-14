#include "instructions/CmpInstruction.h"
#include "core/VMContext.h"

CmpInstruction::CmpInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : m_flag(flag), m_src(src), m_dest(dest) {}

void CmpInstruction::execute(VMContext& context) {
    uint8_t val1_unsigned = context.getRegister(m_dest);
    uint8_t val2_unsigned = 0;
    FlagType flag = static_cast<FlagType>(m_flag);
    if (flag == FlagType::REG_REG) {
        val2_unsigned = context.getRegister(m_src);
    } else if (flag == FlagType::REG_VAL) {
        val2_unsigned = m_src;
    }

    int16_t result = static_cast<int16_t>(static_cast<int8_t>(val1_unsigned)) -
                     static_cast<int16_t>(static_cast<int8_t>(val2_unsigned));

    context.updateCmpFlags(result);
    context.incrementPC();
}