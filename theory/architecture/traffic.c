#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* parseYearFromLine(char* line) {
    char* result = malloc(sizeof(char) * 4);
    strncpy(result, line + 6, 4);
    printf(result);
    return result;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Include file name please");
	return 0;
    }
    FILE* inFile; 
    inFile = fopen(argv[1], "r");
    int bufferSize = 10000;
    char* year_2014 = "2014";
    char* inBuffer = malloc(sizeof(char) * bufferSize);
    int count = 0;
    fgets(inBuffer, bufferSize, inFile);
    while (fgets(inBuffer, bufferSize, inFile)) {
	char* foundYear = parseYearFromLine(inBuffer);
	if(strncmp(foundYear, year_2014, 4) == 0) {
            count++;
        }
    }
    printf("There were %d incidents in 2014", count);
    free(inBuffer);    
    fclose(inFile);
}
