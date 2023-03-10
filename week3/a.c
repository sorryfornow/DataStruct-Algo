#include <stdio.h>
#include <stdlib.h>



typedef struct node {
    int data;
    struct node *next;
} NodeT;

typedef NodeT * Node;

struct list {
    Node head;
    int total;
};

typedef struct list *List;


List createList() {

    List newList = malloc(sizeof (struct list));
    newList->head = NULL;
    newList->total = 0;

    return newList;
}



Node makeNode(int data) {
    Node newNode = malloc(sizeof(NodeT));
    if (newNode != NULL) {
        newNode->data = data;
        // 务必
        newNode->next = NULL;
    }
    return newNode;
}


Node insertLL(Node head, int data) {
    // 1. head, 2, 中间，3， 最后一个
    // head，最后
    Node newNode = makeNode(data);
    if (head == NULL) {
        head = newNode;
    } else {
        Node cur = head;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        // cur next已经为NULL了
        cur->next = newNode;
    }
    return head;
}

void insertList(List list, int data) {
    // 1. head, 2, 中间，3， 最后一个
    // head，最后
    Node newNode = makeNode(data);
    if (list->head == NULL) {
        list->head  = newNode;
    } else {
        Node cur = list->head ;
        while (cur->next != NULL) {
            cur = cur->next;
        }
        // cur next已经为NULL了
        cur->next = newNode;
    }
    list->total++;
}





void showLL(Node head) {
    if(head!=NULL) {
        printf("%d", head->data);
        Node cur = head->next;
        while(cur!=NULL) {
            printf(" -> %d", cur->data);
            cur = cur->next;
        }

        printf("\n");
    }
}

void freeLL(Node head) {
    while(head!=NULL) {
        Node temp = head->next;
        free(head);
        head = temp;
    }
}

void split(Node head) {
    if(head!=NULL) {
        Node slow = head;
        Node fast = head->next;
        while (fast!=NULL && fast->next!=NULL) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node first = head;
        Node second = slow->next;
        slow->next = NULL;
        showLL(first);
        showLL(second);

        freeLL(first);
        freeLL(second);
    }

}




int main() {
    List all = createList();
    while(1) {
        int number = 0;
        printf("请输出:");
        if(scanf("%d", &number) == 1) {
            all->head = insertLL(all->head, number);
        } else {
            printf("Done\n");
            break;
        }
    }
    showLL(all->head);

    split(all->head);
}