# OOP CNU Term Project - Virtual Machine Simulator

## 📋 Project Overview

This project is a **Virtual Machine (VM) Simulator** implemented in C++ that executes custom bytecode instructions. It includes an assembler written in Python to convert human-readable assembly code into binary format that the VM can execute.

The VM supports basic operations including arithmetic operations, register management, stack operations, conditional branching, and output functionality.

## 🏗️ Project Structure

```
oop-cnu-term-project/
├── include/                    # Header files
│   ├── Enums.h                # Enumerations for opcodes, registers, and flags
│   ├── core/                  # Core VM components
│   │   ├── IInstruction.h     # Abstract instruction interface
│   │   ├── InstructionFactory.h  # Factory for creating instructions
│   │   ├── VMContext.h        # VM execution context
│   │   └── VmLoader.h         # Binary file loader
│   └── instructions/          # Concrete instruction implementations
│       ├── AddInstruction.h
│       ├── SubInstruction.h
│       ├── MulInstruction.h
│       ├── MovInstruction.h
│       ├── CmpInstruction.h
│       ├── PushInstruction.h
│       ├── PopInstruction.h
│       ├── JmpInstruction.h
│       ├── BeInstruction.h
│       ├── BneInstruction.h
│       └── PrintInstruction.h
├── src/                       # Implementation files
│   ├── main.cpp              # Entry point
│   ├── core/                 # Core implementations
│   └── instructions/         # Instruction implementations
├── test/                      # Test files and tools
│   ├── encode.py             # Python assembler
│   ├── text/                 # Assembly source files (.txt)
│   ├── bin/                  # Compiled binary files (.bin)
│   └── answer/               # Expected outputs
├── docs/                      # Project documentation
└── CMakeLists.txt            # CMake build configuration
```

## 🎨 Design Patterns Used

### 1. Strategy Pattern
- **Location**: `IInstruction` interface and concrete instruction classes
- **Purpose**: Each instruction implements the `IInstruction` interface with its own `execute()` method
- **Benefit**: Allows the VM to execute different instructions polymorphically without knowing their specific types

### 2. Factory Pattern
- **Location**: `InstructionFactory` class
- **Purpose**: Creates appropriate instruction objects based on opcodes from the bytecode
- **Benefit**: Centralizes object creation logic and decouples instruction instantiation from the VM execution logic

### 3. Command Pattern
- **Location**: Each instruction class encapsulates an operation
- **Purpose**: Each instruction object encapsulates all information needed to perform an action
- **Benefit**: Instructions can be stored, passed around, and executed independently

### 4. Singleton-like Context Pattern
- **Location**: `VMContext` class
- **Purpose**: Maintains the VM state (registers, stack, program counter, flags)
- **Benefit**: Provides a centralized state management for all instructions to operate on

## 🔧 Instruction Set Architecture

The VM supports the following instructions:

| Opcode | Instruction | Description | Format |
|--------|------------|-------------|---------|
| 0x01 | MOV | Move value to register | `MOV dest, src` |
| 0x02 | ADD | Add two values | `ADD dest, src` |
| 0x03 | SUB | Subtract values | `SUB dest, src` |
| 0x04 | MUL | Multiply values | `MUL dest, src` |
| 0x05 | CMP | Compare values and set flags | `CMP op1, op2` |
| 0x06 | PUSH | Push value to stack | `PUSH src` |
| 0x07 | POP | Pop value from stack | `POP dest` |
| 0x08 | JMP | Unconditional jump | `JMP address` |
| 0x09 | BE | Branch if equal (ZF=1) | `BE address` |
| 0x0A | BNE | Branch if not equal (ZF=0) | `BNE address` |
| 0x0B | PRINT | Print register value | `PRINT reg` |

### Registers

- **R0, R1, R2**: General-purpose registers
- **PC**: Program Counter
- **SP**: Stack Pointer
- **BP**: Base Pointer
- **ZF**: Zero Flag
- **CF**: Carry Flag
- **OF**: Overflow Flag

## 🚀 Usage

### Prerequisites

**For C++ VM:**
- CMake 4.0 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)

**For Python Assembler:**
- Python 3.x

### Building the VM (C++)

1. **Create build directory:**
```bash
mkdir build
cd build
```

2. **Generate build files with CMake:**
```bash
cmake ..
```

3. **Compile the project:**
```bash
cmake --build .
```

The executable `oop_cnu_term_project.exe` (Windows) or `oop_cnu_term_project` (Linux/Mac) will be generated.

### Running the VM (C++)

Execute a binary file:

```bash
./oop_cnu_term_project path/to/program.bin
```

**Example:**
```bash
./oop_cnu_term_project ../test/bin/add.bin
```

### Using the Assembler (Python)

The `encode.py` script converts assembly text files to binary format.

**Basic Usage:**

```bash
python encode.py <input_file.txt> <output_file.bin>
```

**Example:**
```bash
python encode.py test/text/add.txt test/bin/add.bin
```

**Batch Encoding Multiple Files:**

The script also supports encoding all `.txt` files in a directory:

```bash
python encode.py
```

This will encode all files in the `text/` directory to the `bin/` directory.

### Writing Assembly Code

Create a text file with assembly instructions:

```assembly
# Example: add.txt
MOV R0, 10      # Move immediate value 10 to R0
PRINT R0        # Print R0 (outputs: 10)
MOV R1, 10      # Move immediate value 10 to R1
ADD R0, R1      # Add R1 to R0
PRINT R0        # Print R0 (outputs: 20)
```

**Operand Types:**
- `REG, REG`: Both operands are registers (e.g., `ADD R0, R1`)
- `REG, IMM`: Register and immediate value (e.g., `MOV R0, 10`)
- `REG`: Single register operand (e.g., `PRINT R0`)
- `IMM`: Single immediate value (e.g., `JMP 5`)

## 🧪 Testing

Test files are located in the `test/` directory:
- `test/text/`: Assembly source files
- `test/bin/`: Compiled binaries
- `test/answer/`: Expected outputs

To run tests:
1. Encode test files: `python test/encode.py`
2. Run each test: `./oop_cnu_term_project test/bin/<test_name>.bin`
3. Compare output with files in `test/answer/`

## 📄 License

See `LICENSE` file for details.

## 👨‍💻 Development

- **Language**: C++17
- **Build System**: CMake
- **Assembler**: Python 3.x
- **IDE Compatibility**: CLion, Visual Studio, VS Code