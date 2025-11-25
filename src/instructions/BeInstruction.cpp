#include "instructions/BeInstruction.h"
#include "core/VMContext.h"

BeInstruction::BeInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

ExecutionResult BeInstruction::execute(VMContext& context) {
    if (context.getFlag(RegisterID::ZF)) {
        uint8_t jumpAddress = resolveValue(context, m_dest);
        context.setPC(jumpAddress);
        return ExecutionResult::Jumped;
    }
    return ExecutionResult::Next;
}
