KEYWORDS:
as
call
definetype
end, endunion, endwhile, endif, endrecord, else
global
input, int, if
list
output
parameters, parameter
real, read, return, record
type, then
union
with, while write
<---, <, <=
>, >=
==, !=, ~, @@@, &&&, /, *, -, +, ., :, ;, ,,(, ), [, ]
_main, 

OTHER:
TK_FUNID: _[a-z|A-Z][a-z|A-Z]*[0-9]*
TK_ID: [b-d] [2-7][b-d] *[2-7] *
TK_RUID: #[a-z][a-z]*
TK_FIELDID: [a-z][a-z]*
TK_NUM: [0-9][0-9]*
TK_RNUM: [0-9][0-9]*.[0-9][0-9] | [0-9][0-9]*.[0-9][0-9]E(+|-|eps)[0-9][0-9]
DELIMITER: \n, \t, " "
