#include "instructions/MovInstruction.h"
#include "core/VMContext.h"

MovInstruction::MovInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

void MovInstruction::execute(VMContext& context) {
    uint8_t valueToMove = resolveValue(context, m_src);
    context.setRegister(m_dest, valueToMove);
    context.incrementPC();
}
