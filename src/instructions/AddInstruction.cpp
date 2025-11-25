#include "instructions/AddInstruction.h"
#include "core/VMContext.h"

AddInstruction::AddInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

ExecutionResult AddInstruction::execute(VMContext& context) {
    uint8_t val1 = context.getRegister(m_dest);
    uint8_t val2 = resolveValue(context, m_src);

    uint16_t result = static_cast<uint16_t>(val1) + static_cast<uint16_t>(val2);
    int16_t signedResult = static_cast<int16_t>(static_cast<int8_t>(val1)) + static_cast<int16_t>(static_cast<int8_t>(val2));
    
    auto finalResult = static_cast<uint8_t>(result);
    bool carry = (result > 0xFF);
    bool overflow = (signedResult > 127 || signedResult < -128);

    context.setRegister(m_dest, finalResult);
    context.updateFlags(finalResult, carry, overflow);
    return ExecutionResult::Next;
}
