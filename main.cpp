/******************************************************************************


Name: main.cpp
Purpose: Client interface for CMS Projects III
Authors: Amy Wentzell and Kyle Dick


******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "menuactions.h"
#include "message.h"
#include "Queues.h"
#include "sound.h"
#include "RS232Comm.h"

// Note: Comment out the Tx or Rx sections below to operate in single sided mode

// Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size

// Virtual Ports (via COM0COM) - uncomment to use (comment out the physical ports)
//wchar_t COMPORT_Rx[] = L"COM7";					// COM port used for Rx (use L"COM7" for transmit program)
//wchar_t COMPORT_Tx[] = L"COM8";					// COM port used for Tx (use L"COM8" for transmit program)

//Physical ports
wchar_t COMPORT_Rx[] = L"COM3";						// Check device manager after plugging device in and change this port
wchar_t COMPORT_Tx[] = L"COM5";						// Check device manager after plugging device in and change this port
// --> If COM# is larger than 9 then use the following syntax--> "\\\\.\\COM10"

// Communication variables and parameters
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable


#define MAX_NUM_MESSAGES 10


int main()
{
	
	int numMessages = FALSE;
	char messType = FALSE;

	link p, q; //p = message, q is for dequeueing

	long  lBigBufSize = SAMPLES_SEC * RECORD_TIME;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file


	//TestAll();
	numMessages = menu(); //get number of messages first

	if (numMessages > 0)
	{
		/**********************************TRANSMITTING END**********************************/
		
		for (int i = 0; i < numMessages; i++)
		{
			messType = messageloop(); // get message type for each message needed by the user
			switch (messType)
			{

			case 'A':
				//allocate memory
				p = (link)malloc(sizeof(Node));
				//run "audio messages" function
				getAudioFromUser(lBigBufSize, p->Data.audio, iBigBufNew);

				AddToQueue(p);


				//TEST CHAR READING
				//printf("%c", messType);

				break;
			case 'T':
				//allocate memory
				p = (link)malloc(sizeof(Node));
				//run "text messages" function
				getMessageFromUser(p->Data.message);


				//TEST string input
				//printf("\nYou entered a message: %s\n", p->Data.message);
				AddToQueue(p);
				//TEST CHAR READING
				//printf("%c", messType);
				break;
			}

		}
		//DeQueue, SendMessage()
		
		printf("\nDEQUEUING...\n\n");
		while (!IsQueueEmpty()) {

			q = DeQueue();
			//SendMessage(q);
			// Transmit side 
			/*initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);						// Initialize the Tx port
			Sleep(500);
			outputToPort(&hComTx, q->Data.message, strlen(q->Data.message) + 1);			// Send string to port - include space for '\0' termination
			Sleep(500);*/																		// Allow time for signal propagation on cable 
			printf("\n Quote is: %s\n", q->Data.message);
			//InitializePlayback();
			//PlayBuffer(q->Data.audio, /*lSamples, what is this for our buffer? */);
			//ClosePlayback();
			free(q);
		}
	}
	else
	{
		/************************************RECEIVING END***********************************/
		
		/*initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);				// Initialize the Rx port
		Sleep(500);
		p = (link)malloc(numMessages * sizeof(Node)* MAX_NUM_MESSAGES);
		DWORD bytesRead;
		bytesRead = inputFromPort(&hComRx, p->Data.message, BUFSIZE);			// Receive string from port
		//printf("Length of received msg = %d", bytesRead);
		p->Data.message[bytesRead] = '\0';
		printf("\nMessage Received: %s\n\n", p->Data.message);*/					// Display message from port
	
	}
	// Tear down both sides of the comm link
	/*purgePort(&hComRx);											// Purge the Rx port
	purgePort(&hComTx);											// Purge the Tx port
	CloseHandle(hComRx);										// Close the handle to Rx port 
	CloseHandle(hComTx);*/										// Close the handle to Tx port 

	//system("pause");

	return(0);
}