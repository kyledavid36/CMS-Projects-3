/******************************************************************************


Name: main.cpp
Purpose: Client interface for CMS Projects III
Authors: Amy Wentzell and Kyle Dick


******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <cwchar>
#include "menu.h"
#include "menuactions.h"
#include "sound.h"
#include "Header.h"
#include "message.h"
#include "compression.h"

//
//void Sender(void* buffer, long int size); //sending function
//int Receiver(void* buffer, long int size);
//
//
//// Note: Comment out the Tx or Rx sections below to operate in single sided mode
//
//// Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size
//
///******* User-Modifiable Values*******/
//
////Audio
int NSamples =  DEFAULT_NSAMPLES; 
int AudBufSize = 15 * SAMPLES_SEC;
int SamplesSec = SAMPLES_SEC;

long lBigBufSize;
//Text
int *TextBufSize;

//Audio
int *RecordTime;
short iBigBuf;


int *menuchoice;
int* txrx;
/*************************************/

// Virtual Ports (via COM0COM) - uncomment to use (comment out the physical ports)
//wchar_t COMPORT_Rx[] = L"COM7";					// COM port used for Rx (use L"COM7" for transmit program)
//wchar_t COMPORT_Tx[] = L"COM8";					// COM port used for Tx (use L"COM8" for transmit program)

////Physical ports
wchar_t COMPORT_Rx[5] = L"COM4";						// Check device manager after plugging device in and change this port
wchar_t COMPORT_Tx[5] = L"COM6";						// Check device manager after plugging device in and change this port
////wcsncpy(COMPORT_Tx, L"COM", 3);
//COMPORT_Tx[0] = L'C';


// --> If COM# is larger than 9 then use the following syntax--> "\\\\.\\COM10"

// Communication variables and parameters
HANDLE hCom;										// Pointer to the selected COM port (Receiver)
int nComRate = 460800;//9600*16;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable

DWORD bytesRead;

Header txHeader;						// Header transmitted 
Header rxHeader;						// Header received


int NumQuotes = fnumQuotes();

/************		AUDIO FILE		 ************/
char inputfilename[30] = "recording.dat";



#define MAX_NUM_MESSAGES 10




int main()
{
	

	int TBuff = BUFSIZE; //Default Text Buffer Size
	int RTime = RECORD_TIME; //Default Record Time

	int menuChoice = 'S';
	int transmit = 1;

	TextBufSize = &TBuff;
	RecordTime = &RTime;
	menuchoice = &menuChoice;
	txrx = &transmit;

	/*******************	BUFFERS ****************************/
	void* message = (void*)malloc(AudBufSize * sizeof(short));

	/***********	MESSAGE.CPP REQUIRED MALLOC	***************/
	
	long int* Indices = (long int*)malloc(NumQuotes * sizeof(long int));
	int* LengthMessage = (int*)malloc(NumQuotes * sizeof(int));



	setup(menuchoice, RecordTime);

	lBigBufSize = SAMPLES_SEC * (*RecordTime);
	menu(message, Indices, LengthMessage, menuchoice, RecordTime, rxHeader, txHeader);

	free(message);
	free(Indices);
	free(LengthMessage);



	return(0);

}