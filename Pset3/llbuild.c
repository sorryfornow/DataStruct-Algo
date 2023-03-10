#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




typedef struct node{
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
    NodeT *temp=list;
    while (temp->next !=NULL){
        temp = temp->next;
    }
    NodeT *tail = makeNode(d);
    temp->next = tail;
    return list;
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
    printf("Done. List is ");
    p = list;
    while ( p != NULL){
        printf("%d",p->data);
        if (p->next!= NULL){
            printf("->");
        }
        p = p->next;
    }
}

void newShow(NodeT *list) {
    NodeT *p;
    p = list;
    while ( p != NULL){
        printf("%d",p->data);
        if (p->next->next!= NULL){
            printf("->");
        }
        p = p->next->next;
    }
}

int main(){
    int n;
    NodeT * all = NULL;
    printf("Enter a number: ");
    if (scanf("%d", &n)) {
        all = makeNode(n);
        assert(all!=NULL);
        // 从第二个开始判断是否继续插入node
        printf("Enter a number: ");
        while(scanf("%d", &n)){
            if (n%2){
                // 如果 odd
                all = appendLL(all, n);
            } else{
                all = insertLL(all,n);
            }
            printf("Enter a number: ");
        }
        showLL(all);

        freeLL(all);

        printf("Even-numbered elements are: ");
        if(all->next != NULL){
            newShow(all->next);
        }
        printf("\nOdd-numbered elements are: ");
        newShow(all);
    } else{
        printf("Done.");
    }

}
