#include "core/IInstruction.h"
#include "core/VMContext.h"
#include "Enums.h"

uint8_t IInstruction::resolveValue(const VMContext& context, uint8_t operand) const {
    auto flag = static_cast<FlagType>(m_flag);
    switch (flag) {
        case FlagType::REG_REG:
        case FlagType::SINGLE_REG:
            return context.getRegister(operand);
        case FlagType::REG_VAL:
        case FlagType::SINGLE_VAL:
            return operand;
        default:
            return operand;
    }
}
