#include "instructions/MulInstruction.h"
#include "core/VMContext.h"

MulInstruction::MulInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

void MulInstruction::execute(VMContext& context) {
    uint8_t val1 = context.getRegister(m_dest);
    uint8_t val2 = resolveValue(context, m_src);

    uint16_t result = static_cast<uint16_t>(val1) * static_cast<uint16_t>(val2);

    auto finalResult = static_cast<uint8_t>(result);
    bool carryOrOverflow = (result > 0xFF);

    context.setRegister(m_dest, finalResult);
    context.updateFlags(finalResult, carryOrOverflow, carryOrOverflow);
    context.incrementPC();
}
