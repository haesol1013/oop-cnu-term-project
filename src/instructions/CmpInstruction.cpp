#include "instructions/CmpInstruction.h"
#include "core/VMContext.h"

CmpInstruction::CmpInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

void CmpInstruction::execute(VMContext& context) {
    uint8_t val1_unsigned = context.getRegister(m_dest);
    uint8_t val2_unsigned = resolveValue(context, m_src);

    int16_t result = static_cast<int16_t>(static_cast<int8_t>(val1_unsigned)) -
                     static_cast<int16_t>(static_cast<int8_t>(val2_unsigned));

    context.updateCmpFlags(result);
    context.incrementPC();
}
