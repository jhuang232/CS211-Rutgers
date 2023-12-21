#include <stdio.h>
#include <stdlib.h>

//creates structure of the node
struct node{
    int num;
    struct node* next;
};

//global variable head to the linked list 
struct node* head = NULL;

struct node* createNode(int value){
    //allocated memory for new node
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->num = value;
    newNode->next = NULL;
    return newNode;
}

//function to search throught linked list to see if target is already in there
//returns index if found and -1 if not found
int find(int target){
    int count = 0;
    if(head != NULL){
        for(struct node* ptr = head; ptr != NULL; ptr = ptr->next){
            if(ptr->num == target){
                return count;
            }
            count++;
        }
    }
    return -1;
}

//insert node function also if there are duplicates then do not add the new node
//also have to sort it
//return 0 if inserted and 1 if not inserted
int insert(int value){
    //if value is found in LL return
    if(find(value) > -1){
        return 1;
    }
    else{
        //begins insertion here
        struct node* newNode = createNode(value);
        //handles NULL head or insertion at head
        if((head == NULL) || (head->num > value)){
            newNode->next = head;
            head = newNode;
            return 0;
        }

        //for all other cases inclueding insertion at tail
        //traverse LL to find isertion spot
        struct node* insertAfter = head;
        while((insertAfter->next != NULL) && insertAfter->next->num < value){
            insertAfter = insertAfter->next;
        }

        //adds the node to LL
        newNode->next = insertAfter->next;
        insertAfter->next = newNode;
        return 0;
    }
    return 1;
}

//delete node function
//only deleted if the node with corresponding value is found
//return 0 if deleted and 1 if not inserted
int delete(int target){
    //uses find function to check if the value is in LL
    int index = find(target);
    //if it is not found just return 1
    if(index == -1){
        return 1;
    }

    //deleting head node
    if(index == 0){
        struct node* temp = head;
        head = head->next;
        free(temp);
        return 0;
    }

    //goes through LL until it reaches the index returned by find
    struct node* prevDelete = head;
    for(int i = 0; i < index -1 ; i++){
        prevDelete = prevDelete->next;
    }

    //deletes the node after prevDelete
    struct node* temp = prevDelete->next;
    prevDelete->next = prevDelete->next->next;
    free(temp);
    return 0;
}

//used to print out the contents of the linked list
void printList(int length){
    printf("%d :", length);
    for(struct node* ptr = head; ptr != NULL; ptr = ptr->next){
        printf(" %d", ptr->num);
    }
    printf("\n");
}

void freeLL(struct node* start){
    if(start == NULL){
        return;
    }
    freeLL(start->next);
    free(start);
}

int main (int argc, char* argv[]){
    char action;
    int value;
    int length = 0;
    while(scanf(" %c %d\n", &action, &value) != EOF){
        if(action == 'i'){
            if(insert(value) == 0) length += 1;
            printList(length);
        }
        else if (action == 'd'){
            if(delete(value) == 0) length -= 1;
            printList(length);
        }
    }
    freeLL(head);
    return EXIT_SUCCESS;
}