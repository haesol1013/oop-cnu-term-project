#pragma once
#include <stdexcept>
#include <string>

class VMException : public std::runtime_error {
public:
    explicit VMException(const std::string& message, int pcIndex = -1)
        : std::runtime_error(message), m_pcIndex(pcIndex) {}

    int getPCIndex() const {
        return m_pcIndex;
    }

    std::string getFullMessage() const {
        if (m_pcIndex != -1) {
            return "Runtime Error at instruction [" + std::to_string(m_pcIndex) + "]: " + what();
        }
        return "Load/Parse Error: " + std::string(what());
    }

private:
    int m_pcIndex;
};
