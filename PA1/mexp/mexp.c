#include <stdio.h>
#include <stdlib.h>

//function to create an Matix of size 
int **allocMatrix(int size){
    //initiates matrix
    int **m = malloc(size * sizeof(int *));
    //assigns all rows at once
    m[0] = malloc(size * size * sizeof(int));
    //set row pointers
    for(int i = 1; i< size; i++){
        m[i] = &m[0][i * size];
    }

    return m;
}

//function to free the malloced matrix
void freeMatrix(int **m){
    free(m[0]);
    free(m);
}

//function used for matrix multiplication not sure if I need to use this function yet
int **matrixMultiplaction(int **currMatrix, int **originalMatrix, int size){
    int **resultMatrix = allocMatrix(size);
    //interates through resulting matrix
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            //creates sum of cell
            int sum = 0;
            //interates through row of currMatrix and col of originalMatrix
            for(int k = 0; k < size; k++){
                sum += (currMatrix[i][k] * originalMatrix[k][j]);
            }
            //sets resultMatrix at i,j to the values of vector sum of row i of currMatrix and col i of originalMatrix
            resultMatrix[i][j] = sum;
        }
    }
    //frees currMatrix as it is now going to be relaced with the return of this function
    freeMatrix(currMatrix);

    return resultMatrix;
}

void printMatrix(int **m, int size) {
    //nested for loop to interate through matrix
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            //prints the value at i,j
            printf("%d", m[i][j]);
            //prints a space between elements except for the last one in each row
            if (j < size - 1) {
                printf(" ");
            }
        }
        //jumps to new line after each row
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    //obtains file from cml
    FILE *file = fopen(argv[1], "r");

    /*need to fix file reading will ont read a single line at a time*/

    //reads file for matrix size
    int matrixSize;
    fscanf(file, " %d", &matrixSize);
    if(matrixSize == 0){
        return EXIT_FAILURE;
    }

    //allocates matrix of size matrixSize
    int **matrixOriginal = allocMatrix(matrixSize);
    //declares finalMatrix to be returned and for inital matrix multiplication
    int **finalMatrix = allocMatrix(matrixSize);

    //nested for loop to read matrix from file
    for(int i = 0; i<matrixSize; i++){
        for(int j = 0; j<matrixSize; j++){
            int num;
            fscanf(file, " %d", &num);
            matrixOriginal[i][j] = num;
            //used for case where matrix A^2
            finalMatrix[i][j] = num;
        }
    }

    //reads file for exponent
    int exp;
    fscanf(file, " %d", &exp);

    //for matrix multiplication up to exp
    for(int i = 1; i < exp; i++){
        //sets finalMatrix to the resulting matrix after matrix multiplication
        finalMatrix = matrixMultiplaction(finalMatrix, matrixOriginal, matrixSize);
    }

    //frees the original matrix
    freeMatrix(matrixOriginal);

    //prints out the final matrix
    printMatrix(finalMatrix, matrixSize);
    //frees the final matrix
    freeMatrix(finalMatrix);
    return EXIT_SUCCESS;
}