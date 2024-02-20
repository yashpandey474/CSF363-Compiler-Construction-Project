def pretty_print_from_file(filename):
    with open(filename, 'r') as file:
        for line in file:
            # Splitting each line based on tab character
            parts = line.strip().split('\t')
            if len(parts) < 3 or 'Error' in line:
                print(line.strip())
                continue

            # Extracting line number, lexeme, and token from the split parts
            line_no = parts[0].split()[-1]  # Assuming "Line no. X" format
            lexeme = parts[1].split()[-1]
            token = (parts[2] if len(parts) ==
                     3 else parts[3]).strip().split()[-1]

            try:
                # Convert line number to integer for formatting
                line_no = int(line_no)
            except ValueError:
                continue  # Skip lines with invalid line number format

            # Pretty printing using printf-style format
            print("Line no. %-5d Lexeme %-30s Token %-s" %
                  (line_no, lexeme, token))


# Example usage
# Replace this with your actual file name
filename = "lexemesandtokens_t1.txt"
pretty_print_from_file(filename)
