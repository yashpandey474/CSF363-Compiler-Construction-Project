//SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

//INCLUDE LIBRARIES
#include <stdio.h>

//PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define BUFFER_SIZE 256
//TYPICALLY 256 BYTES

// EOF CHARACTERS AT END OF BOTH BUFFERS ALWAYS

// FUNCTIONS TO READ "RELOAD" INTO BUFFER FROM A FILE 

// CODE TO READ FROM THE BUFFERS CHARACTER BY CHARACTER
struct TwinBuffer{
    int readingFirst;
    char buffer[BUFFER_SIZE*2 + 2];
};

FILE* readTestFile(char* file_path){
    FILE *file = fopen(file_path, "r");

    //UNABLE TO READ FILE
    if (file == NULL){
        printf("Error opening file %s\n", file_path);
        return NULL;
    }

    return file;

}
void readIntoBuffer(struct TwinBuffer* twinBuffer, FILE* file){
    char* buffer;
    if (twinBuffer->readingFirst)
    {
        buffer=twinBuffer->buffer;
    }
    else
    {
        buffer=twinBuffer->buffer+BUFFER_SIZE+1;
    }

    twinBuffer->readingFirst = 1-twinBuffer->readingFirst;
    size_t read_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);
}

//CREATE THE BUFFER AND RETURN IT
struct TwinBuffer initialiseTwinBuffer(){
    //INITIALISE BUFFER
    struct TwinBuffer twinBuffer;

    twinBuffer.readingFirst = 1;
    twinBuffer.buffer[BUFFER_SIZE] = '\0';
    twinBuffer.buffer[2*BUFFER_SIZE + 1] = '\0';

    return twinBuffer;
}


int main(){
    //TEST THE TWIN BUFFER
    FILE* file = readTestFile("test_program.txt");

    //INITIALISE A TWIN BUFFER
    struct TwinBuffer twinBuffer = initialiseTwinBuffer();

    //READ INTO THE BUFFER
    readIntoBuffer(&twinBuffer, file);
    readIntoBuffer(&twinBuffer, file);

    //CHECK
    for (int i = 0; i<BUFFER_SIZE*2 + 2; i += 1){
        printf("%c", twinBuffer.buffer[i]);
    }
    
}


