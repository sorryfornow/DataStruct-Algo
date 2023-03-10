#include <stdio.h> // include standard I/O library defs and functions

#define SIZE 10000     // define a symbolic constant

typedef struct node {
    int height;
    int length;
} Node;

int compareToOne(Node a, Node b) {
    if (a.height < b.height) {
        return -1;
    } else if (a.height > b.height) {
        return 1;
    } else {
        return 0;
    }
 }


int compareToTwo(Node a, Node b) {
    // 先比较length， 再比较height
    if (a.length < b.length) {
        return -1;
    } else if (a.length > b.length) {
        return 1;
    } else {
        return compareToOne(a, b);
    }
 }

void insertionSortOne(Node array[], int n) {  // function headers must provide types
    int i;                                 // each variable must have a type
    for (i = 1; i < n; i++) {              // for-loop syntax
        Node element = array[i];
        int j = i-1;
        while (j >= 0 && compareToOne(array[j], element) > 0) {  // logical AND
            array[j+1] = array[j];
            j--;                                 // abbreviated assignment j=j-1
        }
        array[j+1] = element;
    }
}

void insertionSortTwo(Node array[], int n) {  // function headers must provide types
    int i;                                 // each variable must have a type
    for (i = 1; i < n; i++) {              // for-loop syntax
        Node element = array[i];
        int j = i-1;
        while (j >= 0 && compareToTwo(array[j],  element) > 0) {  // logical AND
            array[j+1] = array[j];
            j--;                                 // abbreviated assignment j=j-1
        }
        array[j+1] = element;
    }
}

int main(void) {
    // 不会用指针，只能用数组
    //
    Node **node_arrays;
    Node nodes[SIZE];
    nodes[0].height = 15;
    nodes[0].length = 9999;
    nodes[1].height = 1;
    nodes[1].length = 30000;
    nodes[2].height = 20;   
    nodes[2].length = 10000;
    nodes[3].height = 40;   
    nodes[3].length = 50000;
    nodes[4].height = 0;   
    nodes[4].length = 10000;
    nodes[5].height = 60;   
    nodes[5].length = 44343;
    int i;
    insertionSortOne(nodes, SIZE);
     printf("按照height排序\n");
    for (i = 0; i < SIZE; i++)
        printf("height: %d, length: %d\n", nodes[i].height,nodes[i].length);             
    
    insertionSortTwo(nodes, SIZE);
    printf("先排序length,再排序height:\n");
    for (i = 0; i < SIZE; i++)
        printf("height: %d, length: %d\n", nodes[i].height,nodes[i].length);             
    return 0;           
}