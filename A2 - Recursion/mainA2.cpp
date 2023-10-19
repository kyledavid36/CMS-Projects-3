/* main.cpp - Mainline for Queue functions
 *
 *	Copyright 2019 Michael Galle
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "QueuesR.h"
#include "Recursion.h"
#include "message.h"
#include <time.h>

int main() {
	/*
	// Part 1 - Introduction 
	//*************************************************************************
	int n = 5;
	printf("The sum of numbers 1 to %d is: %d\n", n, RecursiveAdd(n));
	printf("(%d)! (factorial) = %d\n", n, factorial(n));
	printf("Greatest common denominator gcd(1272, 216) = %d", gcd(1272, 216));

	printf("\n\n\n");

	*/

	// Part 2 - Recursion and Linked Lists 
	//*************************************************************************

	link p, q;
	int i;

	InitQueue();

	// Create a linked-list based queue
	for (i = 0; i < 15; i++) {
		p = (link)malloc(sizeof(Node));
		p->Data.sid = i;
		AddToQueue(p);
	}

	// Test the count() function on the linked list based queue
	printf("\nThe number of nodes currently in the queue is: %d", count(returnHead()));

	// Test the deleteR() function on the linked list based queue
//	Item v;
//	v.sid = 13;																// Node to look for in LL based queue
//	deleteR(returnHead(), returnHead()->pNext, v);										

	// Test the count() function on the linked list based queue
	printf("\nThe number of nodes in the queue after deleteR() is: %d", count(returnHead()));

	// Traverse list in forward order
	printf("\n\n***** Printing list in forward order - recursively *****");
	traverse(returnHead(), visit);
	
	// Traverse in Reverse order
	printf("\n\n***** Printing list in reverse order - recursively *****");
	traverseR(returnHead(), visit);

	printf("\n\n***** Print list forward and reverse: ****** \n");
	traverse(returnHead(), visit);
	traverseR(returnHead(), visit);

	
	
	// Empty the queue
	printf("\n\n***** Emptying Queue *****");
	while (!IsQueueEmpty()) {
		q = DeQueue();
		printf("\n SID of Node is: %d", q->Data.sid );
		free(q);
	}

	// Check that there are no more nodes in the queue
	printf("\n\nThe number of nodes currently in the queue is: %d\n\n", count(returnHead()));

	/*
	int NumQuotes = NULL;
	long int* Indexes;
	int* MessLength;
	srand(time(NULL));

	NumQuotes = fnumQuotes();
	int randomNumber = frandNum(0, NumQuotes - 1);

	printf("The number of quotes in the file is: %d.\n", NumQuotes);
	printf("A random number between 0 and the number of quotes in the file is %d.\n", randomNumber);


	Indexes = (long int*)malloc(NumQuotes * sizeof(long int));
	MessLength = (int*)malloc(NumQuotes * sizeof(int));
	Indexes = fquoteIndices(NumQuotes);
	MessLength = fquoteLength(NumQuotes, Indexes);
	
	char* buffer = (char*)malloc(MAX_QUOTE_LENGTH * sizeof(char));
	*/
	/*
	InitQueue();
	for (int i = 0; i < 10; i++) {
		p = (link)malloc(sizeof(Node));
		//p->Data.sid = i;
		int randomNumber = frandNum(0, NumQuotes - 1);
		GetMessageFromFile(p->Data.message, MessLength[randomNumber], randomNumber, NumQuotes, Indexes, MessLength);

		AddToQueue(p);
	}

	while (!IsQueueEmpty()) {
		q = DeQueue();
		printf("\n Quote is: %s\n", q->Data.message);
		free(q);
	}
	*/
	
	return(0);
}