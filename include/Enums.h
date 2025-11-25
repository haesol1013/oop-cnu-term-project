#pragma once
#include <cstdint>

constexpr size_t REGISTER_COUNT = 10;

enum class RegisterID : uint8_t {
    R0 = 0x01,
    R1 = 0x02,
    R2 = 0x03,
    PC = 0x04,
    SP = 0x05,
    BP = 0x06,
    ZF = 0x07,
    CF = 0x08,
    OF = 0x09
};

enum class OpCode : uint8_t {
    MOV = 0x01,
    ADD = 0x02,
    SUB = 0x03,
    MUL = 0x04,
    CMP = 0x05,
    PUSH = 0x06,
    POP = 0x07,
    JMP = 0x08,
    BE = 0x09,
    BNE = 0x0A,
    PRINT = 0x0B
};

enum class FlagType : uint8_t {
    REG_REG = 0b00,
    REG_VAL = 0b01,
    SINGLE_REG = 0b10,
    SINGLE_VAL = 0b11
};
