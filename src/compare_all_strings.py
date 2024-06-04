def compare_text_files(file1, file2, output_file):
    with open(file1, 'r') as f1, open(file2, 'r') as f2:
        text1 = set(f1.read().split())
        text2 = set(f2.read().split())

    unique_strings = text2 - text1

    with open(output_file, 'w') as f3:
        for string in sorted(unique_strings):
            if (len(string) > 3):
                f3.write(string + '\n')

file1_path = r"C:\\Dev\\NtSourceTree\\Python\\all_strings\\all_strings_26217.txt"
file2_path = r"C:\\Dev\\NtSourceTree\\Python\\all_strings\\all_strings_26227.txt"
output_file_path = r"C:\\Dev\\NtSourceTree\\Python\\all_strings\\all_strings_diff_26217_&_26227.txt"

compare_text_files(file1_path, file2_path, output_file_path)
