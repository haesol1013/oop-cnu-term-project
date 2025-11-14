#include "instructions/PopInstruction.h"
#include "core/VMContext.h"

PopInstruction::PopInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : m_dest(dest) {}

void PopInstruction::execute(VMContext& context) {
    uint8_t value = context.popStack();
    context.setRegister(m_dest, value);
    context.incrementPC();
}