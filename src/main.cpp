#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "core/VmLoader.h"
#include "core/InstructionFactory.h"
#include "core/VMContext.h"

int main(int argc, char* argv[]) {
    std::string filePath = argv[1];

    try {
        std::vector<uint32_t> rawCode = VmLoader::loadBinaryFile(filePath);

        InstructionFactory factory;
        std::vector<std::unique_ptr<IInstruction>> program = factory.createProgram(rawCode);

        VMContext vm;
        vm.loadProgram(std::move(program));
        vm.run();
    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}