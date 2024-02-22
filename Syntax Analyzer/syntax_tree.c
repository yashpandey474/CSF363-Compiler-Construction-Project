#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntactical.h"

struct node{
    struct Variable *value;
    int type; // 0 for non-terminal, 1 for terminal
    struct node* children;  //children can be terminals or non_terminals
    int number_of_children; //to store the number of children
};

struct node* create_node(struct Variable *value){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->value = value;
    new_node->children = NULL;
    new_node->number_of_children = 0;
    return new_node;
}

//if the value returned is 0, then the child is successfully added
int add_child(struct node *parent, struct node *child){
    if (parent->value->flag == 0){
        printf("Error: Parent is a terminal\n");
        return -1;
    }
    if(parent->children == NULL){
        parent->children = (struct node*)malloc(sizeof(struct node));
        parent->children[0] = *child; 
    }
    else{
        struct node* temp = (struct node*)realloc(parent->children, (parent->number_of_children+1)*sizeof(struct node));
        if (temp == NULL) {
            printf("Error: Memory reallocation failed\n");
            return -1;
        }
        parent->children = temp;
        parent->children[parent->number_of_children] = *child; 
    }
    parent->number_of_children++;
    return  0;
}

int main(){
    return 0;
}