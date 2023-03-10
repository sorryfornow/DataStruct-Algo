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


void splitLL(NodeT *start) {
    /** using 2 pointer to check the middle position of the list hereby
     *  the ptr1 and ptr2 donate the slow and the fast pointer respectively
     */
    if(start!=NULL) {
        NodeT *ptr1 = start;            //slow pointer
        NodeT *ptr2 = start->next;      //fast pointer
        while (ptr2!=NULL && ptr2->next!=NULL) {
            ptr1 = ptr1->next;          // ptr1 moves faster than ptr2 to reach the last
            ptr2 = ptr2->next->next;    // meanwhile ptr2 reach the middle element
        }
        NodeT *part2 = ptr1->next;      // the second part begins from the middle element
        ptr1->next = NULL;              // split the linked list
        printf("\nFirst part is ");
        showLL(start);              // show the first part
        if (part2!=NULL){
            printf("\nSecond part is ");
            showLL(part2);          // show the second part if exist
            freeLL(part2);
        }
        freeLL(start);
    }
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
        NodeT * start = all;                // record the initial address of the list
        printf("Enter an integer: ");
        while(scanf("%d", &num)){
            all = joinLL(all,num);  // add the continuous elements to the list
            printf("Enter an integer: ");
        }
        printf("Done. List is ");
        showLL(all);                   // show the whole list
        splitLL(start);                    // output the very two parts of the split list
        //freeLL(all);                     // free arr in the heap
    }else{
        printf("Done.");                   // print "Done." if no valid element input
    }
    return 0;
}
