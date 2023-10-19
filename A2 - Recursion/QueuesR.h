/* Interface: Header file for Queue functions
*  By: Michael Galle
*  Modified by: myStudentName
*/
#pragma once
const int iLength = 140;

typedef struct node Node;
typedef struct item Item;
typedef Node* link;

struct item {
	char message[iLength];
	short int sid = 3;			// Sender ID
	short int rid;			// Receiver ID
	char priority;			// Priority of message
	short int seqNum;		// Sequence number of a message
	char later[25];
};

struct node {
	link pNext;
	Item Data;
};

// Function Prototypes
void InitQueue(void);
int IsQueueEmpty(void);
void AddToQueue(link);
link DeQueue(void);
link returnHead();
void traverse(link h, void (*visit)(link));
void traverseR(link h, void (*visit)(link));
void visit(link h);
int count(link x);
link deleteR(link parent, link child, Item v);


