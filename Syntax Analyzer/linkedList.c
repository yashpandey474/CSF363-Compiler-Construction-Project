#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<syntactical.h>
//st
//structure of Linked list node
struct Node
{
  struct Variable data;
  struct node *next;
};
// Function to add an elements at the beggining of the linked list
void insert_at_beginning(struct Node** headRef, int newData) {
    // Allocate memory for new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    
    // Check if memory allocation was successful
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    
    // Assign data to the new node
    newNode->data = newData;
    
    // Set the next pointer of the new node to point to the current head
    newNode->next = *headRef;
    
    // Update the head to point to the new node
    *headRef = newNode;
}
// Function to create a singly linked list with the given elements
struct Node* createLinkedList(struct Data* elements, int size) {
    struct Node* head = NULL;
    struct Node* tail = NULL;

    // Iterate through the elements array
    for (int i = 0; i < size; i++) {
        // Create a new node
        struct Node* newNode = createNode(elements[i]);

        // If it's the first node, set it as head
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            // Otherwise, link it to the last node
            tail->next = newNode;
            tail = newNode;
        }
    }

    return head;
}
// Function to print all elements of the linked list
void printList(struct Node* head) {
    struct Node* current = head;

    // Traverse the list and print each element
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main() {
    char str[] = "This is a sample string.";
    char *token;

    printf("Splitting string \"%s\" into tokens:\n", str);

    token = strtok(str, " "); // Use space as the delimiter
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, " "); // Pass NULL to continue tokenizing the same string
    }

    return  0;
}
