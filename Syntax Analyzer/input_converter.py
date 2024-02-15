with open('../Documents/Grammar/input.txt', 'r') as file:
    data = file.read()
    data = data.split('\n')
with open('../Documents/Grammar/input.txt', 'w') as file:
    file.write('')
with open('../Documents/Grammar/input.txt', 'a') as file:
    dict1={}
    for i in data:
        # print(i)
        key, value =i.split(' ===> ')
        dict1[key]=value
    for i in dict1.keys():
        dict1[i]=dict1[i].split('|')
        for j in dict1[i]:
            j.strip()
    for i in dict1.keys():
        for j in dict1[i]:
            file.write(i+' ===> '+j+'\n')


with open('../Documents/Grammar/input.txt', 'r') as file:
    data=file.read()
    

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
    '<optional_global>': 'OPTIONAL_GLOBAL',
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

def process_rule(rule):
    nonterm, multi_prod = rule.split("===>")
    nonterm = nonterm.strip()
    nonterm = nonterminals[nonterm]
    multi_prod=[multi_prod]
    

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
    return nonterm, [production]

def print_grammar(data):
    print("struct GrammarRule productions[NUM_NON_TERMINALS] = ", end="")

    for rule_no, rule in enumerate(data):
        nonterminals, productions = process_rule(rule)

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

    print(";")

# for rule in data.split("\n"):
#     if rule:
#         print(process_rule(rule))

print_grammar(data.split("\n"))