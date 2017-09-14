/*
Author: Fernando Cuevas
Class: CPSC 351
Purpose: Create a queue in c.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct q_node qu_node;
typedef struct queue q;

struct q_node{
  void* data;
  qu_node* next;
};

struct queue{
  q* head;
  size_t queueSize;
};

/*Functions for queue*/

qu_node* newNode(void* item);

q *createQueue();

void q_Push(q* qu, void* newData);

bool isQEmpty(q* check);

void displayQueue(q* qu);

/*end of functions for queue*/

int main(){

  printf("C impilentation of a queue\n");
  printf("Adding  number 1\n");

  q* cQueue = createQueue();

  q_Push(cQueue, 1);
  displayQueue(cQueue);

  return 0;
}

/* function implementation */

qu_node* newNode(void* item){ /* create a new node in the q*/
  qu_node *temp_node = (qu_node*)malloc(sizeof(qu_node));
  temp_node->data = item;
  temp_node->next = NULL;
  return temp_node;
} /*allocate memory and create a new node with the data and habe
    it point to null
  */

q *createQueue(){
  q *newQueue = (q*)malloc(sizeof(q));
  newQueue->head = NULL;
  newQueue->queueSize = 0;
  return newQueue;
}/* initalize the head and queue */


void q_Push(q* qu, void* newData){
  /*check if empyt*/
  qu_node *node = newNode(newData);

  qu->head = newNode;
  qu->queueSize += 1;

} /*push or 'enqueueu' data */


bool isQEmpty(q* check){
  return check->queueSize >= 0;
}


void displayQueue(q* qu){
  /*check first if empty*/
  qu_node *temp = qu->head;

  printf("%p ---> NUll\n", temp->data);
}
