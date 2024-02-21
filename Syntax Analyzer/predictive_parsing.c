#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "syntactical.h"


int top = -1;
int MAX=128;

bool isEmpty(int *stack) {
    return top == -1;
}

bool isFull(int * stack) {
    if (top==MAX-1)
    {
        MAX+=128;
        stack = (int *)realloc(stack,MAX*sizeof(int));
        return true;
    }
    return false; 
}

void push(int *stack, int data) {
    if (isFull(stack)) {
        printf("Stack is full\n");
    }
    stack[++top] = data;
}

int pop(int *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        return -1;
    }
    return stack[top--];
}

int peek(int *stack) {
    if (!isEmpty(stack)) {
        return stack[top];
    } else {
        printf("Stack is empty\n");
        return -1;
    }
}


void predictive_parsing(char* w, struct ParsingTable* pt, int* stack){
    //let a be the first symbol of w
    //let X be the top symbol of the stack
    //while X is not $ do
        //if X==a then 
            //pop(X)
            //a=next symbol of w
        //else if X is a terminal then
            //error
        //else if M[X,a] is an error entry then
            //error
        //else if M[X,a] = Y1Y2...Yk then
            //pop(X)
            //push(Yk)
            //push(Yk-1)
            //...
            //push(Y1)
        //let X be the top symbol of the stack
        
}

int main() {
    int* stack = (int *)malloc(MAX*sizeof(int));
    
    return 0;
}
