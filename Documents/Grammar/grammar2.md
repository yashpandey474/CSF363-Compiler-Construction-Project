1. <program> ===> <otherFunctions> <mainFunction>
2. <mainFunction>===> TK_MAIN <stmts> TK_END
3. <otherFunctions>===> <function> <otherFunctions> | eps
4. <function>===>TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
5. <input_par>===>TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
6. <output_par>===>TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR | eps
7. <parameter_list>===> <dataType> TK_ID <remaining_list>
8. <dataType>===> <primitiveDatatype> |<constructedDatatype>
9. <primitiveDatatype>===> TK_INT | TK_REAL
10. <constructedDatatype>===> TK_RECORD TK_RUID | TK_UNION TK_RUID | TK_RUID
11. <remaining_list>===>TK_COMMA <parameter_list> | eps
12. <stmts>===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
13. <typeDefinitions>===> <actualOrRedefined> <typeDefinitions> | eps
14. <actualOrRedefined> ===> <typeDefinition> | <definetypestmt>
15. <typeDefinition>===>TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD | TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
17. <fieldDefinitions>===> <fieldDefinition> <fieldDefinition> <moreFields>
18. <fieldDefinition>===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
19. <fieldtype>===> <primitiveDatatype> |  <constructedDatatype>
20. <moreFields>===> <fieldDefinition> <moreFields> | eps
21. <declarations> ===> <declaration> <declarations>| eps
22. <declaration>===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
23. <global_or_not>===>TK_COLON TK_GLOBAL| eps
24. <otherStmts>===> <stmt> <otherStmts> | eps
25. <stmt>===> <assignmentStmt> | <iterativeStmt>|<conditionalStmt>|<ioStmt>| <funCallStmt>
26. <assignmentStmt>===> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
27. <singleOrRecId>===>TK_ID <option_single_constructed> 
28. <option_single_constructed>===> eps | <oneExpansion> <moreExpansions>
29. <oneExpansion>===> TK_DOT TK_FIELDID
30. <moreExpansions>===> <oneExpansion> <moreExpansions> | eps
31. <funCallStmt>===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
32. <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP | eps
33. <inputParameters>===> TK_SQL <idList> TK_SQR
34. <iterativeStmt>===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
35. <conditionalStmt>===> TK_IF <booleanExpression> TK_THEN <stmt> <otherStmts> TK_ELSE <otherStmts> TK_ENDIF | TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
37. <elsePart>===>TK_ELSE <stmt> <otherStmts> TK_ENDIF | TK_ENDIF
38. <ioStmt>===>TK_READ TK_OP <var> TK_CL TK_SEM | TK_WRITE TK_OP <var> TK_CL TK_SEM
39. <arithmeticExpression> ===> <term> <expPrime>
40. <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime> | eps
41. <term>===> <factor> <termPrime>
42. <termPrime> ===> <highPrecedenceOperators> <factor> <termPrime> | eps
43. <factor> ===> TK_OP <arithmeticExpression> TK_CL | <var>
44. <highPrecedenceOperators>===> TK_MUL | TK_DIV
45. <lowPrecedenceOperators> ===> TK_PLUS | TK_MINUS
46. <booleanExpression>===>TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL | <var> <relationalOp> <var> | TK_NOT TK_OP <booleanExpression> TK_CL
49. <var>===> <singleOrRecId> | TK_NUM | TK_RNUM
50. <logicalOp>===>TK_AND | TK_OR
51. <relationalOp>===> TK_LT | TK_LE | TK_EQ |TK_GT | TK_GE | TK_NE
52. <returnStmt>===>TK_RETURN <optionalReturn> TK_SEM
53. <optionalReturn>===>TK_SQL <idList> TK_SQR | eps
54. <idList>===> TK_ID <more_ids>
55. <more_ids>===> TK_COMMA <idList> | eps
56. <definetypestmt>===>TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
57. <A> ===>TK_RECORD | TK_UNION