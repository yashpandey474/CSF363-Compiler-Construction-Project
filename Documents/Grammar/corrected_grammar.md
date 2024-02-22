1. <program> ===> <otherFunctions> <mainFunction>
2. <mainFunction>===> TK_MAIN <stmts> TK_END
3. <otherFunctions> ===> <function> <otherFunctions> | EPSILON
4. <function>===>TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
5. <input_par>===>TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
6. <output_par>===>TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR | EPSILON
7. <parameter_list>===> <dataType> TK_ID <remaining_list>
8. <dataType>===> <primitiveDatatype> | <constructedDatatype>
9. <primitiveDatatype>===> TK_INT | TK_REAL
10. <constructedDatatype> ===> <A> TK_RUID | TK_RUID
11. <remaining_list>===>TK_COMMA <parameter_list> | EPSILON
12. <stmts>===><typeDefinitions> <declarations> <otherStmts> <returnStmt>
13. <typeDefinitions> ===> <typeDefinition> <typeDefinitions> | <definetypestmt> <typeDefinitions> | EPSILON
14. <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
15. <fieldDefinitions>===> <fieldDefinition> <fieldDefinition> <moreFields>
16. <fieldDefinition>===> TK_TYPE <dataType> TK_COLON TK_FIELDID TK_SEM
17. <moreFields>===> <fieldDefinition> <moreFields> | EPSILON
18. <declarations> ===> <declaration> <declarations>|EPSILON
19. <declaration>===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
20. <global_or_not>===> TK_COLON TK_GLOBAL| EPSILON
21. <otherStmts>===> <stmt> <otherStmts> | EPSILON
22. <stmt>===> <assignmentStmt> | <iterativeStmt>|<conditionalStmt>|<ioStmt>| <funCallStmt>
23. <assignmentStmt>===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
24. <singleOrRecId> ===> TK_ID <recID>
25. <recID> ===> TK_DOT TK_FIELDID <recID> | EPSILON
26. <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
27. <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP | EPSILON
28. <inputParameters>===> TK_SQL <idList> TK_SQR
29. <iterativeStmt>===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
30. <conditionalStmt>===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <new3> TK_ENDIF 
31. <new3> ===> TK_ELSE <stmt> <otherStmts> | EPSILON
32. <ioStmt>===>TK_READ TK_OP <var> TK_CL TK_SEM | TK_WRITE TK_OP <var> TK_CL TK_SEM
33. <arithmeticExpression>===> <term> <new5>
34. <new5> ===> <operator> <term> <new5> | EPSILON
35. <term> ===> <factor> <new6>
36. <new6> ===> <op_H> <factor> <new6> | EPSILON
37. <factor> ===> <var> | TK_OP <arithmeticExpression> TK_CL
38. <operator> ===> TK_PLUS | TK_MINUS
39. <op_H> ===> TK_MUL | TK_DIV
40. <booleanExpression>===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL | <var> <relationalOp> <var> | TK_NOT TK_OP <booleanExpression> TK_CL 
41. <var>===> <singleOrRecId> | TK_NUM | TK_RNUM 
42. <logicalOp>===> TK_AND | TK_OR
43. <relationalOp>===> TK_LT | TK_LE | TK_EQ |TK_GT | TK_GE | TK_NE
44. <returnStmt>===>TK_RETURN <optionalReturn> TK_SEM
45. <optionalReturn>===>TK_SQL <idList> TK_SQR | EPSILON
46. <idList>===> TK_ID <more_ids>
47. <more_ids>===> TK_COMMA <idList> | EPSILON
48. <definetypestmt>===>TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID 
49. <A>===>TK_RECORD | TK_UNION
