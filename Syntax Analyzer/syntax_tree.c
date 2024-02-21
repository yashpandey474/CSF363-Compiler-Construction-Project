//tree with variable number of children where leaves are terminals and nodes are non terminals

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntactical.h"

struct node {
    struct Variable data;  //this stores the data of the non terminal which is represented by this node
    struct node *children; //this is the array of pointers to the next nodes
    int length; //this stores the number of children of this node
    
};

struct leaf{
    struct Variable data; //this stores the data of the terminal which is represented by this node
};

struct node* create_node(struct Variable data, int length) {
    struct node *new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->children = (struct node*)malloc(length * sizeof(struct node));    //allocating memory for the array of pointers
    new_node->length = length;
    return new_node;
}

struct leaf* create_leaf(struct Variable data) {
    struct leaf *new_leaf = (struct leaf*)malloc(sizeof(struct leaf));
    new_leaf->data = data;
    return new_leaf;
}

void add_child(struct node *parent, struct Variable data, int length) {
    struct node *new_node = create_node(data, length);
    parent->children[parent->length++] = *new_node;
}



