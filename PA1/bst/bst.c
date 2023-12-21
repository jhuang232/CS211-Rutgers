#include <stdio.h>
#include <stdlib.h>

//creates structure of the node
struct node{
    int num;
    struct node* leftChild;
    struct node* rightChild;
};

//global variable root to bst
struct node* root = NULL;

struct node* createNode(int value){
    //allocated memory for new node
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->num = value;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    return newNode;
}

void freeTree(struct node* root){
    if(root == NULL){
        return;
    }
    freeTree(root->leftChild);
    freeTree(root->rightChild);
    free(root);
}

//return 0 if present and 1 if not
int search(struct node* parent, int value){
    //makes sure that 
    if(parent == NULL){
        return 1;
    }

    if(value == parent->num){
        return 0;
    }
    else if(value < parent->num){
        return search(parent->leftChild, value);
    } 
    else if(value > parent->num){
        return search(parent->rightChild, value);
    } 

    return 1;
}

//find max of left tree by passing through the parent's leftChild
struct node* searchLeftMax(struct node* parentLeft){
    //base case
    if(parentLeft->rightChild == NULL){
        return parentLeft;
    }
    return searchLeftMax(parentLeft->rightChild);
}

//if inserted return 0 if not return 1
int insert(struct node* parent, int value){
    //not not already inserted
    if(search(parent, value) == 0){
        return 1;
    }

    //if parent is NULL then set root
    if(parent == NULL){
        root = createNode(value);
        return 0;
    }
    
    //if value is to be inserted to left of parent
    if(value < parent->num){
        if(parent->leftChild == NULL){
            parent->leftChild = createNode(value);
            return 0;
        }else return insert(parent->leftChild, value);
    } 
    //if value is to be inserted to right of parent
    else if(value > parent->num){
        if(parent->rightChild == NULL){
            parent->rightChild = createNode(value);
            return 0;
        }else return insert(parent->rightChild, value);
    } 

    return 1;
}

//return 0 if deleted and 1 if not
int delete(struct node** parent, int target){
    //check if value is actually in the bst
    if(search((*parent), target) == 1){
        return 1;
    }
    if(*parent == NULL){
        return 1;
    }
    //finds node to be deleted
    if(target < (*parent)->num){
        return delete(&((*parent)->leftChild), target);
    } 
    else if(target > (*parent)->num){
        return delete(&((*parent)->rightChild), target);
    } else if((*parent)->num == target){
        //case one no children just remove
        if(((*parent)->leftChild == NULL) && ((*parent)->rightChild == NULL)){
            struct node* temp = *parent;
            *parent = NULL;            
            free(temp);
            return 0;
        }
        //case two one child set child to parent and then remove parent and replace with child
        else if(((*parent)->leftChild == NULL) || ((*parent)->rightChild == NULL)){
            //if parent has leftChild then set left child equal to the parent and then set parent to leftChild and then delete leftChild
            if ((*parent)->leftChild == NULL) {
                struct node* temp = *parent;
                *parent = (*parent)->rightChild;
                free(temp);
                return 0;
            }
            else if ((*parent)->rightChild == NULL) {
                struct node* temp = *parent;
                *parent = (*parent)->leftChild;
                free(temp);
                return 0;
            }
        }
        //case three two children find predecessor store it and then delete it and set parent to predecessor 
        else if(((*parent)->leftChild != NULL) && ((*parent)->rightChild != NULL)){
            struct node* temp = searchLeftMax((*parent)->leftChild);
            (*parent)->num = temp->num;
            delete(&((*parent)->leftChild), temp->num);            
            return 0;
        }
    }
    return 1;
}

void printTree(struct node* root) {
    if (root == NULL) {
        return;
    }
    printf("(");
    printTree(root->leftChild);
    printf("%d", root->num);
    printTree(root->rightChild);
    printf(")");
}

int main(int argc, char* argv[]){
    char action;
    int value;

    while(scanf("%c ", &action) != EOF){
        if(action == 'i'||action == 's'||action == 'd'){
            scanf("%d ", &value);
        }
        if(action == 'i'){
            if(insert(root, value) == 0){
                printf("inserted\n");
            }else printf("not inserted\n");
        }
        else if(action == 's'){
            if(search(root, value) == 0){
                printf("present\n");
            }else printf("absent\n");
        }
        else if(action == 'p'){
            printTree(root);
            printf("\n");
        }
        else if(action == 'd'){
            if(delete(&root, value) == 0){
                printf("deleted\n");
            }else printf("absent\n");
        }
    }
    freeTree(root);
    return EXIT_SUCCESS;
}