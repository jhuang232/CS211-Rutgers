#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    //cheaks for number of parameters
    if(argc != 2){
        printf("Error: Expects only one argument");
        return EXIT_FAILURE;
    }
    //creates a pointer called string to first char in argument
    char *string = argv[1];

    //for loop to iterate through the char array while the char is not null (meaning terminated)
    for (int i = 0; string[i] != '\0'; i++){
        if(isalpha(string[i])){
            //find the start of the alpha character's ASCII based on letter casing
            char start = islower(string[i])? 'a':'A';
            //using start get the diffrence between the current char and the base add 13 to get the cypher mod 26 to make sure that it is in bounds
            //using that add it to the start so that you get the proper cypher
            string[i] = (string[i] - start + 13)%26 + start;
        }
    }
    
    printf("%s\n", string);
    

    return EXIT_SUCCESS;
}