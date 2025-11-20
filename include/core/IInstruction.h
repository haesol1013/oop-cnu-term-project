#pragma once

class VMContext;

class IInstruction {
public:
    virtual ~IInstruction() = default;
    virtual void execute(VMContext& context) = 0;
};
