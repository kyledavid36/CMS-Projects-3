/* Queues.cpp - Implementation of Queue functions
 *
 *	Copyright 2019 Michael Galle
 */

#include "QueuesR.h"
#include <stdlib.h>
#include <stdio.h>

static link pHead, pTail;		// Private pointers to head and tail of queue

void InitQueue(void) {			// Initialize the queue 
	pHead = pTail = NULL;
}

int IsQueueEmpty(void) {		// Check if queue is empty - empty if pHead is NULL
	return(pHead == NULL);
}

void AddToQueue(link pn) {			// Enqueue new node provided and instantiated in main()
	if (IsQueueEmpty()) {
		pHead = pTail = pn;			// pHead only stored once for first node added
	}
	else {
		pTail->pNext = pn;			// Old tail points to new Node added
		pTail = pn;					// Make pn the new tail
	}
	pTail->pNext = NULL;			// Set pointer from last node to NULL - indicates end of Queue
}

link DeQueue(void) {
	link pTemp;						// Holds current Head
	if (IsQueueEmpty()) {
		return NULL;
	}
	else {
		pTemp = pHead;				// Store current head
		pHead = pHead->pNext;		// Next Node becomes new head
		return (pTemp);				// Returns original head
	}
}

link returnHead()
{
	return pHead;

}

int count(link x) {
	if (x == NULL) return(0);
	return(1 + count(x->pNext));
}

link deleteR(link parent, link child, Item v) {
	if (child == NULL) return(NULL);
	if (child->Data.sid == v.sid) {
		parent->pNext = child->pNext;
		free(child);
		deleteR(parent, parent->pNext, v);
	}
	else {
		deleteR(child, child->pNext, v);
	}

}

void traverse(link h, void(*visit)(link)) {
	if (h == NULL) return;
	(*visit)(h);  // calls ‘visit( )’ before recursive call
	traverse(h->pNext, visit);
}

// Execute a function on each node in reverse
void traverseR(link h, void (*visit)(link)) {
	if (h == NULL) return;
	traverseR(h->pNext, visit);
	(*visit)(h); // only called after last node is reached 
}

void visit(link h)
{
	printf("\nSID of node: %d\n", h->Data.sid);

}