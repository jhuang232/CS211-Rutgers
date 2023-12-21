#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    //checks for number of parameters
    if(argc <= 1){
        return EXIT_FAILURE;
    }
    //create an array of pointers to char arrays ussign malloc with size +1 to account for the added NULL spot
    char **arrayString = (char **)malloc((argc + 1) * sizeof(char *));    
    //makues sure that arrayString isn't null
    if (arrayString == NULL) {
        return EXIT_FAILURE;
    }
    //copies over cml arguments to arrayString excluding argv[0] since it is the file name
    for (int i = 1; i < argc; i++) {
        arrayString[i - 1] = argv[i];
    }
    //makes the extra last element in array NULL so insertion sort algo knows when to stop
    arrayString[argc - 1] = NULL;
    //performs insertion sort on arrayString
    for (int i = 1; i < argc - 1; i++) {
        char *current = arrayString[i];
        int j = i - 1;

        //compare current with the elements before it to make sure that everything to left is less than current lexicographically
        while (j >= 0 && strcmp(arrayString[j], current) > 0) {
            arrayString[j + 1] = arrayString[j];
            j--;
        }

        //puts current in its correct position
        arrayString[j + 1] = current;
    }
    
    //prints contents in arrayString which are the sorted strings
    for(int i = 0; arrayString[i] != NULL; i++){
        printf("%s\n", arrayString[i]);
    }
    
    //frees the malloced array
    free(arrayString);
    return EXIT_SUCCESS;
}