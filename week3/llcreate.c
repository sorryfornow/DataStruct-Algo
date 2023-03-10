#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct node {
    int data;
    struct node *next;
} NodeT;


void freeLL(NodeT *list) {
    /** using intermediate NodeT temp to transfer the heading element
     *  in the list to make sure every memory unit get free in heap.
     **/
    NodeT *p, *temp;

    p = list;
    while (p != NULL) {
        temp = p->next;
        free(p);
        p = temp;
    }
}


void showLL(NodeT *list) {
    /** show the entire list with arrow inserted in the gap of elements
     *  when the length of the linked list could be bigger than 1.
     */
    NodeT *p;
    if (list != NULL){
        printf("%d", list->data);    // the single element in the list
    }
    if (list->next != NULL){
        for (p = list->next; p != NULL; p = p->next)
            printf("-->%d", p->data);    // the continuous elements with arrow
    }
}


NodeT *makeNode(int v) {
    NodeT *new = malloc(sizeof(NodeT));
    assert(new != NULL);
    new->data = v;       // initialise data
    new->next = NULL;    // initialise link to next node
    return new;          // return pointer to new node
}


NodeT *joinLL(NodeT *list, int v){
    NodeT *current = list;          // catch the current position in the linked list
    while (current->next != NULL) {
        current = current->next;    // find the last if the current is not
    }
    current->next = makeNode(v);    // create a new linked node with data v
    return list;
}


int main() {
    int num;
    printf("Enter an integer: ");
    // check if the first element in the linked list is valid
    if (scanf("%d", &num)){
        NodeT *all = malloc(sizeof (NodeT));
        assert(all!=NULL);
        // if so, initialize the linked list
        all->next=NULL;
        all->data=num;
        printf("Enter an integer: ");
        while(scanf("%d", &num)){
            all = joinLL(all,num);  // add the continuous elements to the list
            printf("Enter an integer: ");
        }
        printf("Done. List is ");
        showLL(all);
        freeLL(all);                    // free arr in the heap
    }else{
        printf("Done.");                    // if there is no valid element input
    }
    return 0;
}
