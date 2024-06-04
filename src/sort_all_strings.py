input_file = "all_strings_25977.txt"
output_file = "all_strings_25977_sorted.txt"

with open(input_file, 'r') as file:
    all_strings = file.readlines()

all_strings.sort(key=len, reverse=True)

with open(output_file, 'w') as file:
    file.writelines(all_strings)
