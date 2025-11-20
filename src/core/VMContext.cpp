#include "core/VMContext.h"
#include <stdexcept>

constexpr size_t STACK_SIZE = 256;

VMContext::VMContext() : m_stackMemory(STACK_SIZE) {
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
    m_program = std::move(program);
}

void VMContext::run() {
    uint8_t& pc = m_registers[static_cast<uint8_t>(RegisterID::PC)];

    while (pc < m_program.size()) {
        IInstruction* currentInstruction = m_program[pc].get();
        currentInstruction->execute(*this);
    }
}

uint8_t VMContext::getRegister(uint8_t regId) const {
    auto it = m_registers.find(regId);
    if (it == m_registers.end()) {
        throw std::runtime_error("Error: Accessing invalid register");
    }
    return it->second;
}

void VMContext::setRegister(uint8_t regId, uint8_t value) {
    auto it = m_registers.find(regId);
    if (it == m_registers.end()) {
        throw std::runtime_error("Error: Accessing invalid register");
    }
    it->second = value;
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
