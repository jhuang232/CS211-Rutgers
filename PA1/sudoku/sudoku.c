#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkRowCol(int **matrix){
    //for every row check if there are numbers 1..9 
    for(int j = 0; j <9; j++){
        //int[] to check for nums use index as a couter
        int numChecker[] = {0,0,0,0,0,0,0,0,0};
        for(int k = 0; k<9; k++){
            //increments index as it hits target
            numChecker[matrix[k][j] - 1] += 1;
            numChecker[matrix[j][k] - 1] += 1;
        }
        for(int i =0; i<9; i++){
            if(numChecker[i] != 2){
                return 1;
            }
        }
    }
    return 0;
}

int checkBox(int **matrix){
    //first for loop gives x index for top left corner of box
    for(int j = 0; j <9; j+=3){
        //second for loop gives y index for top left corner of box
        for(int i = 0; i <9; i+=3){
            //int[] to check for nums use index as a couter
            int numChecker[] = {0,0,0,0,0,0,0,0,0};
            //3rd for loop to lop through x index of box
            for(int n = j; n<j+3; n++){
                //4th for loop to loop through y index of box
                for(int m = i; m< i+3; m++){
                    //increments index as it hits target
                    numChecker[matrix[n][m] - 1] += 1;
                }
            }
            for(int i =0; i<9; i++){
                if(numChecker[i] != 1){
                    return 1;
                }
            }
        }
    }
    return 0;
}

//solves the sudoku at x,y
void solve(int **matrix, int row, int col){
    //int[] to check for nums use index as a couter
    int numChecker[] = {0,0,0,0,0,0,0,0,0};

    //search row, col, and box, to check for missing value; value is denoted by 0 in numChecker[]
    //search row, col
    for(int j = 0; j < 9; j++){
        //increments index as it hits target
        if(matrix[row][j] != 0) numChecker[matrix[row][j] - 1] += 1;
        if(matrix[j][col] != 0) numChecker[matrix[j][col] - 1] += 1;
    }
    //search box
    int boxStartX = row - (row % 3);
    int boxStartY = col - (col % 3);
    //3rd for loop to lop through x index of box
    for(int n = boxStartX; n < boxStartX+3; n++){
    //4th for loop to loop through y index of box
        for(int m = boxStartY; m < boxStartY+3; m++){
            //increments index as it hits target
            if(matrix[n][m] != 0) numChecker[matrix[n][m] - 1] += 1;
        }
    }

    for(int i=0; i<9; i++){
        //if their is valid sub print solvable else unsolveable
        if(numChecker[i] == 0){
            matrix[row][col] = i+1;
            printf("solvable");
            return;
        }
    }

    printf("unsolvable");
    return;
}

//checks to see it sudoku is correct
int correctness(int **matrix){
    int correctConditions = 0;
    //check to see if all conditions are meet
    correctConditions += checkRowCol(matrix) + checkBox(matrix);
    //if correct return 0 else return 1
    if(correctConditions != 0) return 1;
    return 0;
}

int main(int argc, char *argv[]){
    //checks for number of cml inputs 
    if(argc != 2){
        printf("error");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        return EXIT_FAILURE;
    }
    // allocate 1 pointer per row
    int **board = malloc(9 * sizeof(int *));
    // allocate all rows at once
    board[0] = malloc(9 * 9 * sizeof(int));
    // set row pointers
    for (int i = 1; i < 9; i++) {
        board[i] = &board[0][i*9];
    }

    //parameters for solve 
    int needsSolving = 0;
    int x = 0;
    int y = 0;
    //counter for correct number of inputs //81
    int totalinputs = 0;
    //copy over inputs from the file to the 2D array
    for(int i = 0; i < 9; i++){
        for(int j = 0; j< 9; j++){
            char num;
            int result = fscanf(file, " %c", &num);
            totalinputs += result;
            if(num == '_') {
                num = '0'; 
                needsSolving = 1; 
                x = i; 
                y = j;
            }
            board[i][j] = (int)(num - '0');
        }
    }
    
    if(totalinputs != 81){
        return EXIT_FAILURE;
    }
    //pass on the 2D array to correctness()

    if(needsSolving != 0){
        solve(board, x,y);
    }
    else{
        int valid = correctness(board);
        (valid == 0) ? printf("correct"): printf("incorrect");
    }
        
    free(board[0]);
    free(board);
    
    return EXIT_SUCCESS;
}
