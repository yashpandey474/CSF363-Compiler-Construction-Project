#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<syntactical.h>

// Struct for the array of linked lists
struct LinkedListArray {
    struct Node* head;
};

//structure of Linked list node
struct Node
{
  struct Variable data;
  struct Node *next;
};
// Function to create a new node with the given data
struct Node* createNode(struct Variable data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}
// Function to create a singly linked list with the given elements
struct Node* createLinkedList(struct Variable* elements, int size) {
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
// Function to insert a new node at the beginning of the linked list
void insertAtBeginning(struct Node** headRef, struct Variable newData) {
    // Create a new node
    struct Node* newNode = createNode(newData);

    // Set the next pointer of the new node to point to the current head
    newNode->next = *headRef;

    // Update the head to point to the new node
    *headRef = newNode;
}
// Function to print all elements of the linked list
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("(val: %d, flag: %d) ", current->data.val, current->data.flag);
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
