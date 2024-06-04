input_file_path = "all_debug_symbols_25921.txt"
output_file_path = "prefixes_25926.txt"

def process_line(line):
    # Probably any function is longer than 5 symbols.
    if len(line) < 5:
        return None

    # First 3 symbols are letters and the first letter is capital.
    if line[:3].isalpha() and line[0].isupper():
        uppercase_count = 0
        for i, char in enumerate(line):
            if char.isupper():
                uppercase_count += 1
                if uppercase_count == 2:
                    return line[:i]
    else:
        return None

def main():    
    unique_lines = set()

    with open(input_file_path, 'r') as input_file:
        for line in input_file:
            processed_line = process_line(line.strip())
            if processed_line is not None:
                unique_lines.add(processed_line)

    unique_lines = sorted(unique_lines)

    with open(output_file_path, 'w') as output_file:
        for unique_line in unique_lines:
            output_file.write(unique_line + '\n')


if __name__ == "__main__":
    main()
