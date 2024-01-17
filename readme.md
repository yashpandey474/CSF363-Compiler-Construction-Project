1. Lexical Analyzer:
L1: Remove white spaces and comments.
L2: Group the lexemes into tokens
L3: Put identifier entries in symbol table (scope, lineNo, type, lexeme etc.)
L4: Errors: illegal characters, identifier length exceeded
L5: Recovery: delete, insert or replace characters


2. Syntax Analyzer


3. Symbol Table
ST1: Initially store keywords
ST2: Store info of identifiers in source program