#include "core/InstructionFactory.h"
#include <stdexcept>

#include "Enums.h"
#include "instructions/MovInstruction.h"
#include "instructions/AddInstruction.h"
#include "instructions/SubInstruction.h"
#include "instructions/MulInstruction.h"
#include "instructions/CmpInstruction.h"
#include "instructions/PushInstruction.h"
#include "instructions/PopInstruction.h"
#include "instructions/JmpInstruction.h"
#include "instructions/BeInstruction.h"
#include "instructions/BneInstruction.h"
#include "instructions/PrintInstruction.h"


InstructionFactory::InstructionFactory() {

    m_registry[static_cast<uint8_t>(OpCode::MOV)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<MovInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::ADD)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<AddInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::SUB)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<SubInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::MUL)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<MulInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::CMP)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<CmpInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::PUSH)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<PushInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::POP)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<PopInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::JMP)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<JmpInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::BE)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<BeInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::BNE)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<BneInstruction>(f, s, d);
        };

    m_registry[static_cast<uint8_t>(OpCode::PRINT)] =
        [](uint8_t f, uint8_t s, uint8_t d) {
            return std::make_unique<PrintInstruction>(f, s, d);
        };
}

std::vector<std::unique_ptr<IInstruction>> InstructionFactory::createProgram(
    const std::vector<uint32_t>& rawByteStream
) {
    std::vector<std::unique_ptr<IInstruction>> program;
    program.reserve(rawByteStream.size());

    for (const uint32_t& raw : rawByteStream) {
        ParsedInstruction parsed = parseRaw(raw);

        auto it = m_registry.find(parsed.opcode);
        if (it == m_registry.end()) {
            throw std::runtime_error("Error: Unknown Opcode encountered: 0x" + std::to_string(parsed.opcode));
        }

        CreateFunc& creator = it->second;
        program.push_back(creator(parsed.flag, parsed.src, parsed.dest));
    }

    return program;
}

ParsedInstruction InstructionFactory::parseRaw(uint32_t raw) {
    ParsedInstruction p;
    uint8_t byte0 = (raw >> 0) & 0xFF;
    p.opcode = byte0 >> 2;
    p.flag = byte0 & 0x03;
    p.src = (raw >> 16) & 0xFF;
    p.dest = (raw >> 24) & 0xFF;
    return p;
}
