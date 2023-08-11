#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../include/hashset.hpp"

extern "C"{
  #include "../include/linkedlist.h"
}
extern "C"{
  #include "linkedlist.c"
}

// the constructor for the hash set
HashSet::HashSet(size_t size) {
  this -> array = new LinkedList*[size];
  for (size_t i = 0; i < size; i++){
    this -> array[i] = NULL;
  }
  this -> count = 0;
  this -> size = size;
}

// the destructor for the hash set
HashSet::~HashSet() {
  for (size_t i = 0; i < size; i++){
    delete this -> array[i];
  }
  delete[] this -> array;
}

// generate a prehash for an item with a given size
unsigned long HashSet::prehash(int item) {
 unsigned char* bytes = new unsigned char[4]; // i used the given hash function but split the integer into its four bytes
  
  bytes[0] = (item >> 24) & 0xFF; // assigning each byte
  bytes[1] = (item >> 16) & 0xFF;
  bytes[2] = (item >> 8) & 0xFF;
  bytes[3] = item & 0xFF;

  unsigned long h = 5381;  //hash function given in gitlab
    int c;
    while (c = *bytes++) { 
	    h = ((h << 5) + h) + c;
    }
  return h;
}

// hash an unsigned long into an index that fits into a hash set
unsigned long HashSet::hash(int item) {
  unsigned long hashCode = prehash(item);
  unsigned long index = hashCode % this -> size;
  return index;
}

// resize the set if it is approaching the load capacity
void HashSet::resize(size_t newSize){
  LinkedList** old = this -> array;
  for (size_t i = 0; i < newSize; i++){
    arr[i] = NULL;
  } 
  for (size_t j = 0; j < size; j++){
    if (!(isEmpty(this -> array[j]))){
      Node* temp = this -> array[j] -> head;
      while (temp != nullptr && temp -> item != nullptr && temp != NULL && temp -> item != NULL){
        int* curr_ptr = (int*) temp -> item;
        int curr = *curr_ptr;
        unsigned long index = hash(curr);
        if (arr[index] == NULL){
          arr[index] = new LinkedList(); // initializing unused links
        }
        insertAtTail(arr[index], (void*)curr); // inserting in list 
        temp = temp -> next;
      }
      delete this-> array[j];
      this -> array[j] = nullptr;
    }
  }
  delete[] this -> array;
  this -> array = arr;
}

// insert an item in the set. return true if it was added, false if it already existed in the set.
bool HashSet::insert(int item) {
  unsigned long index = hash(item);
  if (this -> array[index] == NULL){
    this -> array[index] = new LinkedList(); // initializing unused links
  }
  if (containsNode(this -> array[index], (void*)item)){ // already exists in list
    r .  eturn false;
  }else{
    insertAtTail(this -> array[index], (void*)item); // inserting in list
    this -> count++;
    float load_factor = (float)this -> count / (float)this -> size; // calculating load factor
    if (load_factor > 0.8){
      resize();
    }
    return true;
  }
}

// remove an item from the set. return true if it was removed, false if it wasn't (i.e. it wasn't in the set to begin with)
bool HashSet::remove(int item) {
  unsigned long index = hash(item);
  if (this->array[index] == NULL){ // list is empty, item does not exist
    return false;
  }

  if (containsNode(this -> array[index], (void*)item)){ // item exists in the list
    this -> count--;
    int rmIndex = 0;
    Node* temp = this -> array[index] -> head;
    while (temp != NULL){
      if (temp -> item == (void*)item){
        removeAtIndex(this ->array[index], rmIndex); // removing item
        return true;
      }
      temp = temp -> next;
      rmIndex++;
    }
  }

  return false;
}

// return true if the item exists in the set, false otherwise
bool HashSet::contains(int item) { 
  unsigned long index = hash(item);  // hashed item
  if (isEmpty(this -> array[index])){ //does not exist in the set
    return false;
  }else{
    if (containsNode(this -> array[index], (void*)item)){ // does exist in the set
      return true;
    }
    return false;
  }
}

// returns the number of items in the hash set
size_t HashSet::len() {
  return this -> count;
}

// returns the number of empty buckets that can be filled before reallocating
size_t HashSet::capacity() {
  size_t empty_buckets = 0;

  for (size_t i = 0; i < this -> size; i++){
    if (this -> array[i] == NULL){
      empty_buckets++;
    }
  }
  return empty_buckets;
}

void HashSet::print() {
  printf("Hashset:\n");

  for(size_t i = 0; i < size; i++){
    printf("\nList %d: \n\n\t", i); 
    printList(this->array[i]);
  }
}



