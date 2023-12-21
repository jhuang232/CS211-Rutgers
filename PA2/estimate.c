#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//need to create struct for a mtrix to hold its row and col values
typedef struct{
    int numRows;
    int numCols;
    double **matrix;
}Matrix;

//function to create an Matrix of size 
double **allocMatrix(int row, int col){
    //initiates matrix
    double **m = malloc(row * sizeof(double *));
    //assigns all rows at once
    m[0] = malloc(row * col * sizeof(double));
    //set row pointers
    for(int i = 1; i< row; i++){
        m[i] = &m[0][i * col];
    }
    return m;
}

//creates a matrix with row and cols
Matrix createMatrix(int row, int col){
    Matrix m;
    m.numRows = row;
    m.numCols = col;
    if(row <= 0 || col <= 0){
        m.matrix = NULL;
    }else{
        m.matrix = allocMatrix(row, col);
    }
    return m;
}


//function to free the malloced matrix
void freeMatrix(Matrix m){
    if(m.matrix != NULL){
        free(m.matrix[0]);
        free(m.matrix);
    }
}

//creates a customixes power function
double customPow(double base, int exponent) {
    if (exponent == 0) {
        return 1.0;
    } else if (exponent % 2 == 0) {
        return 1.0;
    } else {
        return -1.0;
    }
}

//gets the minor of a matrix given row and col for computer indexes where  human 1 equal computer 0
Matrix matrixMinor(Matrix m, int row, int col){
    //gets the number of rows and cols in matrix
    int numRows = m.numRows - 1;
    int numCols = m.numCols - 1;
    //allocates a matrixMinor of numRows -1 and numCols - 1
    Matrix matrixMinor = createMatrix(numRows, numCols);
    //secondary counters for minor matrix
    int mRow = 0;
    int mCol = 0;
    if (numCols > 0 && numRows > 0) {
        //assigns values to the matrixMinor removing row and col
        for (int i = 0; i < m.numRows; i++) {
            if (i == row) {
                continue; //skips this row
            }
            for (int j = 0; j < m.numCols; j++) {
                if (j == col) {
                    continue; //skips this column
                }
                matrixMinor.matrix[mRow][mCol] = m.matrix[i][j];
                mCol++; //move to the next column in the minor matrix
            }
            mRow++; //move to the next row in the minor matrix
            mCol = 0; //after moving reset the column counter
        }
    }
    return matrixMinor;
}



//returns the determinat of a matrix
double matrixDeterminant(Matrix m){
    //null matrix case
    if(m.matrix == NULL){
        return 0;
    }
    //base case
    if (m.numRows == 1 && m.numCols == 1) {
        return m.matrix[0][0];
    }
    //actuall determinant calculation
    double det = 0;
    for(int i = 0; i < m.numRows; i++){
        //matrixMinor at row 0 and col i for computer counting purposes
        Matrix minor = matrixMinor(m, 0, i);
        double cofactor = m.matrix[0][i] * customPow(-1, i+2) * matrixDeterminant(minor);
        det += cofactor;
        freeMatrix(minor);
    }
    return det;
}

//creates and returns the adjoint matrix used to obtain inverse
Matrix matrixAdjoint(Matrix m){
    int numRows = m.numRows;
    int numCols = m.numCols;
    Matrix adjointMatrix = createMatrix(numRows, numCols);
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            Matrix minorMatrix = matrixMinor(m, i, j);
            //adjoint matrix equals transpose of cofactor matrix so I swapped i and j and also multiplied the determinant by power of -1 to get the
            adjointMatrix.matrix[j][i] = customPow(-1, i+j) * matrixDeterminant(minorMatrix);
            freeMatrix(minorMatrix);
        }
    }
    return adjointMatrix;
}

//creates the matrix inverse from the determinat and adjoint matrix given a matrix
Matrix matrixInverse(Matrix matrix){
    double det = matrixDeterminant(matrix); //should be 0.25
    Matrix adjoint = matrixAdjoint(matrix); 
    int numRows = matrix.numRows;
    int numCols = matrix.numCols;
    Matrix inverse = createMatrix(numRows, numCols);
    //loops through the matrix while dividing everthing by the determinant to get inverse
    if(det != 0){
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numCols; j++){
                inverse.matrix[i][j] = adjoint.matrix[i][j]/det;
            }
        }
    }
    freeMatrix(adjoint);
    return inverse; 
}

//transposes a matrix and returns it
Matrix transpose(Matrix matrix){
    int row = matrix.numRows;
    int col = matrix.numCols;
    Matrix mTranspose = createMatrix(col, row);
    for(int i = 0; i<row; i++){
        for(int j = 0; j<col; j++){
            mTranspose.matrix[j][i] = matrix.matrix[i][j];
        }
    }
    return mTranspose;
}

//function used for matrix multiplication not sure if I need to use this function yet
Matrix matrixMultiplication(Matrix currMatrix, Matrix originalMatrix){
    //gets numRows from current, get numCols from originalMatrix, then get the common value k (i*k)*(k*j)
    int numRows = currMatrix.numRows;
    int commonVal = currMatrix.numCols;
    int numCols = originalMatrix.numCols;
    Matrix resultMatrix = createMatrix(numRows, numCols);
    //interates through resulting matrix
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
            //creates sum of cell
            double sum = 0;
            //interates through row of currMatrix and col of originalMatrix
            for(int k = 0; k < commonVal; k++){
                sum += (currMatrix.matrix[i][k] * originalMatrix.matrix[k][j]);
            }
            //sets resultMatrix at i,j to the values of vector sum of row i of currMatrix and col i of originalMatrix
            resultMatrix.matrix[i][j] = sum;
        }
    }
    return resultMatrix;
}

//prints out the matrix but need to fix
void printMatrix(Matrix matrix) {
    if(matrix.matrix == NULL){
        return;
    }
    //nested for loop to interate through matrix
    for (int i = 0; i < matrix.numRows; i++) {
        for (int j = 0; j < matrix.numCols; j++) {
            //prints the value at i,j
            printf("%.0lf", matrix.matrix[i][j]);
            //prints a space between elements except for the last one in each row
            if (j < matrix.numCols - 1) {
                printf(" ");
            }
        }
        //jumps to new line after each row
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        return EXIT_FAILURE;
    }

    //obtains files from cml
    FILE *train = fopen(argv[1], "r");
    FILE *data = fopen(argv[2], "r");
    if (train == NULL || data == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    //reads file for fileType size
    char file1Type[100];
    char file2Type[100];    
    fscanf(train, "%s ", file1Type);
    fscanf(data, "%s ", file2Type);
    if(strcmp(file1Type, "train") != 0|| strcmp(file2Type, "data") != 0){
        return EXIT_FAILURE;
    }

    //reads file for matrix sizes
    int file1Col, file1Row;
    int file2Col, file2Row;
    fscanf(train, "%d ", &file1Col);
    fscanf(data, "%d ", &file2Col);
    fscanf(train, "%d ", &file1Row);
    fscanf(data, "%d ", &file2Row);
    if(file1Col == 0 || file1Row == 0 || file2Col == 0 || file2Row == 0){
        return EXIT_FAILURE;
    }

    //allocates matrix trainX of size fileRow and file1Col + 1 
    Matrix trainX = createMatrix(file1Row, file1Col + 1);
    //allocates matrix trainXTranspose of size file1Col + 1  and file1Row
    Matrix trainXTranspose = createMatrix(file1Col + 1, file1Row);
    //allocates matrix Y of size fileRow and 1
    Matrix Y = createMatrix(file1Row, 1);
    //allocates matrix dataX of size file2Row and file1Col + 1
    Matrix dataX = createMatrix(file2Row, file2Col + 1);

    //nested for loop to read trainX, trainXTranspose and Y from file
    for(int i = 0; i<file1Row; i++){
        double num = 1;
        for(int j = 0; j<file1Col + 1; j++){
            if(j == 0){
                trainX.matrix[i][j] = num;
                trainXTranspose.matrix[j][i] = num;
            }else{
                fscanf(train, " %lf", &trainX.matrix[i][j]);
                trainXTranspose.matrix[j][i] = trainX.matrix[i][j];
            }
        }
        fscanf(train, " %lf", &Y.matrix[i][0]);    
    }

    //nested for loop to read dataX
    for(int i = 0; i<file2Row; i++){
        for(int j = 0; j<file2Col + 1; j++){
            if(j == 0){
                dataX.matrix[i][j] = 1;
            }else{
                fscanf(data, " %lf", &dataX.matrix[i][j]);
            }
        }
    }

    // printMatrix(trainX);
    // printMatrix(trainXTranspose);
    // printMatrix(dataX);

    //begin to obtain W from (trainXTranspose * trainX)^(-1) * trainXTranspose * Y
    Matrix trainXMult = matrixMultiplication(trainXTranspose, trainX); //works
    Matrix trainXMultInver = matrixInverse(trainXMult); //dosen't work
    Matrix InverTransposeProduct = matrixMultiplication(trainXMultInver, trainXTranspose);
    Matrix W = matrixMultiplication(InverTransposeProduct, Y);

    // printMatrix(trainXMult);
    // printMatrix(W);


    //Use dataX*W to get Y
    Matrix resultY = matrixMultiplication(dataX, W);

    //frees all matrices
    freeMatrix(trainX);
    freeMatrix(trainXTranspose);
    freeMatrix(Y);
    freeMatrix(dataX);
    freeMatrix(trainXMult);
    freeMatrix(trainXMultInver);
    freeMatrix(InverTransposeProduct);
    freeMatrix(W);

    //prints out the final matrix
    printMatrix(resultY);
    //frees the final matrix
    freeMatrix(resultY);
    fclose(train);
    fclose(data);

    return EXIT_SUCCESS;
}