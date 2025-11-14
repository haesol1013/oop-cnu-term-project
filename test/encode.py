import sys
import os # os 모듈 임포트

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
    if sys.argv.__len__() != 3:
        print("Usage: python encode.py <input_file_or_folder> <output_folder>")
        sys.exit(1)

    input_path = sys.argv[1]
    output_folder = sys.argv[2]

    # 출력 폴더가 없으면 생성 (exist_ok=True는 폴더가 이미 있어도 오류를 내지 않음)
    os.makedirs(output_folder, exist_ok=True)

    # 처리할 파일 목록을 담을 리스트
    files_to_process = []

    if os.path.isdir(input_path):
        # argv[1]이 폴더인 경우, 해당 폴더 내의 .txt 파일만 대상으로 함
        print(f"Input is a directory. Processing all .txt files in {input_path}...")
        for filename in os.listdir(input_path):
            if filename.endswith(".txt"):
                file_path = os.path.join(input_path, filename)
                files_to_process.append(file_path)
    elif os.path.isfile(input_path):
        # argv[1]이 파일인 경우, 해당 파일만 대상으로 함
        print(f"Input is a single file: {input_path}")
        files_to_process.append(input_path)
    else:
        print(f"Error: Input path {input_path} is not a valid file or directory.")
        sys.exit(1)

    if not files_to_process:
        print("No input files to process.")
        return

    # 대상 파일들을 순회하며 인코딩
    for input_file_path in files_to_process:
        # 출력 파일 이름 생성 (예: /path/to/test.txt -> test.bin)
        base_filename = os.path.basename(input_file_path) # test.txt
        filename_no_ext = os.path.splitext(base_filename)[0] # test
        output_bin_name = filename_no_ext + ".bin" # test.bin

        # 출력 파일의 전체 경로 (예: /output_folder/test.bin)
        output_bin_path = os.path.join(output_folder, output_bin_name)

        try:
            print(f"Encoding {input_file_path} -> {output_bin_path}")
            encode_file(input_file_path, output_bin_path)
        except Exception as e:
            print(f"--- FAILED to encode {input_file_path}: {e} ---")

    print(f"\nEncoding complete. Output files are in {output_folder}")

if __name__ == "__main__":
    main()