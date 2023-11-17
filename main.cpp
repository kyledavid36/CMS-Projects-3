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


void Sender(void* buffer, int size); //sending function
int Receiver(void* buffer, int size);


// Note: Comment out the Tx or Rx sections below to operate in single sided mode

// Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size

/******* User-Modifiable Values*******/

//Audio
int NSamples =  DEFAULT_NSAMPLES; 
int AudBufSize = MIN_BUFSIZE;
int RecordTime = RECORD_TIME;
int SamplesSec = SAMPLES_SEC;

//Text
int TextBufSize = BUFSIZE;

/*************************************/

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
	int Received = 0;																		//number of messages the receiving end is looking for.
	link p, q;																				//p = message, q is for dequeueing

	long  lBigBufSize = SAMPLES_SEC * RECORD_TIME;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));						// buffer used for reading recorded sound from file
	char* Text = (char*)malloc(BUFSIZE * sizeof(char));									// Text message buffer for NoQueues()
	//char *RText = (char*)malloc((BUFSIZE) * sizeof(char));							//Text message buffer for receiver for NoQueues()
	//TestAll();
	Received, numMessages = menu(TextBufSize, lBigBufSize, iBigBufNew, Text); //get number of messages first
	switch (numMessages)
	{
	case 1:
	{
		/*********************TRANSMITTTING END, NO QUEUES (kyle here)************************/

		printf("\nMessage Sent: %s\n\n", Text);
		Sender(Text, TextBufSize);
		
		break;
	}
	case 0:
	{
		/************************************RECEIVING END***********************************/
		if (Received == 1)
		{
			Receiver(Text, TextBufSize);
			printf("\nMessage Received: %s\n\n", Text);

			//bytesRead = inputFromPort(&hComRx, iBigBufNew, lBigBufSize);				// Receive audio from port
			//InitializePlayback();
			//PlayBuffer(iBigBufNew, lBigBufSize);
			//ClosePlayback();
		}
		else if (Received > 1)
		{

			p = (link)malloc(sizeof(Node));
			Text[Receiver(p->Data.message, TextBufSize)] = '\0';
			AddToQueue(p);
			//printf("Length of received msg = %d", bytesRead);
			while (!IsQueueEmpty()) {

				q = DeQueue();
				printf("\nMessage Received: %s\n\n", q->Data.message);					// Display message from port

				/*InitializePlayback();
				PlayBuffer(q->Data.audio, lBigBufSize);
				ClosePlayback();*/
				free(q);
			}
		}
		break;

	}



	default:
	{
		if (numMessages > 2)
		{
			/**********************************TRANSMITTING END**********************************/
			Received = numMessages;
			for (int i = 0; i < numMessages; i++)
			{
				messType = messageloop(); // get message type for each message needed by the user
				switch (messType)
				{

				case 'A':
					//allocate memory
					p = (link)malloc(sizeof(Node));
					//run "audio messages" function
					getAudioFromUser(lBigBufSize, p->Data.audio);

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

				default:
					break;
				}

			}
		}
		//DeQueue, SendMessage()

		printf("\nDEQUEUING...\n\n");
		while (!IsQueueEmpty()) {

			q = DeQueue();
			//SendMessage(q);
			// Transmit side 
			Sender(q->Data.message, TextBufSize);
			free(q);
		}

		break;
	}
	// Tear down both sides of the comm link
	purgePort(&hComRx);										// Purge the Rx port
	purgePort(&hComTx);										// Purge the Tx port
	CloseHandle(hComRx);									// Close the handle to Rx port 
	CloseHandle(hComTx);									// Close the handle to Tx port 


	free(iBigBufNew);
	free(Text);
	//free(RText);
	system("pause");

	return(0);
	}
}








void Sender(void *buffer, int size)
{
	initPort(&hComTx, COMPORT_Tx, nComRate, nComBits, timeout);						// Initialize the Tx port
	Sleep(500);
	outputToPort(&hComTx, buffer, size + 1);									// Send string to port - include space for '\0' termination
	Sleep(500);																		// Allow time for signal propagation on cable 
}

int Receiver(void *buffer, int size)
{
	initPort(&hComRx, COMPORT_Rx, nComRate, nComBits, timeout);					// Initialize the Rx port
	Sleep(500);
	DWORD bytesRead;
	bytesRead = inputFromPort(&hComRx, buffer, size + 1);							// Receive string from port
	return(bytesRead);
}