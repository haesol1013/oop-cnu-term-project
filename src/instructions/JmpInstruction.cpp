#include "instructions/JmpInstruction.h"
#include "core/VMContext.h"

JmpInstruction::JmpInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

ExecutionResult JmpInstruction::execute(VMContext& context) {
    uint8_t jumpAddress = resolveValue(context, m_dest);
    
    context.setPC(jumpAddress);
    return ExecutionResult::Jumped;
}
