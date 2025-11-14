cmake_minimum_required(VERSION 4.0)
project(oop_cnu_term_project)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CORE_SOURCES
    src/core/VMContext.cpp
    src/core/VmLoader.cpp
    src/core/InstructionFactory.cpp
)

set(INSTRUCTION_SOURCES
    src/instructions/MovInstruction.cpp
    src/instructions/AddInstruction.cpp
    src/instructions/SubInstruction.cpp
    src/instructions/MulInstruction.cpp
    src/instructions/CmpInstruction.cpp
    src/instructions/PushInstruction.cpp
    src/instructions/PopInstruction.cpp
    src/instructions/JmpInstruction.cpp
    src/instructions/BeInstruction.cpp
    src/instructions/BneInstruction.cpp
    src/instructions/PrintInstruction.cpp
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    ${CORE_SOURCES}
    ${INSTRUCTION_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)