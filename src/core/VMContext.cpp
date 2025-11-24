#include "core/VMContext.h"
#include "core/VMException.h"
#include <stdexcept>

VMContext::VMContext() : m_registers{}, m_stackMemory{} {
    m_registers[static_cast<uint8_t>(RegisterID::R0)] = 0;
    m_registers[static_cast<uint8_t>(RegisterID::R1)] = 0;
    m_registers[static_cast<uint8_t>(RegisterID::R2)] = 0;
    m_registers[static_cast<uint8_t>(RegisterID::PC)] = 0;
    m_registers[static_cast<uint8_t>(RegisterID::ZF)] = 0;
    m_registers[static_cast<uint8_t>(RegisterID::CF)] = 0;
    m_registers[static_cast<uint8_t>(RegisterID::OF)] = 0;

    m_registers[static_cast<uint8_t>(RegisterID::SP)] = STACK_SIZE - 1;
    m_registers[static_cast<uint8_t>(RegisterID::BP)] = STACK_SIZE - 1;
}

void VMContext::loadProgram(std::vector<std::unique_ptr<IInstruction>> program) {
    if (program.size() > 255) {
        throw std::runtime_error("Program too large: Max 255 instructions allowed.");
    }
    m_program = std::move(program);
}

void VMContext::run() {
    uint8_t& pc = m_registers[static_cast<uint8_t>(RegisterID::PC)];

    try {
        while (pc < m_program.size()) {
            if (!m_program[pc]) {
                throw std::runtime_error("Null instruction pointer encountered at index " + std::to_string(pc));
            }

            IInstruction* currentInstruction = m_program[pc].get();
            currentInstruction->execute(*this);
            if (pc > m_program.size()) {
                throw std::runtime_error("Program Counter out of bounds: " + std::to_string(pc));
            }
        }
    } catch (const std::exception& e) {
        if (dynamic_cast<const VMException*>(&e)) {
            throw;
        }
        throw VMException(e.what(), static_cast<int>(pc));
    }

}

uint8_t VMContext::getRegister(uint8_t regId) const {
    if (regId >= m_registers.size()) {
        throw std::runtime_error("Error: Accessing invalid register");
    }
    return m_registers[regId];
}

void VMContext::setRegister(uint8_t regId, uint8_t value) {
    if (regId >= m_registers.size()) {
        throw std::runtime_error("Error: Accessing invalid register");
    }
    m_registers[regId] = value;
}

bool VMContext::getFlag(RegisterID flag) const {
    return getRegister(static_cast<uint8_t>(flag)) == 1;
}

void VMContext::setFlag(RegisterID flag, bool value) {
    m_registers[static_cast<uint8_t>(flag)] = (value ? 1 : 0);
}

void VMContext::pushStack(uint8_t value) {
    uint8_t& sp = m_registers[static_cast<uint8_t>(RegisterID::SP)];
    if (sp == 0) {
        throw std::runtime_error("Error: Stack Overflow");
    }
    sp--;
    m_stackMemory[sp] = value;
}

uint8_t VMContext::popStack() {
    uint8_t& sp = m_registers[static_cast<uint8_t>(RegisterID::SP)];
    if (sp == STACK_SIZE - 1) {
        throw std::runtime_error("Error: Stack Underflow");
    }
    uint8_t value = m_stackMemory[sp];
    sp++;
    return value;
}

void VMContext::incrementPC() {
    m_registers[static_cast<uint8_t>(RegisterID::PC)]++;
}

void VMContext::setPC(uint8_t address) {
    m_registers[static_cast<uint8_t>(RegisterID::PC)] = address;
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
