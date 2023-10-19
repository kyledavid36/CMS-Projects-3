/*	Filename: sound.h 
	Author: Michael Galle
	Date: Updated 2022
	Details: Interface - Contains prototypes for Windows sound API (sound recording and playback functions)
*/
#pragma once

#include <windows.h>					// Contains WAVEFORMATEX structure

// CONSTANTS - SET RECORDING TIME AND RATE
#define	DEFAULT_NSAMPLES	4000	
#define MIN_BUFSIZE			1000
#define RECORD_TIME			6		// seconds to record from mic
#define SAMPLES_SEC			8000	// number of samples per second
#define	NFREQUENCIES		96		// number of frequencies used - leave this alone


// FUNCTION PROTOTYPES
/* ************************ */
// Playback
int InitializePlayback(void);
int PlayBuffer(short *piBuf, long lSamples);
void ClosePlayback(void);

// Recording
int InitializeRecording(void);
int	RecordBuffer(short *piBuf, long lBufSize);
void CloseRecording(void);

// Support functions for Playback functions (updated 2021)
void SetupFormat(WAVEFORMATEX* wf);			// Used by InitializePlayback()					
int WaitOnHeader(WAVEHDR* wh, char cDit);	// Used by Playbuffer()





/***********************Queues.h**************************************/
const int iLength = 140;

typedef struct node Node;
typedef struct item Item;
typedef Node* link;

struct item {
	char message[iLength];
	short int sid;			// Sender ID
	short int rid;			// Receiver ID
	short audio;			// Audio buffer
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
