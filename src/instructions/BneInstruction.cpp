#include "instructions/BneInstruction.h"
#include "core/VMContext.h"

BneInstruction::BneInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

ExecutionResult BneInstruction::execute(VMContext& context) {
    if (!context.getFlag(RegisterID::ZF)) {
        uint8_t jumpAddress = resolveValue(context, m_dest);
        context.setPC(jumpAddress);
        return ExecutionResult::Jumped;
    }
    return ExecutionResult::Next;
}
