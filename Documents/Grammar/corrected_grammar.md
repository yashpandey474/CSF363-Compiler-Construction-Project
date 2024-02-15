1. <program> ===> <otherFunctions> <mainFunction>
2. <mainFunction>===> TK_MAIN <stmts> TK_END
3. <otherFunctions> ===> <function> <otherFunctions> | ePSILON
4. <function>===>TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
5. <input_par>===>TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
6. <output_par>===>TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR | ePSILON
7. <parameter_list>===> <dataType> TK_ID <remaining_list>
8. <dataType>===> <primitiveDatatype> | <constructedDatatype>
9. <primitiveDatatype>===> TK_INT | TK_REAL
10. <constructedDatatype> ===> <A> TK_RUID | TK_RUID
11. <remaining_list>===>TK_COMMA <parameter_list> | ePSILON
12. <stmts>===><typeDefinitions> <declarations> <otherStmts> <returnStmt>
13. <typeDefinitions> ===> <typeDefinition> <typeDefinitions> | <definetypestmt> <typeDefinitions> | ePSILON
14. <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
16. <fieldDefinitions>===> <fieldDefinition> <fieldDefinition> <moreFields>
17. <fieldDefinition>===> TK_TYPE <datatype> TK_COLON TK_FIELDID TK_SEM
18. <moreFields>===> <fieldDefinition> <moreFields> | ePSILON
19. <declarations> ===> <declaration> <declarations>|ePSILON
20. <declaration>===> TK_TYPE <dataType> TK_COLON TK_ID <optional_global> TK_SEM
21. <optional_global>===> TK_COLON TK_GLOBAL| ePSILON
22. <otherStmts>===> <stmt> <otherStmts> | ePSILON
23. <stmt>===> <assignmentStmt> | <iterativeStmt>|<conditionalStmt>|<ioStmt>| <funCallStmt>
24. <assignmentStmt>===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
25. <singleOrRecId> ===> TK_ID <recID>
<recID> ===> TK_DOT TK_FIELDID <recID> | ePSILON
26. <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
27. <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP | ePSILON
28. <inputParameters>===> TK_SQL <idList> TK_SQR
29. <iterativeStmt>===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
30. <conditionalStmt>===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <new3> TK_ENDIF 
<new3> ===> TK_ELSE <stmt> <otherStmts> | ePSILON
32. <ioStmt>===>TK_READ TK_OP <var> TK_CL TK_SEM | TK_WRITE TK_OP <var> TK_CL TK_SEM
33. <arithmeticExpression>===> <term> <new5>
<new5> ===> <operator> <term> <new5> | ePSILON
<term> ===> <factor> <new6>
<new6> ===> <op_H> <factor> <new6> | ePSILON
<factor> ===> <var> | TK_OP <arithmeticExpression> TK_CL
35. <operator> ===> TK_PLUS | TK_MINUS
<op_H> ===> TK_MUL | TK_DIV
36. <booleanExpression>===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL | <var> <relationalOp> <var> | TK_NOT TK_OP <booleanExpression> TK_CL 
39. <var>===> <singleOrRecId> | TK_NUM | TK_RNUM 
40. <logicalOp>===> TK_AND | TK_OR
41. <relationalOp>===> TK_LT | TK_LE | TK_EQ |TK_GT | TK_GE | TK_NE
42. <returnStmt>===>TK_RETURN <optionalReturn> TK_SEM
43. <optionalReturn>===>TK_SQL <idList> TK_SQR | ePSILON
44. <idList>===> TK_ID <more_ids>
45. <more_ids>===> TK_COMMA TK_ID <more_ids> | ePSILON
46. <definetypestmt>===>TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID 
47. <A>===>TK_RECORD | TK_UNION