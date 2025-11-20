import sys
import os

instructions = {
    "MOV"   : 0b000001,
    "ADD"   : 0b000010,
    "SUB"   : 0b000011,
    "MUL"   : 0b000100,
    "CMP"   : 0b000101,
    "PUSH"  : 0b000110,
    "POP"   : 0b000111,
    "JMP"   : 0b001000,
    "BE"    : 0b001001,
    "BNE"   : 0b001010,
    "PRINT" : 0b001011
}

registers = {
    "R0" : 0b00000001,
    "R1" : 0b00000010,
    "R2" : 0b00000011,
    "PC" : 0b00000100,
    "SP" : 0b00000101,
    "BP" : 0b00000110,
    "ZF" : 0b00000111,
    "CF" : 0b00001000,
    "OF" : 0b00001001,
}

flags = {
    "bothReg" : 0b00,
    "bothIMM" : 0b01,
    "oneReg"  : 0b10,
    "oneIMM"  : 0b11
}

def decode_instruction(instruction):
    # 공백 라인이나 주석(#) 라인 무시
    line = instruction.strip()
    if not line or line.startswith("#"):
        return None

    temp = line.replace(",", "").split(" ")
    # split 과정에서 생길 수 있는 공백 엔트리 제거
    temp = [item for item in temp if item]

    src = None
    dest = None
    flag = None

    if len(temp) == 2: # 단일 operand만을 받는 명령어
        opcode = temp[0]
        dest = temp[1]
        if dest in registers:
            flag = flags["oneReg"]
        elif dest not in registers:
            flag = flags["oneIMM"]
        else:
            raise ValueError(f"Invalid operand: {dest}")
    elif len(temp) == 3: # 두 개의 operand를 받는 명령어
        opcode = temp[0]
        dest = temp[1]
        src = temp[2]
        if src in registers and dest in registers:
            flag = flags["bothReg"]
        elif src not in registers and dest in registers:
            flag = flags["bothIMM"]
        else:
            raise ValueError(f"Invalid operands: {dest}, {src}")
    else:
        raise ValueError(f"Invalid instruction format: {line}")

    return opcode, src, dest, flag

def encode_file(input_file_path, output_bin_path):
    """단일 텍스트 파일을 .bin 파일로 인코딩합니다."""

    out = open(output_bin_path, "wb")
    with open(input_file_path, "r") as f:
        line_number = 0
        while True:
            line_number += 1
            line = f.readline()
            if not line:  # EOF
                break

            # 공백 또는 주석 라인 처리
            decoded_data = None
            try:
                decoded_data = decode_instruction(line)
                if not decoded_data:
                    continue # 공백 라인이면 다음 줄로
            except Exception as e:
                print(f"Error parsing {input_file_path} at line {line_number}: {e}")
                out.close() # 오류 발생 시 파일 닫기
                return

            opcode, src, dest, flag = decoded_data

            out.write(bytes([instructions[opcode] << 2 | flag])) # 6 + 2bit
            out.write(bytes([0b00000000])) # 8bit, reserved
            if flag == flags["bothReg"]:
                out.write(bytes([registers[src]])) # 8bit, src
                out.write(bytes([registers[dest]])) # 8bit, dest
            elif flag == flags["bothIMM"]:
                out.write(bytes([int(src)])) # 8bit, src
                out.write(bytes([registers[dest]])) # 8bit, dest
            elif flag == flags["oneReg"]:
                out.write(bytes([0b00000000])) # 8bit, src (unused)
                out.write(bytes([registers[dest]])) # 8bit, dest
            elif flag == flags["oneIMM"]:
                out.write(bytes([0b00000000])) # 8bit, src (unused)
                out.write(bytes([int(dest)])) # 8bit, dest
    out.close()

def main():
    # 인자가 없으면 기본 경로 사용 (text/ -> bin/)
    if len(sys.argv) == 1:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        input_folder = os.path.join(script_dir, "text")
        output_folder = os.path.join(script_dir, "bin")

        print("No arguments provided. Using default paths:")
        print(f"  Input folder: {input_folder}")
        print(f"  Output folder: {output_folder}\n")

        # 출력 폴더가 없으면 생성
        os.makedirs(output_folder, exist_ok=True)

        # text 폴더의 모든 .txt 파일 찾기
        if not os.path.isdir(input_folder):
            print(f"Error: Input folder {input_folder} does not exist.")
            sys.exit(1)

        txt_files = [f for f in os.listdir(input_folder) if f.endswith(".txt")]

        if not txt_files:
            print("No .txt files found in text/ folder.")
            return

        # 각 파일 변환
        for filename in txt_files:
            input_file_path = os.path.join(input_folder, filename)
            output_file_path = os.path.join(output_folder, filename.replace(".txt", ".bin"))

            try:
                print(f"Encoding {filename} -> {os.path.basename(output_file_path)}")
                encode_file(input_file_path, output_file_path)
            except Exception as e:
                print(f"  FAILED: {e}")

        print(f"\nEncoding complete. Output files are in {output_folder}")

    elif len(sys.argv) == 3:
        # 단일 파일 변환 모드
        input_file = sys.argv[1]
        output_file = sys.argv[2]

        if not os.path.isfile(input_file):
            print(f"Error: Input file {input_file} does not exist.")
            sys.exit(1)

        # 출력 파일의 디렉토리가 없으면 생성
        output_dir = os.path.dirname(output_file)
        if output_dir and not os.path.exists(output_dir):
            os.makedirs(output_dir, exist_ok=True)

        try:
            print(f"Encoding {input_file} -> {output_file}")
            encode_file(input_file, output_file)
            print("Encoding complete.")
        except Exception as e:
            print(f"FAILED: {e}")
            sys.exit(1)
    else:
        print("Usage:")
        print("  python encode.py                                    # Encode all .txt files in text/ to bin/")
        print("  python encode.py <input_file.txt> <output_file.bin> # Encode single file")
        sys.exit(1)

if __name__ == "__main__":
    main()
