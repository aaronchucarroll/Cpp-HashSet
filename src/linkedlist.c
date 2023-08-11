#include <stdio.h>
#include <stdlib.h>
#include "../include/linkedlist.h"
#include <stdbool.h>

void printList(LinkedList* list) {
	Node* node;

  	// Handle an empty node. Just print a message.
	if(list->head == NULL) { 
		printf("\nEmpty List");
		return;
	}
	
  	// Start with the head.
	node = (Node*) list->head;

	printf("\nList: \n\n\t"); 
	while(node != NULL) {
		printf("[ %x ]", node->item);

		// Move to the next node
		node = (Node*) node->next;

		if(node !=NULL) {
			printf("-->");
		}
	}
	printf("\n\n");
}

// Initialize an empty list
void initList(LinkedList** list_pointer){
	(*list_pointer) -> head = NULL;
	(*list_pointer) -> tail = NULL;
}

// Create node containing item, return reference of it.
Node* createNode(void* item){
	Node* node = (Node*)malloc(sizeof(Node));
	node -> item = item;
	node -> next = NULL;
	return node;
}

// Insert new item at the end of list.
int insertAtTail(LinkedList* list_pointer, void* item){
	Node* newNode = createNode(item);
	if (isEmpty(list_pointer) == 1){ // special case for empty list
		list_pointer -> head = newNode;
		list_pointer -> tail = newNode;
	}else{ //creates a pointer to the current tail. Then replaces the tail with a new node.
		Node* lastNode = list_pointer -> tail;
		lastNode -> next = newNode;
		list_pointer -> tail = newNode;
	}

	return 0;
}

// Insert item at start of the list.
int insertAtHead(LinkedList* list_pointer, void* item){
	Node* newNode = createNode(item);
	if (isEmpty(list_pointer) == 1){ // special case for empty list
		list_pointer -> head = newNode;
		list_pointer -> tail = newNode;
	}else{ // creates a new node whose next item points at the current head. Then replaces the head with the new node.
		newNode -> next = list_pointer -> head; 
		list_pointer -> head = newNode;
	}

	return 0;
}

// Insert item at a specified index.
int insertAtIndex(LinkedList* list_pointer, int index, void* item){
	if (index > size(list_pointer) || index < 0){ // special case for invalid index
		exit(99);
	}
	if (index  == 0){ //special case for inserting at head
		insertAtHead(list_pointer, item);
		return 0;
	}
	if (index == size(list_pointer)){ // special case for inserting at tail
		insertAtTail(list_pointer, item);
		return 0;
	}
	Node* newNode = createNode(item);

	Node* prev = (Node*)nodeAtIndex(list_pointer, index - 1); // points to the node before the desired node
	Node* nextNode = NULL;
	if (prev != NULL){ // required check that the previous node exists
		nextNode = (Node*)prev -> next; // points to the node after the desired node
		prev -> next = newNode;
		newNode -> next = nextNode;
	}
	return 0;
}

// Remove item from the end of list and return a reference to it
void* removeTail(LinkedList* list_pointer){
	if (size(list_pointer) < 2){ // special case for too small list
		free(list_pointer -> head);
	}
	Node* penUltNode = nodeAtIndex(list_pointer, size(list_pointer) - 2);
	Node* temp = list_pointer -> tail;
	list_pointer -> tail = penUltNode;
	penUltNode -> next = NULL;
	void* val = temp -> item;
	free(temp);
	return val;
}

// Remove item from start of list and return a reference to it
void* removeHead(LinkedList* list_pointer){
	Node* secondNode = nodeAtIndex(list_pointer, 1);
	Node* temp = list_pointer -> head;
	void*val = temp -> item;
	list_pointer -> head = secondNode;
	free(temp);
	return val;
}

// Insert item at a specified index and return a reference to it
void* removeAtIndex(LinkedList* list_pointer, int index){
	Node* temp = nodeAtIndex(list_pointer, index);
	Node* prev = nodeAtIndex(list_pointer, index-1);
	Node* next = nodeAtIndex(list_pointer, index+1);
	prev -> next = next; // the previous item now points at the next item, skipping the removed item
	void* val = temp -> item;
	free(temp);
	return val;
}

// Return item at index
void* itemAtIndex(LinkedList* list_pointer, int index){
	return (void*)nodeAtIndex(list_pointer, index) -> item; // returns the item belonging to the node at the given index
}

// Return true if the list contains the given item at least once, false otherwise.
bool containsNode(LinkedList* list_pointer, void* item){
	if (isEmpty(list_pointer) == 1){
		return false;
	}

	Node* temp = list_pointer -> head;
	
	if (temp -> item == item){ // special case for the item existing at the 0th index
		return true;
	}
	
	while (temp != NULL){ // iterates over the list, returns 1 if the item matches at any node
		if (temp -> item == item){
			return true;
		}
		temp = temp -> next;
	}
	return 0;
}


//very useful method that returns the node at the specified index
Node* nodeAtIndex(LinkedList* list_pointer, int index){
	if (index >= size(list_pointer)){
		return NULL;
	}
	Node* temp = (Node*)list_pointer -> head;
	int i = 0;
	while (i < index && temp->next != NULL){
		temp = temp -> next;
		i++;
	}

	return (Node*)temp;
}



// Returns the size of the list, measured in nodes.
int size(LinkedList* list_pointer){
	int size = 0;
	if (isEmpty(list_pointer) == 1){
		return size;
	}
	Node* temp = list_pointer -> head;
	while (temp != NULL){
		temp = temp -> next;
		size++;
	}
	return size;
}

//easy check for empty list
bool isEmpty(LinkedList* list_pointer){
	if (list_pointer -> head == NULL){
		return true;
	}
	if (list_pointer -> head -> item == NULL){
		return true;
	}
	return false;
}


//frees the list. I use this after printing the list so that i avoid valgrind errors
void freeList(LinkedList* list_pointer){
	Node* temp = (Node*) list_pointer -> head;
	while (temp != NULL){
		Node* toFree = temp;
		temp = temp -> next;
		free(toFree);
	}
}
