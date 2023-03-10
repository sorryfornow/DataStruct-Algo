//
// Created by Siqing Zhang on 2022/7/25.
//
#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef struct List_Node *Node;

struct List_Repr {
    Node head;  // head of the list
    int number; // #node
};

struct List_Node {
    string data;    // string data in node
    Node last;      // previous node
    Node next;      // next node
};

// meta interface
/**
 * list_create
 * allocate the required memory for a new list
 * return a pointer to the new list
 * return NULL on error
 */
list list_create (void) {
    list newList;
    newList = malloc(sizeof (struct List_Repr));
    // to program execution in case failure to allocate memory
    if (newList == NULL){
        return NULL;
    }
    // initialize the created list
    newList->head = NULL;
    newList->number = 0;
    return newList;
}

/**
 * list_destroy
 * free all memory associated with a given list
 */
void nodeFree (Node first) {
    // continue to free the successive nodes in the list
    if (first != NULL) {
        nodeFree(first->next);
    }
    // deallocate the memory block of the current node
    if (first != NULL) {
        free(first);
    }
}

void list_destroy (list inputList) {
    if (inputList != NULL){
        if (inputList->head != NULL){
            nodeFree(inputList->head);
        }
        free(inputList);
    }
}

// misc interface
/**
 * list_is_empty
 * return True if there are no items in the list, False otherwise
 * return False on error
 */
bool list_is_empty (list inputList) {
    if (inputList == NULL || inputList->head == NULL || inputList->number == 0) {
        return true;    // list is empty
    }
    return false;
    // return (inputList->number == 0)
}

/**
 * list_length
 * return the number of items in the list
 * return 0 on error
 */
size_t list_length (list inputList) {
    if (inputList != NULL) {
        return inputList->number;
    }
    return 0;
}

/**
 * nodeCreate
 * allocate the required memory for a new node
 * return a pointer to the new node
 */
Node nodeCreate(string item) {
    Node newNode = malloc(sizeof (struct List_Node));
    // to program execution in case failure to allocate memory
    if (newNode == NULL) {
        return NULL;
    }
    // initialize the created node
    newNode->last = NULL;
    newNode->next = NULL;
    // transcribe the data in the given string into node's value
    newNode->data = malloc(sizeof(char) * (strlen(item) + 1));
    strcpy(newNode->data, item);
    return newNode;
}

// stack interface
/**
 * list_push
 * add an item to the head of the stack
 */
void list_push (list inputList, string item) {
    if (inputList != NULL && item != NULL) {
        Node newNode = nodeCreate(item);
        if (newNode != NULL) {
            // change the starting node in the list
            if (inputList->head != NULL){
                inputList->head->last = newNode;
            }
            newNode->next = inputList->head;
            inputList->head = newNode;
            // increase the number of list by 1
            inputList->number++;
        }
    }
}

/**
 * list_pop
 * remove and return the item at the head of the stack
 * return NULL on error
 */
string list_pop (list inputList) {
    if (inputList != NULL && inputList->head != NULL){
        // transcribe the data in the popped node
        string item;
        item = malloc(sizeof(char) * (strlen(inputList->head->data) + 1));
        strcpy(item, inputList->head->data);

        Node first = inputList->head;
        // change the starting node
        inputList->head = inputList->head->next;
        if (inputList->head != NULL) {
            Node headNode = inputList->head;
            headNode->last = NULL; // more than 1 node
        }
        // decrease the number of list by 1
        inputList->number--;
        // free the memory of the popped node
        free(first);
        return item;
    }
    return NULL;
}

// queue interface
/**
 * list_enqueue
 * add an item to the head of the queue
 */
void list_enqueue (list inputList, string item) {
    list_push(inputList,item);
}

/**
 * list_dequeue
 * remove and return the item at the tail of the queue
 * return NULL on error
 */
string list_dequeue (list inputList) {
    if (inputList != NULL && inputList->head != NULL) {
        Node tailNode = inputList->head;
        while (tailNode!= NULL && tailNode->next != NULL) {
            tailNode = tailNode->next;
        }   // find the tail node
        // transcribe the data
        string item;
        item = malloc(sizeof(char) * (strlen(tailNode->data) + 1));
        strcpy(item, tailNode->data);
        // dequeue the tail
        if (tailNode->last != NULL) {
            Node newTail = tailNode->last;
            newTail->next = NULL;
        } else{ // there was only one node
            inputList->head = NULL;
        }
        // decrease the number of list by 1
        inputList->number--;
        free(tailNode);
        return item;
    }
    return NULL;
}

// set interface
/**
 * list_add
 * add an item into the set, if it is not already in the set
 */
void list_add (list inputList, string item) {
    if (inputList != NULL) {
        if (inputList->head != NULL){
            Node curNode = inputList->head;
            while (curNode != NULL) {
                if (strcmp(curNode->data, item) == 0){
                    break;
                }
                curNode = curNode->next;
            }
            if (curNode == NULL){
                // item was not inside
                list_push(inputList,item);
            }
        } else{ // set was empty
            list_push(inputList,item);
        }
    }
}

/**
 * list_remove
 * remove an item from the set
 */
void list_remove (list inputList, string item) {
    if (inputList != NULL && inputList->head != NULL) {
        Node curNode = inputList->head;
        while (curNode != NULL) {
            if (strcmp(curNode->data, item) == 0) {
                break;  // find the target item
            }
            curNode = curNode->next;
        }
        if (curNode != NULL) {
            if (curNode->next == NULL && curNode->last == NULL) {
                inputList->head = NULL;
            }
            else if (curNode->last != NULL && curNode->next != NULL) {
                curNode->last->next = curNode->next;
                curNode->next->last = curNode->last;
            }
            else if (curNode->last == NULL && curNode->next != NULL) {
                // head
                inputList->head = curNode->next;
                curNode->next->last = NULL;
            }
            else if (curNode->last != NULL && curNode->next == NULL) {
                curNode->last->next = NULL;
            }
            // decrease the number of list by 1
            inputList->number--;
            free(curNode);
        }
    }
}

/**
 * list_contains
 * return True if a particular item is in the set, False otherwise
 * return False on error
 */
bool list_contains (list inputList, string item) {
    if (inputList != NULL && item != NULL) {
        Node curNode = inputList->head;
        while (curNode != NULL) {
            if (curNode->data == NULL) {
                return false;
            }
            if (strcmp(curNode->data, item) == 0) {
                return true;    // find the target item
            }
            curNode = curNode->next;
        }
    }
    return false;
}
