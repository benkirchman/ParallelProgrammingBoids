#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct YearNode {
    char* year;
    int stops;
    struct YearNode * next;
} node_t;

void addToList(node_t * head, char* year) {
    if(strncmp(&year[0], "2", 1) !=0) {
        return;
    } 
    if(head->year == NULL || head->year == 0) {
        head->year = year;
        head->stops = 1;
        return;
    } else if (strncmp(head->year, year, 4) == 0) {
        head->stops += 1;
        return;
    } 
    else if (head->next == NULL) {
        head->next = malloc(sizeof(node_t));
	head->next->year = NULL;
        head->next->next = NULL;
    }
    addToList(head->next, year);
}

void printList(node_t * head) {
    printf("%s had %d stops \n", head->year, head->stops);
    if(head->next != NULL) {
        printList(head->next);
    }
}

char* parseYearFromLine(char* line) {
    char* result = malloc(sizeof(char) * 4);
    strncpy(result, line + 6, 4);
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
    char* inBuffer = malloc(sizeof(char) * bufferSize);
    int count = 0;
    fgets(inBuffer, bufferSize, inFile);
    node_t head;
    head.year = NULL;
    head.next = NULL;
    while (fgets(inBuffer, bufferSize, inFile)) {
        char* foundYear = parseYearFromLine(inBuffer);
	addToList(&head, foundYear);
    }
    printList(&head);
    free(inBuffer);    
    fclose(inFile);
}

