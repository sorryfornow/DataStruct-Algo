#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct{
    int data;
    struct node *next;
} NodeT;

// Create a new list node:
NodeT *makeNode(int v) {
    NodeT *new = malloc(sizeof(NodeT));
    assert(new != NULL);
    new->data = v;       // initialise data
    new->next = NULL;    // initialise link to next node
    return new;          // return pointer to new node
}

// Insert a new element at the beginning:
NodeT *insertLL(NodeT *list, int d) {
    NodeT *new = makeNode(d);  // create new list element
    new->next = list;          // link to beginning of list
    return new;                // new element is new head
}

// insert a new element at the end:
NodeT *appendLL(NodeT *list, int d){
    if (list == NULL){
        return makeNode(d);
    }else {
        NodeT *temp=list;
        while (temp->next !=NULL){
            temp = temp->next;
        }
        NodeT *tail = makeNode(d);
        temp->next = tail;
        return list;
    }
}

//free a list
void freeLL(NodeT *list) {
    NodeT *p, *temp;

    p = list;
    while (p != NULL) {
        temp = p->next;
        free(p);
        p = temp;
    }
}

// Print all elements:
void showLL(NodeT *list) {
    NodeT *p;
    for (p = list; p != NULL; p = p->next)
        printf("Done. List is ", p->data);
}


int main(){
    int n;
    printf("Enter a number:\n");
    scanf("%d",&n);
    NodeT *list = malloc(sizeof(NodeT));
    if (n%2 == 0 && list != NULL) { 
        list = list->next;
        list = insertLL(list, n);
    } else if (n%2 != 0 && list != NULL) {
    {
        list = appendLL(list, n);
    }
    
}