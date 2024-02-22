from grammar_converter import nonterminals

file_path = 'finalized_sets_doc.txt'  # Replace 'example.txt' with the path to your file

def get_data(filename):
    try:
        with open(filename, 'r') as file:
            file_contents = [s.strip()[:-1] for s in file.readlines()]
            if '' in file_contents:
                file_contents.remove('')
            
            return file_contents
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
        return []
    
    
# NT_PROGRAM(First): TK_MAIN, TK_FUNID, 
# NT_PROGRAM(Follow): TK_EOF, 

def process_data(data):
    for i in range(0, len(data), 3):
        nt, first, follow = (data[i], data[i+1], data[i+2])
        # print(nt, first, follow)
        if (f"<{nt}>") not in nonterminals:
            print(f"Non-terminal '{nt}' not found in nonterminals dictionary.")
            continue
        if first[-1]==",":
            first = first[:-1]
        if follow[-1]==",":
            follow = follow[:-1]
        print('NT_'+nonterminals[f"<{nt}>"]+'(First): '+ first+",")
        print('NT_'+nonterminals[f"<{nt}>"]+'(Follow): '+ follow+",")
        print()



if __name__=='__main__':

    data = get_data(file_path)
    # print(data,"\n")
    if len(data) % 3 != 0:
        print("Invalid format. Please check the file and try again.")
        print("previously, first set of stmts was split in two lines, make that one line")
        exit(1)

    process_data(data)

