#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntactical.h"

// I am assuming that the function which gives me the rule to be added returns a structure which has
// a field called nonterminal whose value is the non-terminal whose rule is being added
// the other field in the structure is an array of variables which is the reverse ordered rule called rule
// another field is the length of the rule called length

// declaration of function to fetch rule from the predictive parsing table
// add its definition

// structure for the tree node

// function to create a new tree_node
struct tree_node *create_tree_node(struct Variable data)
{
    struct tree_node *new_tree_node = (struct tree_node *)malloc(sizeof(struct tree_node));
    new_tree_node->data = data;
    new_tree_node->next = NULL;
    new_tree_node->head = NULL;
    new_tree_node->parent = NULL;
    return new_tree_node;
}
void serialize_node(struct tree_node *node, FILE *output)
{
    if (node == NULL)
        return;

    // Serialize current node data and its relationships
    fprintf(output, "{\n");
    fprintf(output, "  \"current\": \"%p\",\n", (void *)node);

    if (node->data.flag == 1)
    {
        fprintf(output, "  \"data\": \"%s\",\n", NonTerminalToString(node->data.val)); // Assuming data can be represented as a string
    }
    else
    {
        fprintf(output, "  \"data\": \"%s\",\n", TokenToString(node->data.val)); // Assuming data can be represented as a string
    }
    if (node->parent != NULL)
    {
        fprintf(output, "  \"parent\": \"%p\",\n", (void *)node->parent);
    }
    if (node->next != NULL)
    {
        fprintf(output, "  \"next\": \"%p\",\n", (void *)node->next);
    }
    fprintf(output, "  \"children\": [\n");

    struct tree_node *child = node->head;
    while (child != NULL)
    {
        serialize_node(child, output);
        child = child->next;
        if (child != NULL)
            fprintf(output, ",\n");
    }

    fprintf(output, "  ]\n");
    fprintf(output, "}\n");
}

void serialize_tree(struct tree_node *root)
{
    printf("%d", root->data.val);
    printf("Serializing tree\n");
    FILE *output = fopen("tree_structure.json", "w");
    if (output == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    serialize_node(root, output);

    fclose(output);
}

// function to add a tree_node to the linkedlist which functions as the children of the parse tree
// the tree_nodes are added to the head (they are in reverse order)
void add_tree_node(struct tree_node *parent, struct tree_node *child)
{
    child->next = parent->head;
    parent->head = child;
    child->parent = parent;
}

// function to find the next non terminal for a given node
struct tree_node *nextNonTerminal(struct tree_node *current)
{
    if (current->data.flag == 1)
    {
        printf("FINDING NEXT NT OF: %s\n", NonTerminalToString(current->data.val));
    }
    else
    {
        printf("FINDING NEXT NT OF: %s\n", TokenToString(current->data.val));
    }
    while (current->next != NULL && current->data.flag == 0)
    {
        current = current->next;
    }

    if (current->data.flag == 1)
    {
        printf("RETURNED: %s\n", NonTerminalToString(current->data.val));
        return current;
    }
    while (current->parent != NULL && current->parent->next == NULL)
    {
        current = current->parent;
    }
    if (current->parent == NULL)
    {
        return NULL;
    }
    return nextNonTerminal(current->parent->next);
}

// function which takes the structure described in the first few comments as input and the tree_node where the rule has to be added
struct tree_node *repeated_add(struct tree_node *parent, struct input_structure input, int a, struct ParsingTable *pt)
{

    if (parent->data.val != input.nonterminal.val)
    {
        printf("Error: The input does not match the first non-terminal found\n");
        printf("Non-terminal entered: %s\n", NonTerminalToString(input.nonterminal.val));
        printf("Non-terminal found: %s\n", NonTerminalToString(parent->data.val));
        return nextNonTerminal;
    }

    for (int var = 8; var >= 0; var -= 1)
    {
        if (isDefault(pt->table[input.nonterminal.val][a][var]))
        {
            continue;
        }

        // if (pt->table[input.nonterminal.val][a][var].flag == 1)
        // {
        //     printf("ADDED TO TREE: %s\n", NonTerminalToString(pt->table[input.nonterminal.val][a][var].val));
        // }
        // else
        // {
        //     printf("ADDED TO TREE: %s\n", TokenToString(pt->table[input.nonterminal.val][a][var].val));
        // }

        input.length += 1;

        add_tree_node(parent, create_tree_node(pt->table[input.nonterminal.val][a][var]));
    }

    return nextNonTerminal(parent->head);
}
// parent = add_to_tree(topStack, a, pt, parent);
struct tree_node *add_to_tree(struct Variable nt, int a, struct ParsingTable *pt, struct tree_node *parent)
{
    struct input_structure inp;

    printf("NONTERMINAL IN TREE: %s\n", NonTerminalToString(nt.val));

    inp.nonterminal = nt;

    parent = repeated_add(parent, inp, a, pt);

    if (parent != NULL)
    {
        printf("PARENT POINTER; %s\n", NonTerminalToString(parent->data.val));
    }
    // printf("TREE RETURNED NEXT NONTERMINAL: %s\n", NonTerminalToString(parent->data.val));
    return parent;
}

void printParseTree(parseTree PT, char *outfile)
{
    
}
