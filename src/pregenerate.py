def replace_backslashes(file_path):
    # Read the content of the file
    with open(file_path, 'r') as file:
        content = file.read()

    # Replace backslashes with double backslashes
    replaced_content = content.replace('\\', '\\\\')

    # Write the modified content back to the file
    with open(file_path, 'w') as file:
        file.write(replaced_content)

# Example usage
file_path = 'paths26040.txt'  # Replace with the path to your file
replace_backslashes(file_path)
