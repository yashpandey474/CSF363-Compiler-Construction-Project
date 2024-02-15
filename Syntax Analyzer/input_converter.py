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
    nonterm = nonterminals.get(nonterm, nonterm)

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

for rule in 