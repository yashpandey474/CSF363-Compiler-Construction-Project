#include <stdio.h>
#include "lexerDef.h"


FILE *getStream(FILE *fp);

tokenInfo getNextToken(twinBuffer B);

void remove_comments(char *testcasefile, char *cleanFile);