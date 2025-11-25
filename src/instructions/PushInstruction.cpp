#include "instructions/PushInstruction.h"
#include "core/VMContext.h"

PushInstruction::PushInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

ExecutionResult PushInstruction::execute(VMContext& context) {
    uint8_t value = resolveValue(context, m_dest);

    context.pushStack(value);
    return ExecutionResult::Next;
}
