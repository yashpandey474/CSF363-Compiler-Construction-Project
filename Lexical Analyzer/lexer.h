#include <stdio.h>
#include "lexerDef.h"

FILE *readTestFile(char *file_path);
twinBufferArray initialiseTwinBuffer(FILE *file);
twinBuffer *initialiseLA(twinBufferArray bufferArray);
int getStream(twinBufferArray bufferArray);
tokenInfo getNextToken(twinBuffer LA);
void changeForward(twinBuffer *LA, int flag);