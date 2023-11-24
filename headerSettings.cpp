#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RS232Comm.h"
#include "Header.h"
#include "menuactions.h"
#include "sound.h"

// Declare constants, variables and communication parameters
extern const int BUFSIZE = 140;							// Buffer size
extern wchar_t COMPORT_Rx[];						// COM port used for Rx 
extern wchar_t COMPORT_Tx[];						// COM port used for Tx 
extern HANDLE hComRx;								// Pointer to the selected COM port (Receiver)
extern HANDLE hComTx;								// Pointer to the selected COM port (Transmitter)
extern int nComRate;								// Baud (Bit) rate in bits/second 
extern int nComBits;								// Number of bits per frame
extern COMMTIMEOUTS timeout;						// A commtimeout struct variable
extern short iBigBuf[SAMPLES_SEC * RECORD_TIME];
extern long  lBigBufSize;
extern long int TextBufSize;

int headerSettings()
{
	char selection;							// Tx or Rx (can run two instances of this program - double click the exe file)
	Header txHeader;						// Header transmitted 
	Header rxHeader;						// Header received
	void* rxPayload = NULL;					// Received payload (buffer) - void so it can be any data type
	DWORD bytesRead;
	int numMessages = FALSE;
	int c;
	int choice;
	int Received = 0;
	long  lBigBufSize = SAMPLES_SEC * RECORD_TIME;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));						// buffer used for reading recorded sound from file
	char* Text = (char*)malloc(BUFSIZE * sizeof(char));

	printf("\nDo you want to transmit using the header? YES(1) or NO(0): ");
	scanf_s("%d", &choice);
	//while ((c = getchar()) != '\n' && c != EOF) {}
	
	if (choice == 0)
	{
		
		Received, numMessages = menu(TextBufSize, lBigBufSize, iBigBufNew, Text);

		return(0);
	
	}
	else
	{
		printf("\nChoose the sender ID: ");
		scanf_s("%hi", &txHeader.sid); // phonebook
		//while (isalpha(txHeader.sid -'\0'))
		//{
		//	printf("\nYou did not enter a number.");
		//	printf("\nChoose the sender ID: ");
		//	scanf_s("%hi", &txHeader.sid);
		//}
		printf("\nChoose the receiver ID: ");
		scanf_s("%hi", &txHeader.rid);

		printf("\nChoose the type of compression: (0) None, (1) RLE or (2) Huffman: ");
		scanf_s("%hi", &txHeader.compression);

		printf("\nNo encryption for now:");
		txHeader.encryption = 0; //set to zero

		////printf("\nChoose the type of encription: (0) None, (1) XOR or (2) Vingere ");
		////scanf_s("%u", &txHeader.encryption, 1);

		printf("\n\nChoose the payload type: (1) Audio, (2) Text: ");
		scanf_s("%hi", txHeader.payLoadType);
		printf("\n\n");
		

		//// PayloadType is Audio
		//if (txHeader.payLoadType == 1)
		//{
		//	// initialize recording
		//	InitializeRecording();
		//	// start recording
		//	RecordBuffer(iBigBuf, lBigBufSize);
		//	CloseRecording();

		//	transmit(&txHeader, (char*)iBigBuf, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);

		//}
		//else if (txHeader.payLoadType == 2)
		//{
		//	char txPayload[] = "\nHi there this is a great message for you\n";

		//	printf("\nChoose the pay load: ");

		//	getMessageFromUser(txPayload);

		//	transmit(&txHeader, txPayload, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);

		//}
		//else
		//{
		//	printf("ERROR, please enter (1) or (2)");
		//}
		

	
	}
	
	return(0);


}