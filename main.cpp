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
int AudBufSize = MIN_BUFSIZE;
int SamplesSec = SAMPLES_SEC;

//Text
int *TextBufSize;

//Audio
int *RecordTime;
short AudioBuf[SAMPLES_SEC * RECORD_TIME];
long  AudioBufSize = SAMPLES_SEC * RECORD_TIME;	// total number of samples


int *menuchoice;
int* txrx;
/*************************************/

// Virtual Ports (via COM0COM) - uncomment to use (comment out the physical ports)
//wchar_t COMPORT_Rx[] = L"COM7";					// COM port used for Rx (use L"COM7" for transmit program)
//wchar_t COMPORT_Tx[] = L"COM8";					// COM port used for Tx (use L"COM8" for transmit program)

////Physical ports
//wchar_t COMPORT_Rx[5];						// Check device manager after plugging device in and change this port
//wchar_t COMPORT_Tx[5];						// Check device manager after plugging device in and change this port
LPCSTR COMPORT;
////wcsncpy(COMPORT_Tx, L"COM", 3);
//COMPORT_Tx[0] = L'C';


// --> If COM# is larger than 9 then use the following syntax--> "\\\\.\\COM10"

// Communication variables and parameters
HANDLE hCom;										// Pointer to the selected COM port (Receiver)
int nComRate = 230400;//9600*16;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable


Header txHeader;						// Header transmitted 
Header rxHeader;						// Header received

int com = 0; //Receiving com port



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

	menu(menuchoice, TextBufSize, RecordTime, &com, AudioBufSize, txrx, COMPORT, rxHeader, txHeader);

	



	return(0);

}






//
//
//void Sender(void *buffer, long int size)
//{
//	initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);						// Initialize the Tx port
//	Sleep(500);
//	outputToPort(&hComTx, buffer, size);											// Send string to port - include space for '\0' termination
//	Sleep(500);																		// Allow time for signal propagation on cable 
//}
//
//int Receiver(void *buffer, long int size)
//{
//	initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);						// Initialize the Rx port
//	Sleep(500);
//	DWORD bytesRead;
//	bytesRead = inputFromPort(&hComRx, buffer, size);								// Receive string from port
//	return(bytesRead);
//}