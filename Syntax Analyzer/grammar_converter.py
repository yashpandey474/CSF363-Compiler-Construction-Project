
# grammar->productions = {
#     {   # list of tokens
#         {{OTHER_FUNCTIONS,0}, {MAIN_FUNCTION,1}}, # list of var structs
#     }, 
#     {
#         {},
#         {}
#     },
# }

nonterminals = {
    '<program>': 'PROGRAM',
    '<mainFunction>': 'MAIN_FUNCTION',
    '<otherFunctions>': 'OTHER_FUNCTIONS',
    '<function>': 'FUNCTION',
    '<input_par>': 'INPUT_PAR',
    '<output_par>': 'OUTPUT_PAR',
    '<parameter_list>': 'PARAMETER_LIST',
    '<dataType>': 'DATA_TYPE',
    '<primitiveDatatype>': 'PRIMITIVE_DATA_TYPE',
    '<constructedDatatype>': 'CONSTRUCTED_DATA_TYPE',
    '<remaining_list>': 'REMAINING_LIST',
    '<stmts>': 'STMTS',
    '<typeDefinitions>': 'TYPE_DEFINITIONS',
    '<typeDefinition>': 'TYPE_DEFINITION',
    '<fieldDefinitions>': 'FIELD_DEFINITIONS',
    '<fieldDefinition>': 'FIELD_DEFINITION',
    '<new1>': 'NEW1',
    '<new2>': 'NEW2',
    '<new3>': 'NEW3',
    '<moreFields>': 'MORE_FIELDS',
    '<declarations>': 'DECLARATIONS',
    '<declaration>': 'DECLARATION',
    '<global_or_not>': 'GLOBAL_OR_NOT',
    '<otherStmts>': 'OTHER_STMTS',
    '<stmt>': 'STMT',
    '<assignmentStmt>': 'ASSIGNMENT_STMT',
    '<singleOrRecId>': 'SINGLE_OR_REC_ID',
    '<recID>': 'REC_ID',
    '<funCallStmt>': 'FUN_CALL_STMT',
    '<outputParameters>': 'OUTPUT_PARAMETERS',
    '<inputParameters>': 'INPUT_PARAMETERS',
    '<iterativeStmt>': 'ITERATIVE_STMT',
    '<conditionalStmt>': 'CONDITIONAL_STMT',
    '<ioStmt>': 'IO_STMT',
    '<arithmeticExpression>': 'ARITHMETIC_EXPRESSION',
    '<new5>': 'NEW5',
    '<term>': 'TERM',
    '<new6>': 'NEW6',
    '<factor>': 'FACTOR',
    '<operator>': 'OPERATOR',
    '<op_H>': 'OP_H',
    '<booleanExpression>': 'BOOLEAN_EXPRESSION',
    '<var>': 'VAR',
    '<logicalOp>': 'LOGICAL_OP',
    '<relationalOp>': 'RELATIONAL_OP',
    '<returnStmt>': 'RETURN_STMT',
    '<optionalReturn>': 'OPTIONAL_RETURN',
    '<idList>': 'ID_LIST',
    '<more_ids>': 'MORE_IDS',
    '<definetypestmt>': 'DEFINETYPE_STMT',
    '<A>': 'A'
}


def read_data_from_file(file):
    with open(file, 'r') as f:
        data = f.readlines()
    return data

def process_rule(rule):
    rule = rule.split(".")[-1]  # remove the number at the start of the line
    nonterm, multi_prod = rule.split("===>")
    nonterm = nonterm.strip()
    nonterm = nonterminals.get(nonterm, nonterm)

    multi_prod = multi_prod.replace("\n","").split("|")
    productions = []

    for prod in multi_prod:
        prod = prod.strip()
        vars = prod.split(" ")
        production = []

        for var in vars:
            is_nonterminal = 0
            if var in nonterminals:
                var = "NT_" + nonterminals[var]
                is_nonterminal = 1
            elif var == "ePSILON":
                var = "TK_EPS"

            production.append((var, is_nonterminal))

        productions.append(production)

    return productions

def print_grammar(data):
    print("struct GrammarRule productions[NUM_NON_TERMINALS] = {", end="")

    for rule_no, rule in enumerate(data):
        productions = process_rule(rule)

        print("{" + str(len(productions)) + ",{", end="")

        for i, prod in enumerate(productions):
            print("{", end="")
            for j, (var, k) in enumerate(prod):
                print("{" + var + "," + str(k) + "}", end="")
                if j != len(prod) - 1:
                    print(",", end="")
            print("}", end="")
            if i != len(productions) - 1:
                print(",", end="")

        print("}}", end="")
        if rule_no != len(data) - 1:
            print(",", end="")

    print("};")

def max_num_productions(data):
    max_num = 0
    max_rule = ""
    
    for rule in data:
        productions = process_rule(rule)
        num_productions = len(productions)
        
        if num_productions > max_num:
            max_num = num_productions
            max_rule = rule.strip()
    
    print(f"max number of productions is: {max_rule}\n\033[93m(max productions: {max_num})\033[00m")
    return max_num

def max_vars(data):
    max_vars_count = 0
    max_rule = ""
    
    for rule in data:
        productions = process_rule(rule)
        
        for prod in productions:
            vars_count = len(prod)
            
            if vars_count > max_vars_count:
                max_vars_count = vars_count
                max_rule = rule.strip()
    
    print(f"max number of variables is: {max_rule}\n\033[93m(max variables: {max_vars_count})\033[00m")
    return max_vars_count


def print_enum(data):

    non_terminals_list = []
    
    for rule in data:
        rule = rule.split(".")[-1]  # remove the number at the start of the line
        nonterm, multi_prod = rule.split("===>")
        nonterm = nonterm.strip()
        if not nonterm in nonterminals:
            raise Exception(f"non-terminal {nonterm} not found in nonterminals dict")
        non_terminals_list.append(nonterminals[nonterm])

    print("enum NonTerminals\n{")
    for non_terminal in non_terminals_list:
        print(f"  NT_{non_terminal},")
    print("  NT_EPS")
    print("};")

    print(f"\033[93m(non terminals count (eXcluding epsilon): {len(non_terminals_list)})\033[00m")



if __name__ == "__main__":
    file = "../Documents/Grammar/corrected_grammar.md"
    data = read_data_from_file(file)

    print_grammar(data)
    print()
    max_num_productions(data)
    max_vars(data)
    print()
    print_enum(data)
    print()