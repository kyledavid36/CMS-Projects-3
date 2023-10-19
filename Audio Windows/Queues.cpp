/* Queues.cpp - Implementation of Queue functions
 *
 *	Copyright 2019 Michael Galle
 */

#include "Queues.h"
#include <stdlib.h>

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