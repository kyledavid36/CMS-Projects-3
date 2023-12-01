/******************************************************************************

Name: menuactions.cpp
Purpose: All menu functions for the CMS Projects III
Author: Amy Wentzell


******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "message.h"
#include "sound.h"
#include "Queues.h"
#include "RS232Comm.h"
#include "compression.h"
#include "Header.h"


// Declare constants, variables and communication parameters
extern const int BUFSIZE;							// Buffer size
extern wchar_t COMPORT_Rx[];						// COM port used for Rx 
extern wchar_t COMPORT_Tx[];						// COM port used for Tx 
extern HANDLE hComRx;								// Pointer to the selected COM port (Receiver)
extern HANDLE hComTx;								// Pointer to the selected COM port (Transmitter)
extern int nComRate;								// Baud (Bit) rate in bits/second 
extern int nComBits;								// Number of bits per frame
extern COMMTIMEOUTS timeout;						// A commtimeout struct variable
extern Header txHeader;
extern Header rxHeader;


void myFlushAll()
{
	int c;
	do
	{
		c = getchar();

	} while (c != EOF && c != '\n');
}

/****************************		SETUP		*****************************/
void setup(int *menuchoice, int *TextBufSize, int *RecordTime, int* compointer, int *txrx)
{

	printf("Calibrating...\n");

	printf("\n\n	Are you transmitting or receiving? \n\n	Tx = 1\n	Rx = 0\n\nChoice:	");
	scanf_s("%d", txrx);
	myFlushAll();

	printf("\n\n	Please input the COM port: \n\n	COM0 = 0\n	COM1 = 1\n	COM2 = 2\n	COM3 = 3\n	COM4 = 4\n	COM5 = 5\n	COM6 = 6\n\nChoice:	");
	scanf_s("%d", compointer);
	myFlushAll();

	printf("\n\n	Please input the text buffer size (Max = 140, Min = 1): ");
	scanf_s("%d", TextBufSize);

	printf("\n\n	Please input the record time (Max = 10, Min = 1): ");
	scanf_s("%d", RecordTime);


	if (0 < *TextBufSize && *TextBufSize <= 140 && 0 < *RecordTime && *RecordTime <= 15 && *txrx >=0 && *txrx <= 1 )
	{
		*menuchoice = 0;
		
	}
	else
	{
		printf("\nERROR: invalid input.");
		
	}

}


/****************************		MAIN MENU		*****************************/
void mainMenu(int *menuchoice, char options[25][40], int* txrx)
{

	printf("\n\n\n\n\n\nCoded Messaging System\n	By: Amy Wentzell and Kyle Dick\n	Version: 2023\n\n\n");
	printf("\nWelcome! Choose your setting: \n\n");
	
	

	for (int i = 1; i < 20; i++)
	{
		printf("	(%d)	", i);
		for (int j = 0; j < 40; j++)
		{
				printf("%c", options[i - 1][j]);
		}
		printf("\n");
	}
	
	printf("Choice:\n");
	scanf_s("%d", menuchoice);
	myFlushAll();
	
}

/*************************************************		AUDIO FUNCTIONS		**********************************************/
void RecordAudio(long lBigBufSize, short* iBigBuf)
{

	printf("\n\n");
	printf("About to record...\n\n");
	Sleep(2000);
	InitializeRecording();
	// start recording
	RecordBuffer(iBigBuf, lBigBufSize);
	CloseRecording();

}

void PlaybackAudio(long lBigBufSize, short* iBigBuf)
{
	InitializePlayback();
	printf("\nPlaying recording from buffer\n");
	PlayBuffer(iBigBuf, lBigBufSize);
	ClosePlayback();
}

int SaveAudio(long lBigBufSize, short* iBigBuf)
{
	char replay;
	char c;																// used to flush extra input
	FILE* f;



	/* Open input file */
	fopen_s(&f, "recording.dat", "wb");
	if (!f) {
		printf("unable to open %s\n", "recording.dat");
		return (0);
	}
	printf("Writing to sound file ...\n");
	fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
	fclose(f);
	// replay audio recording from file -- read and store in buffer, then use playback() to play it
	printf("Would you like to replay the saved audio recording from the file? (y/n): ");
	scanf_s("%c", &replay, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	if ((replay == 'y') || (replay == 'Y')) {
		/* Open input file */
		fopen_s(&f, "recording.dat", "rb");
		if (!f) {
			printf("unable to open %s\n", "recording.dat");
			return 0;
		}
		printf("Reading from sound file ...\n");
		fread(iBigBuf, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
		fclose(f);
		InitializePlayback();
		printf("\nPlaying recording from saved file ...\n");
		PlayBuffer(iBigBuf, lBigBufSize);
		ClosePlayback();
	}

	printf("\n");
}

void CompressMessage(char *MessageType, char* inputfilename, unsigned char* in, long lBigBufSize, short *iBigBuf)
{
	if (*MessageType == 'A')
	{
		SaveAudio(lBigBufSize, iBigBuf);
		compression(inputfilename, in);
	}
	else
	{
		printf("\n	We do not like text messages here. See 'ENCRYPT MESSAGE' at the main menu.\n\n");
	}
}

int CommsTest(int* txrx, long int lBigBufSize, short int* audiomessage)
{


	if (*txrx == 1)
	{
		FILE* f;


		/* Open input file */
		fopen_s(&f, "Test.dat", "rb");
		if (!f) {
			printf("unable to open %s\n", "Test.dat");
			return (0);
		}
		for (int i = 0; i < lBigBufSize; i++)
		{
			*(audiomessage++) = fgetc(f);
		}
		fclose(f);
		printf("\n\nPress a key to transmit ...");
		getchar();
		NHtransmit(audiomessage, lBigBufSize * 2, &hComTx, nComRate, nComBits, timeout);

	}
	else
	{
		printf("\n\nPress a key to receive ...");
		getchar();
		NHreceive((void**)audiomessage, lBigBufSize, &hComRx, nComRate, nComBits, timeout);
		PlaybackAudio(lBigBufSize, audiomessage);
	}

}


/****************************************		TEXT FUNCTIONS		**********************************************/
void InputText(char* Message, int* TextBufSize)
{

	fflush(stdin); // Clear input buffer
	printf("Please enter a message:\n");
	scanf_s("%139[^\n]s", Message, *TextBufSize);
	//printf("%s", Message);

}


int TextSettings(int* TextBufSize)
{
	int pass1 = FALSE;
	int choice;


	do
	{
		printf("\nTEXT SETTINGS\n");
		printf("\nModify Buffer Size?\n	(NOTE: Default = 140, Max = 140, Min = 1, Input '0' to leave the settings as is.)\n");
		scanf_s("%d", &choice);
		if (choice == 0)
		{
			printf("\n\nReturning to Main Menu...\n");
			*TextBufSize = MAX_QUOTE_LENGTH;
			pass1 = TRUE;
		}
		else if (0 < choice && choice <= 140)
		{
			*TextBufSize = choice;
			printf("New Buffer Length is: %d", choice);

			pass1 = TRUE;

		}
		else
		{
			printf("\nERROR: invalid input. Please retry.\n");
		}
	} while (!pass1);
	return(*TextBufSize);
}

int QueuesTest(int NumQuotes, long int *Indices, int * LengthMessage, char *Message, const int BUFSIZE, int* txrx)
{	
	link p;
	int randnum;
	Header Queues; //QueuesTest Header Tx
	Header QueuesRx; 
	DWORD bytesRead;

	fquoteIndices(NumQuotes, Indices);
	fquoteLength(NumQuotes, Indices, LengthMessage);

	for (int i = 0; i < 3; i++)
	{
		if (*txrx == 1)
		{
			//Sleep(1000);
			randnum = frandNum(0, NumQuotes);
			printf("%d %d %d %d %d", NumQuotes, Indices[randnum-1], Indices[randnum], randnum, LengthMessage[randnum]);
			GetMessageFromFile(Message, 140, randnum, NumQuotes, Indices, LengthMessage);
			printf("Message Sent: %s\n\n", Message);
			
			Queues.sid = 1;
			Queues.rid = 0;
			Queues.payloadSize = BUFSIZE + 1;		// Number of bytes in payload after this header
			Queues.payLoadType = 0;			// 0: Text, 1: Audio, 2: Image etc.
			Queues.encryption = 0;			// 0: None, 1: XOR,	  2: Vigenere	3: Both
			Queues.compression = 0;			// 0: None, 1: RLE,	  2: Huffman,	3: Both
			
			
			printf("\n\nPress a key to transmit ...");
			getchar();
			transmit(&Queues, Message, &hComTx, nComRate, nComBits, timeout);

			//Sleep(3500);
		}
		else
		{
			p = (link)malloc(sizeof(Node));

			printf("\n\nPress a key to receive...");
			getchar();

			bytesRead = receive(&QueuesRx, &(p->Data.rxBuff), &hComRx, nComRate, nComBits, timeout);
			
			char* mesg = (char*)(p->Data.rxBuff);
			mesg[bytesRead] = '\0';
			//printf("\n\nRxHeader.payLoadtype is %d\n\n", Queues.payLoadType);
			printf("Message received: %s\n\n", p->Data.rxBuff);
			AddToQueue(p);
		}
	}

	if (*txrx == 0)
	{
		printf("\n	The number of nodes sent  is %d.\n", count(returnHead()));

		while (!IsQueueEmpty())
		{
			
			printf("MESSAGE:\n\n	%s\n\n", DeQueue()->Data.rxBuff);
		}
	}
	return(0);
}


void SendReceive(void *message, int headerOnOff, int *txrx, char *MessageType)
{
	
	switch (*txrx)
		{
		case 0:
			if (headerOnOff == 1)
			{
				printf("\n\nPress a key to receive...");
				getchar();
				receive(&rxHeader, (void**)message, &hComRx, nComRate, nComBits, timeout);
			}
			else
			{
				printf("\n\nPress a key to receive...");
				getchar();
				NHreceive((void**)message, sizeof(message), &hComRx, nComRate, nComBits, timeout);
			}
			break;
		case 1:
			switch (*MessageType)
				{
				case 'A':
					if (headerOnOff == 1)
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						transmit(&txHeader, (char*)message, &hComTx, nComRate, nComBits, timeout);
					}
					else
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						NHtransmit((char*)message, sizeof(message), &hComTx, nComRate, nComBits, timeout);
					}
					break;
				case 'T':
					if (headerOnOff == 1)
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						transmit(&txHeader, (short*)message, &hComTx, nComRate, nComBits, timeout);
					}
					else
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						NHtransmit((short*)message, sizeof(message), &hComTx, nComRate, nComBits, timeout);
					}
					break;
				default:
					break;
				}
			break;
		default:
			break;
		}
	
}

void AddMessageToQueue(link p, void *message)
{
	p->Data.rxBuff = message;
	AddToQueue(p);
}


//int messageloop()
//{
//	char messType = {NULL};
//	int pass = FALSE;
//	char c;
//	
//	do
//	{
//		printf("\nWhat type of message would you like to send? (Audio = A, Text = T):\n");
//		scanf_s(" %c", &messType, 1);
//		while ((c = getchar()) != '\n' && c != EOF) {}
//		if (messType == 'A' || messType == 'T')
//		{
//			pass = TRUE;
//		}
//		else
//		{
//			printf("\nERROR: invalid input. Please retry.\n");
//		}
//	}while (!pass);
//
//	return(messType);
//}







//void NoQueues(long lBigBufSize, short *iBigBuf, char *Message)
//{
//	int messagetype;
//
//
//	messagetype = messageloop();
//	if (messagetype == 'A')
//	{
//		getAudioFromUser(lBigBufSize, iBigBuf);
//	}
//	else if (messagetype == 'T')
//	{
//		getMessageFromUser(Message);
//	}
//	
//}




//void testAll()
//{
//	audioTest();
//	queuesTest();
//}


//int menu(int TextBufSize, long lBigBufSize, short *iBigBuf, char *Message)
//{
//	int pass = FALSE;
//	int amount = FALSE;
//	char x = NULL; //Tx or Rx
//
//	char c;
//	char setting = '0'; //where you are in the menu
//	char loc;
//	
//
//	char options[6][25] = { "Test Functionality","Audio Settings", "Text Settings", "Tx/Rx with Queues","Tx/Rx without Queues", "Exit" };
//
//	printf("Coded Messaging System\n	By: Amy Wentzell and Kyle Dick\n	Version: 2023\n\n\n");
//
//	do
//	{
//
//		switch (setting)
//		{
//
//		case '0':
//			printf("\nWelcome! Choose your setting: \n\n");
//			for (int i = 1; i < 7; i++)
//			{
//				printf("(%d)	", i);
//				for (int j = 0; j < 25; j++)
//				{
//					printf("%c", options[i - 1][j]);
//				}
//				printf("\n");
//			}
//			printf("Choice:\n");
//			scanf_s("%c", &loc, 1);
//			while ((c = getchar()) != '\n' && c != EOF) {}
//			setting = loc;
//			break;
//		case '1':
//			// TEST
//			testAll();
//			setting = '0';
//			break;
//		case '2':
//			//AudioSettings
//			setting = '0';
//			break;
//		case '3':
//			//Text Settings
//
//			TextBufSize = TextSettings(TextBufSize);
//			setting = '0';
//			printf("\n			Setting is %c, TextBufSize is %d.\n", setting, TextBufSize);
//			break;
//		case '4':
//			printf("Are you transmitting or receiving? (Transmitting = 1, Receiving = 0):\n");
//			scanf_s("%c", &x, 1);
//			while ((c = getchar()) != '\n' && c != EOF) {}
//			if (x == '1')
//			{
//				printf("How many messages would you like to send?(1 - 10):\n");
//				scanf_s("%d", &amount);
//				if (2 <= amount && amount <= 10)
//				{
//					
//					pass = TRUE;
//				}
//				else if (amount == 1)
//				{
//					NoQueues(lBigBufSize, iBigBuf, Message);
//					pass = TRUE;
//				}
//				else
//				{
//					printf("\nERROR: invalid input. Please retry.\n");
//				}
//			}
//			else if (x == '0')
//			{
//				
//				pass = TRUE;
//			}
//			else
//			{
//				printf("\nERROR: invalid input. Please retry.\n");
//			}
//			break;
//		case '5':
//			//Send one audio or text message
//			printf("Are you transmitting or receiving? (Transmitting = 1, Receiving = 0):\n");
//			scanf_s("%c", &x, 1);
//			while ((c = getchar()) != '\n' && c != EOF) {}
//			if (x == '1')
//			{
//				NoQueues(lBigBufSize, iBigBuf, Message);
//				//printf("%s sending message...\n", Message);
//				amount = 1;
//				pass = TRUE;
//			}
//			else if (x == '0')
//			{
//				amount = 0;
//				pass = TRUE;
//			}
//			else
//			{
//				printf("\nERROR: invalid input. Please retry.\n");
//			}
//			break;
//		case '6':
//			exit(0);
//			break;
//		default:
//			break;
//		}
//
//	} while (!pass);
//
//	return(amount);
//}