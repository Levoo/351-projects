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

struct q_node {
	void* data;
	qu_node *next;
};

struct queue {
	q *head;
	size_t queueSize;
};

/*Functions for queue*/

qu_node* newNode(void* item);

q* createQueue();

void q_Push(q* qu, void* newData);

void q_Pop(q* qu);

void q_removeAll(q* qu);

void q_delete(q* qu);

bool isQEmpty(q* check);

size_t qSize(q* check);

void displayQueue(q* qu);

void* front(q* qu);

void* back(q* qu);

/*end of functions for queue*/

int main() {

	printf("C impilentation of a queue\n");

	q* cQueue = createQueue(); // create an empty queue

	q_Push(cQueue, 5);		//push some values 
	q_Push(cQueue, 2);
	displayQueue(cQueue);  // will display 2 then 5

	front(cQueue);			// print the first in list in this case 2
	back(cQueue);			// "" 5, usually wont change unless we pop everything.
	
	q_Push(cQueue, 10);     //
	front(cQueue);
	back(cQueue);
	displayQueue(cQueue);
	
	q_Pop(cQueue);

	front(cQueue);
	back(cQueue);
	displayQueue(cQueue);

	q_Push(cQueue, 15);
	front(cQueue);
	back(cQueue);
	displayQueue(cQueue);

	q_removeAll(cQueue);  //removes all the nodes plus the queue itself...

	return 0;
}

/* function implementation */

qu_node* newNode(void* item) { /* create a new node in the q*/
	qu_node *temp_node = (qu_node*)malloc(sizeof(qu_node));
	temp_node->data = item;
	temp_node->next = NULL;
	return temp_node;
} /*allocate memory and create a new node with the data and habe
  it point to null
  */

q *createQueue() {
	q *newQueue = (q*)malloc(sizeof(q));  //allocat mem and then set values
	newQueue->head = NULL;
	newQueue->queueSize = 0;
	return newQueue;
}/* initalize the head and queue */

void q_Push(q* qu, void* newData) {
	/*check if empyt*/
	qu_node *node = newNode(newData);
	
	if (qu == NULL) {			//if null e.i empty then just have head point ot it and done 
		qu->head = node;
		qu->queueSize += 1;
	}

	node->next = qu->head; //else have the new node point to what head was pointing to and then have head 
	qu->head = node;	// point to the node
	qu->queueSize += 1;  //make sure to increase size of the queue

} /*push or 'enqueueu' data */

void q_Pop(q* qu) {
	if (!isQEmpty) {
		printf("\nError! Queue is empty!\n");
	}

	qu_node *temp = qu->head;        //have a temp for the head, and then chaneg that
	qu_node *popNode = qu->head;     // since pop is popping off the top node we can just leave pop node = head

	qu->head = temp->next;  //this is that change, make head point to the next one so head -> 5 -> 6 -> 7
							// will turn into head -> 6 ->7 that leaves only pop node pointing to the top node
							//letting us free the mem

	printf("\nPopping %d\n", popNode->data);

	free(popNode);

}

void q_removeAll(q* qu) {
	printf("\nWARNING!\n Remove all invoked deleteing all queue nodes and the queue itself \n");
	qu_node *temp = qu->head;

	while (temp != NULL) {  //go through each node delete and update temp since pop changes the head
		q_Pop(qu);
		temp = qu->head;
	}

	q_delete(qu);
}

void q_delete(q* qu) {
	free(qu);  //once all hope is lost you end it all
	printf("\n queue removal was successful!\n");
}

void* front(q* qu) {
	
	if (!isQEmpty(qu)) {
		printf("Error! Queue is empty");
		printf("\nCurrent Queue size is %d\n", qSize(qu));
		return;
	}

	qu_node *temp = qu->head;
	printf("\nFront of queue is: %d\n", temp->data);

}

void* back(q* qu) {
	qu_node *temp = qu->head;

	if (!isQEmpty(qu)) printf("\nError, queue is empty!\n");

	while (temp->next != NULL) {  //go through quueue until you are at the end 
		temp = temp->next;
	}

	printf("Back of queue is: %d\n", temp->data);
}

bool isQEmpty(q* check) {
	return qSize(check) > 0;
}

size_t qSize(q* check) {
	return check->queueSize;
}

void displayQueue(q* qu) {
	/*check first if empty*/
	qu_node *temp = qu->head;
	printf("queue: <front> [ ");
	while (temp->next != NULL) {  //go through each element until the end 
		printf(" %d --> ", temp->data);
		temp = temp->next;
	}

	printf("%d ---> NUll ] <back>\n", temp->data);
}
