def get_data(file_name):
    with open(file_name, 'r') as file:
        return file.read().splitlines()

if __name__ == '__main__':

    fs = sorted(get_data('finalized_sets.txt'))
    cs = sorted(get_data('computed_sets.txt'))
    
    fs = [x.strip().replace("EPS","TK_EPS").replace(' ','') for x in fs if x.strip()]
    cs = [x.strip().replace(' ','') for x in cs if x.strip()]

    print(fs, cs)

    if len(fs) != len(cs):
        print("Length mismatch")
        print("Length of fs:", len(fs))
        print("Length of cs:", len(cs))
        exit(1)

    for i in range(len(fs)):
        if sorted(fs[i]) != sorted(cs[i]):
            print("Mismatch at line", i+1)
            print("fs:", fs[i])
            print("cs:", cs[i])
            exit(1)
    
