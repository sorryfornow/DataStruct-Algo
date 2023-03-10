//
// Created by Siqing Zhang on 2022/6/5.
//
//
//#ifndef WEEK1_INTSTACK_H
//#define WEEK1_INTSTACK_H
//
//#endif //WEEK1_INTSTACK_H

// Integer Stack ADO header file

#define MAXITEMS 10

void StackInit();     // set up empty stack
int  StackIsEmpty();  // check whether stack is empty
void StackPush(int);  // insert int on top of stack
int  StackPop();      // remove int from top of stack