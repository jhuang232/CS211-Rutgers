#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER} Kind;
const char *kindStrings[] = {"AND", "OR", "NAND", "NOR", "XOR", "NOT", "PASS", "DECODER", "MULTIPLEXER"};

typedef struct Directive{
    Kind kind;
    int size;
    int *params;
    int outputStart;
    int paramsLength;
}Directive;

typedef struct Circuit{
    int numDir;
    Directive **directives;
    int currIndex;
}Circuit;

typedef struct VarTable{
    char **allParams;
    int maxParams;
    int currIndex;
}VarTable;

// Returns the size of the param array for a directive given size and kind
int paramArraySize(Kind k, int size){
    switch(k){
        case MULTIPLEXER:
            return (1 << size) + size + 1;
        case DECODER:
            return (1 << size) + size;
        case NOT: case PASS:
            return 2;
        default:
            return 3;
    }
}

// Calculates the starting index of the outputs
int outputStartIndex(Kind k, int size){
    switch(k){
        case MULTIPLEXER:
            return (1 << size) + size;
        case DECODER:
            return size;
        case NOT: case PASS:
            return 1;
        default:
            return 2;
    }
}


Directive *makeDirective(Kind k, int s, int *params) {
    Directive *dir = malloc(sizeof(Directive));
    dir->kind = k;
    dir->size = s;
    dir->outputStart = outputStartIndex(k, s);
    dir->paramsLength = paramArraySize(k, s);
    // Copy the params array
    dir->params = malloc(sizeof(int) * dir->paramsLength);
    if (dir->params == NULL) {
        free(dir);
    }
    memcpy(dir->params, params, dir->paramsLength);
    return dir;
}

Circuit *makeCircuit(){
    Circuit *c = malloc(sizeof(Circuit));
    c->numDir = 0;
    c->directives = NULL;
    c->currIndex = 0;
    return c;
}

VarTable *makeVarTable(){
    VarTable *table = malloc(sizeof(VarTable));
    table->allParams = malloc(sizeof(char*) * 4);
    table->allParams[0] = "0";
    table->allParams[1] = "1";
    table->currIndex = 2;
    table->maxParams = 4;
    return table;
}


// Converts a string to kind else returns -1 for error
Kind stringToKind(char *kind){
    for (int i = 0; i < 9; i++) {
        if (strcmp(kind, kindStrings[i]) == 0) {
            return (Kind)i;
        }
    }
    return -1;
}

// Searches through the VarTable for the matching var and returns it's index in the array
int indexVar(VarTable *t, char *var){
    for(int i = 0; i < t->currIndex;i++){
        if(strcmp(t->allParams[i], var) == 0){
            return i;
        }
    }
    return -1;
}

// Adds the variable to the array if the variable it not already added and then increases the counter in VarTable
// returns 0 is everything is sucessful and 1 for error
int addVar(VarTable *t, char *var) {
    // Check if the variable already exists
    if (indexVar(t, var) != -1) {
        return 1;
    }

    // If the array is filled double the array size
    if (t->currIndex >= t->maxParams - 1) {
        char **temp = realloc(t->allParams, 2 * sizeof(char *) * t->maxParams);
        if (temp == NULL) {
            return 1;
        }
        t->allParams = temp;
        t->maxParams *= 2;
    }

    // Normal additon of the var at the currIndex
    t->allParams[t->currIndex] = malloc(strlen(var) + 1);
    if (t->allParams[t->currIndex] == NULL) {
        return 1;
    }
    strcpy(t->allParams[t->currIndex], var);

    // Increments the index of the data struct
    t->currIndex += 1;
    return 0;
}

// Adds the directive to the array and increases the counter
void addDirective(Circuit *c, Directive *dir) {
    c->directives = realloc(c->directives, (c->numDir + 1) * sizeof(Directive*));
    c->directives[c->numDir] = dir;
    c->numDir++;
}

// Build DAG using the directives as nodes
Directive **topoSort(Circuit *c, VarTable *t, int inputIndex){
    Directive **sortedDirectives = malloc(sizeof(Directive*) * c->numDir);
    int sortedIndex = 0;
    int *knownParams = malloc(t->currIndex * sizeof(int));    
    memset(knownParams, 0, t->currIndex * sizeof(int));    
    // For the initial input variables set their value to 1 if they are already known
    for(int i = 0; i < inputIndex + 2; i++){
        knownParams[i] = 1;
    }
    // Begin sort
    while (sortedIndex < c->numDir) {
        int found = 0;
        // Iterate over directives
        for (int i = 0; i < c->numDir; i++) {
            Directive *dir = c->directives[i];
            if (dir == NULL) {
                continue;
            }
            // Check if all inputs are known
            int allInputsKnown = 1;
            // For each input of the directive check if it's index value is 1
            for(int j = 0; j < dir->outputStart; j++){
                if(!knownParams[dir->params[j]]){
                    allInputsKnown = 0;
                    break;
                }
            }

            // If all inputs are known, process the directive
            if (allInputsKnown){
                // Add the directive to the sorted list
                sortedDirectives[sortedIndex++] = dir;
                // Removes form the Circuit
                c->directives[i] = NULL;
                // Update knownParams based on the outputs of the directive
                for(int j = dir->outputStart; j < dir->paramsLength; j++){
                    knownParams[dir->params[j]] = 1;
                }
                found = 1;
            }
        }
        // If no directive is found with all inputs known, break the loop
        if(!found){
            break;
        }
    }
    free(knownParams);
    return sortedDirectives;
}

// Print from index 2 to inputIndex, and inputIndex to outputIndex
void printTruthLine(int count, Directive **sorted, VarTable *t, int inputCount, int outputCount, int dirCount){
    int* calculatedParams = malloc(sizeof(int) * t->currIndex);
    memset(calculatedParams, 0, t->currIndex * sizeof(int));    
    calculatedParams[1] = 1; // This if for the literal "1" input
    // For the initial input variables set their value interpreting the binary form of count
    for (int i = inputCount + 1; i >= 2; i--) {
        calculatedParams[i] = count % 2;
        count /= 2;
    }    

    for (int i = 0; i < dirCount; i++) {
        Directive *dir = sorted[i];

        // Evaluate the gate based on the directive type
        switch (dir->kind) {
            case AND:
                calculatedParams[dir->params[dir->outputStart]] = (calculatedParams[dir->params[0]] & calculatedParams[dir->params[1]]) & 1;
                break;
            case OR:
                calculatedParams[dir->params[dir->outputStart]] = (calculatedParams[dir->params[0]] | calculatedParams[dir->params[1]]) & 1;
                break;
            case NAND:
                calculatedParams[dir->params[dir->outputStart]] = ~((calculatedParams[dir->params[0]] & calculatedParams[dir->params[1]]) & 1) & 1;
                break;
            case NOR:
                calculatedParams[dir->params[dir->outputStart]] = ~((calculatedParams[dir->params[0]] | calculatedParams[dir->params[1]]) & 1) & 1;
                break;
            case XOR:
                calculatedParams[dir->params[dir->outputStart]] = (calculatedParams[dir->params[0]] ^ calculatedParams[dir->params[1]]) & 1;
                break;
            case NOT:
                calculatedParams[dir->params[dir->outputStart]] = ~calculatedParams[dir->params[0]];
                break;
            case PASS:
                calculatedParams[dir->params[dir->outputStart]] = calculatedParams[dir->params[0]];
                break;
            case DECODER:
                // Obtain the index offset of the decoder output that will be 1
                int decoderIndex = 0;
                for (int j = 0; j < dir->size; j++) {
                    decoderIndex = (decoderIndex << 1) | calculatedParams[dir->params[j]];
                }
                calculatedParams[dir->params[dir->outputStart + decoderIndex]] = 1;
                break;
            case MULTIPLEXER:
                // Obtain the value of the selector
                int selectorValue = 0;
                // For every selector which is index [2^size to 2^size - 1 + size] append to the binary value of selector
                for (int j = (1 << dir->size); j <= (1 << dir->size) - 1 + dir->size; j++) {
                    selectorValue = (selectorValue << 1) | calculatedParams[dir->params[j]];
                }
                calculatedParams[dir->params[dir->outputStart]] = calculatedParams[dir->params[selectorValue]];
                break;
        }
    }

    // Begin print output
    for (int i = 2; i < inputCount + 2; i++) {
        printf("%d ", calculatedParams[i]);
    }
    printf("|");
    for (int i = inputCount + 2; i < inputCount + 2 + outputCount; i++) {
        printf(" %d", calculatedParams[i]);
    }
    printf("\n");

    free(calculatedParams);
}

void freeDirective(Directive *dir){
    if(dir == NULL) return;
    free(dir->params);
    free(dir);
}

void freeCircuit(Circuit *c){
    for(int i = 0; i < c->numDir; i++){
        freeDirective(c->directives[i]);
    }
    free(c->directives);
    c->directives = NULL;
    free(c);
}

void freeVarTable(VarTable *table){
    for (int i = 2; i < table->currIndex; i++) {
        free(table->allParams[i]);
    }
    free(table->allParams);
    free(table);
}

void freeSortedDirectives(Directive **sortedDirectives, int numDirs) {
    for(int i = 0; i < numDirs; i++){
        freeDirective(sortedDirectives[i]);
    }
    free(sortedDirectives);
}


int main(int argc, char *argv[]){
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        return EXIT_FAILURE;
    }

    // File data storage
    VarTable *table = makeVarTable();
    Circuit *circuit = makeCircuit();
    char directiveType[17];
    int size;
    int *params;
    int inputCount = 0;
    int outputCount = 0;

    // Reading INPUT line
    if (fscanf(file, "%16s", directiveType) == 1 && strcmp(directiveType, "INPUT") == 0){
        if (fscanf(file, "%16d", &size) == 1){
            for (int i = 0; i < size; i++){
                char currParam[17];
                if (fscanf(file, "%16s", currParam) == 1){
                    addVar(table, currParam);
                    inputCount++;
                }
            }
        }
    } else{
        return EXIT_FAILURE;
    }

    // Reading OUTPUT line
    if(fscanf(file, "%16s", directiveType) == 1 && strcmp(directiveType, "OUTPUT") == 0){
        if(fscanf(file, "%16d", &size) == 1){
            for(int i = 0; i < size; i++){
                char currParam[17];
                if(fscanf(file, "%16s", currParam) == 1){
                    addVar(table, currParam);
                    outputCount++;
                }
            }
        }
    }else{
        return EXIT_FAILURE;
    }

    // Rest of file handling and processing of directives
    while(fscanf(file, "%16s", directiveType) != EOF){
        // Processing of directives
        Kind currKind = stringToKind(directiveType);
        // Calculates the number of reads for a directive's params
        if(currKind >= 7){
            if(fscanf(file, "%16d", &size) != 1) return EXIT_FAILURE;
        }
        int numReads = paramArraySize(currKind, size);

        // Set up for the directives params and conversion from vairble to integer
        params = malloc(sizeof(int) * numReads);
        for (int i = 0; i < numReads; i++){
            char currentParam[17];
            if(fscanf(file, "%16s", currentParam) == 1){
                addVar(table, currentParam);
                params[i] = indexVar(table, currentParam);
            }
        }
        Directive *dir = makeDirective(currKind, size, params);
        memcpy(dir->params, params, sizeof(int) * numReads);
        addDirective(circuit, dir);
        free(params);
    }

    Directive **finalCircuit = topoSort(circuit, table, inputCount);
    for(int i = 0; i < (1 << (inputCount)); i++){
        printTruthLine(i, finalCircuit, table, inputCount, outputCount, circuit->numDir);
    }

    freeVarTable(table);
    freeSortedDirectives(finalCircuit, circuit->numDir);
    freeCircuit(circuit);
    fclose(file);
    return EXIT_SUCCESS;
}
