#include "instructions/PrintInstruction.h"
#include "core/VMContext.h"
#include <iostream>

PrintInstruction::PrintInstruction(uint8_t flag, uint8_t src, uint8_t dest)
    : IInstruction(flag, src, dest) {}

ExecutionResult PrintInstruction::execute(VMContext& context) {
    uint8_t valueToPrint = resolveValue(context, m_dest);
    std::cout << std::to_string(static_cast<int8_t>(valueToPrint)) << std::endl;
    return ExecutionResult::Next;
}
