#include "core/VMContext.h"
#include "core/VMException.h"
#include <stdexcept>

VMContext::VMContext() : m_registers{}, m_stackMemory{} {
    setRegister(RegisterID::R0, 0);
    setRegister(RegisterID::R1, 0);
    setRegister(RegisterID::R2, 0);
    setRegister(RegisterID::PC, 0);
    setRegister(RegisterID::ZF, 0);
    setRegister(RegisterID::CF, 0);
    setRegister(RegisterID::OF, 0);

    setRegister(RegisterID::SP, STACK_SIZE - 1);
    setRegister(RegisterID::BP, STACK_SIZE - 1);
}

void VMContext::loadProgram(std::vector<std::unique_ptr<IInstruction>> program) {
    if (program.size() > 255) {
        throw std::runtime_error("Program too large: Max 255 instructions allowed.");
    }
    m_program = std::move(program);
}

void VMContext::run() {
    try {
        while (true) {
            uint8_t pc = getRegister(RegisterID::PC);
            if (pc >= m_program.size()) {
                 break; 
            }

            if (!m_program[pc]) {
                throw std::runtime_error("Null instruction pointer encountered at index " + std::to_string(pc));
            }

            IInstruction* currentInstruction = m_program[pc].get();
            currentInstruction->execute(*this);

            if (getRegister(RegisterID::PC) > m_program.size()) {
                 throw std::runtime_error("Program Counter out of bounds: " + std::to_string(getRegister(RegisterID::PC)));
            }
        }
    } catch (const std::exception& e) {
        if (dynamic_cast<const VMException*>(&e)) {
            throw;
        }
        throw VMException(e.what(), static_cast<int>(getRegister(RegisterID::PC)));
    }

}

uint8_t VMContext::getRegister(uint8_t regId) const {
    if (regId >= m_registers.size()) {
        throw std::runtime_error("Error: Accessing invalid register");
    }
    return m_registers[regId];
}

uint8_t VMContext::getRegister(RegisterID regId) const {
    return getRegister(static_cast<uint8_t>(regId));
}

void VMContext::setRegister(uint8_t regId, uint8_t value) {
    if (regId >= m_registers.size()) {
        throw std::runtime_error("Error: Accessing invalid register");
    }
    m_registers[regId] = value;
}

void VMContext::setRegister(RegisterID regId, uint8_t value) {
    setRegister(static_cast<uint8_t>(regId), value);
}

bool VMContext::getFlag(const RegisterID flag) const {
    return getRegister(flag) == 1;
}

void VMContext::setFlag(const RegisterID flag, bool value) {
    setRegister(flag, (value ? 1 : 0));
}

void VMContext::pushStack(uint8_t value) {
    uint8_t sp = getRegister(RegisterID::SP);
    if (sp == 0) {
        throw std::runtime_error("Error: Stack Overflow");
    }
    sp--;
    m_stackMemory[sp] = value;
    setRegister(RegisterID::SP, sp);
}

uint8_t VMContext::popStack() {
    uint8_t sp = getRegister(RegisterID::SP);
    if (sp == STACK_SIZE - 1) {
        throw std::runtime_error("Error: Stack Underflow");
    }
    uint8_t value = m_stackMemory[sp];
    sp++;
    setRegister(RegisterID::SP, sp);
    return value;
}

void VMContext::incrementPC() {
    setRegister(RegisterID::PC, getRegister(RegisterID::PC) + 1);
}

void VMContext::setPC(uint8_t address) {
    setRegister(RegisterID::PC, address);
}

void VMContext::updateFlags(uint8_t result, bool carry, bool overflow) {
    setFlag(RegisterID::ZF, result == 0);
    setFlag(RegisterID::CF, carry);
    setFlag(RegisterID::OF, overflow);
}

void VMContext::updateCmpFlags(int16_t result) {
    setFlag(RegisterID::ZF, result == 0);
    setFlag(RegisterID::CF, result > 0);
    setFlag(RegisterID::OF, result < 0);
}
