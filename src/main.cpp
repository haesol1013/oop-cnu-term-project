#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "core/VmLoader.h"
#include "core/InstructionFactory.h"
#include "core/VMContext.h"
#include "core/VMException.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_bin_file" << std::endl;
        return 1;
    }
    std::string filePath = argv[1];

    try {
        std::vector<uint32_t> rawCode = VmLoader::loadBinaryFile(filePath);

        InstructionFactory factory;
        std::vector<std::unique_ptr<IInstruction>> program = factory.createProgram(rawCode);

        VMContext vm;
        vm.loadProgram(std::move(program));
        vm.run();
    } catch (const VMException& e) {
        std::cerr << "[VM Error] " << e.getFullMessage() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[System Error] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
