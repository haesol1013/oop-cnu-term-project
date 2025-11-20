#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Enums.h"
#include "core/IInstruction.h"

class VMContext {
public:
    VMContext();
    void loadProgram(std::vector<std::unique_ptr<IInstruction>> program);
    void run();

    uint8_t getRegister(uint8_t regId) const;
    void setRegister(uint8_t regId, uint8_t value);

    bool getFlag(RegisterID flag) const;
    void setFlag(RegisterID flag, bool value);

    void pushStack(uint8_t value);
    uint8_t popStack();

    void incrementPC();
    void setPC(uint8_t address);

    void updateFlags(uint8_t result, bool carry, bool overflow);

    void updateCmpFlags(int16_t result);

private:
    std::unordered_map<uint8_t, uint8_t> m_registers;
    std::vector<uint8_t> m_stackMemory;
    std::vector<std::unique_ptr<IInstruction>> m_program;
};
