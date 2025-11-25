# OOP CNU Term Project - Virtual Machine Simulator

## 📋 Project Overview

This project is a **Virtual Machine (VM) Simulator** implemented in C++ designed to execute custom bytecode instructions. The project incorporates an assembler developed in Python to convert human-readable assembly code into a binary format executable by the VM.

The VM supports basic operations including arithmetic operations, register management, stack operations, conditional branching, and output functionality.

For detailed technical documentation, including system architecture, design patterns, and instruction set details, please refer to [ARCHITECTURE.md](docs/ARCHITECTURE.md).

## 📦 Prerequisites

**For C++ VM:**

- CMake 4.0 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)

**For Python Tools:**

- Python 3.x

## 🚀 Usage

### Step 1: Build the VM

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


The executable `oop_cnu_term_project.exe` (Windows) or `oop_cnu_term_project` (Linux/Mac) will be generated in the `build` directory.

### Step 2: Write Assembly Code

Create a source file containing assembly instructions within the `test/text/` directory:

```assembly
# Example: add.txt
MOV R0, 10      # Move immediate value 10 to R0
PRINT R0        # Print R0 (outputs: 10)
MOV R1, 10      # Move immediate value 10 to R1
ADD R0, R1      # Add R1 to R0
PRINT R0        # Print R0 (outputs: 20)
```

### Step 3: Assemble to Binary

Navigate to the `test/` directory to execute the assembler:

**Encode all source files:**

```bash
cd test
python encode.py
```

This command converts all `.txt` files in the `text/` directory to `.bin` files in the `bin/` directory.

**Encode a single file:**

```bash
python encode.py text/add.txt bin/add.bin
```

### Step 4: Run the Program

Execute the binary file with the VM:

```bash
cd build
./oop_cnu_term_project ../test/bin/add.bin
```

**Example output:**

```
10
20
```


## 🧪 Testing

Test files are located in the `test/` directory:

- `test/text/`: Assembly source files
- `test/bin/`: Compiled binaries
- `test/answer/`: Expected outputs

**To run tests:**

1. **Encode test files:**

   ```bash
   cd test
   python encode.py
   ```

2. **Run automated tests:**

   The provided Python script facilitates automated testing and result verification.

   ```bash
   python run_tests.py
   ```

   The script performs the following actions:

   - Automatically locates the VM executable
   - Executes all `.bin` files in `test/bin/`
   - Compares the output with expected results in `test/answer/`
   - Reports the pass/fail status for each test

**Manual Testing:**

Manual testing may be performed using the following commands:

```bash
cd ../build
./oop_cnu_term_project ../test/bin/add.bin
```


## 📄 License

See `LICENSE` file for details.

## 👨‍💻 Development

- **Language**: C++17
- **Build System**: CMake
- **Assembler**: Python 3.x
- **IDE Compatibility**: CLion, Visual Studio, VS Code
