/* main.cpp - Mainline for Queue functions
 *
 *	Copyright 2019 Michael Galle
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "message.h"
#include "QueuesR.h"
#include <time.h>

#define ONE 1
#define N 3



int main() {
	/*
	int part = NULL;
	int count = NULL; //amount of messages
	int* Count; 
	Count = &count; 

	int MessageNum = NULL; //message number in the list (text file)
	int* MessNum;
	MessNum = &MessageNum; //MessNum is a pointer so we can call it in multiple files without the need for local definitions

	//int iLen = 140;

	int* Part;     //this will determine which part of the lab we are in at the start
	Part = &part;
	do
	{
		printf("Assignment 1: Part 1, or Part 2? (P1 = 1, P2 = 2):\n"); //Asks user for which part of the lab
		scanf_s("%d", &part);
	} while (part != 1 && part != 2);
	printf("%d", *Part);

	if (*Part == 1)
	{
		//*Count = ONE;
	}
	else
	{
		//*Count = N;
	}
	*/

	link p, q;
	//int i;
	int NumQuotes = NULL;
	long int* Indexes;
	int* MessLength;
	srand(time(NULL));

	NumQuotes = fnumQuotes();
	int randomNumber = frandNum(0, NumQuotes - 1);

	printf("The number of quotes in the file is: %d.\n", NumQuotes);
	printf("A random number between 0 and the number of quotes in the file is %d.\n",  randomNumber );

	
	Indexes = (long int*)malloc(NumQuotes * sizeof(long int));
	MessLength = (int*)malloc(NumQuotes * sizeof(int));
	Indexes = fquoteIndices(NumQuotes);
	MessLength = fquoteLength(NumQuotes, Indexes);
	
	
	// Testing
	//for (i = 0; i < NumQuotes; i++)
	//{
	//printf("%d\n", Indexes[i]);
	//}
	


	
	//printf("Quote %d: ", 100);
	//printf("%d\n", MessLength[]);

	char* buffer = (char*)malloc(MAX_QUOTE_LENGTH * sizeof(char));
	//GetMessageFromFile(buffer, MessLength[randomNumber], randomNumber, NumQuotes, Indexes, MessLength);

	//printf("MESSAGE: %s", buffer);



	/* **************************** TEST Queues Functions ***************************************** */

	/* 
	InitQueue();
	for (int i = 0; i < 10; i++) {
		p = (link)malloc(sizeof(Node));
		p->Data.sid = i;
		AddToQueue(p);
	}

	while (!IsQueueEmpty()) {
		q = DeQueue();
		printf("\n SID of Node is: %d", q->Data.sid );
		free(q);
	}
	
	*/


	// Test getmessage

	InitQueue();
	for (int i = 0; i < 10; i++) {
		p = (link)malloc(sizeof(Node));
		//p->Data.sid = i;
		randomNumber = frandNum(0, NumQuotes - 1);
		GetMessageFromFile(p->Data.message, MessLength[randomNumber], randomNumber, NumQuotes, Indexes, MessLength);

		AddToQueue(p);
		//free(p);

	}

	while (!IsQueueEmpty()) {
		q = DeQueue();
		printf("\n Quote is: %s\n", q->Data.message);
		free(q);
	}
	
	printf("\nAbout to print Traverse:\n");
	
	traverse(returnHead(),visit);

	traverseR(returnHead(), visit);




	return(0);
}