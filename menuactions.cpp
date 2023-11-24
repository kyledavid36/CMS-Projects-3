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
#include "audioTest.h"
#include "queuesTest.h"
#include "Header.h"

void myFlushAll()
{
	int c;
	do
	{
		c = getchar();

	} while (c != EOF && c != '\n');
}

void setup(int *menuchoice, int *TextBufSize, int *RecordTime, int* com1, int* com2, int *txrx)
{

	printf("Calibrating...\n");

	printf("\n\n	Are you transmitting or receiving? \n\n	Tx = 1\n	Rx = 0\n\nChoice:	");
	scanf_s("%d", txrx);
	myFlushAll();

	printf("\n\n	Please input the transmitter COM port: \n\n	COM0 = 0\n	COM1 = 1\n	COM2 = 2\n	COM3 = 3\n	COM4 = 4\n	COM5 = 5\n	COM6 = 6\n\nChoice:	");
	wscanf_s(L"%d", com1);
	myFlushAll();

	printf("\n\n	Please input the receiver COM port: \n\n	COM0 = 0\n	COM1 = 1\n	COM2 = 2\n	COM3 = 3\n	COM4 = 4\n	COM5 = 5\n	COM6 = 6\n\nChoice:	");
	wscanf_s(L"%d", com2);
	myFlushAll();

	printf("\n\n	Please input the text buffer size (Max = 140, Min = 1): ");
	scanf_s("%d", TextBufSize);

	printf("\n\n	Please input the record time (Max = 10, Min = 1): ");
	scanf_s("%d", RecordTime);


	if (0 < *TextBufSize && *TextBufSize <= 140 && 0 < *RecordTime && *RecordTime <= 10 && *txrx >=0 && *txrx <= 1 )
	{
		*menuchoice = 0;
		
	}
	else
	{
		printf("\nERROR: invalid input.");
		
	}

}



void mainMenu(int *menuchoice, char options[25][40], int* txrx)
{

	printf("\n\n\n\n\n\nCoded Messaging System\n	By: Amy Wentzell and Kyle Dick\n	Version: 2023\n\n\n");
	printf("\nWelcome! Choose your setting: \n\n");
	
	

	for (int i = 1; i < 25; i++)
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

void QueuesTest(const int BUFSIZE, int* txrx, HANDLE* hComTx, HANDLE* hComRx, wchar_t* COMPORT1, wchar_t* COMPORT2, int nComRate, int nComBits, COMMTIMEOUTS timeout, link p)
{
	int randnum;
	char* buffer;
	Header Queues; //QueuesTest Header


	Queues.sid = 1;
	Queues.rid = 0;
	Queues.payloadSize = BUFSIZE;		// Number of bytes in payload after this header
	Queues.payLoadType = 0;			// 0: Text, 1: Audio, 2: Image etc.
	Queues.encryption = 0;			// 0: None, 1: XOR,	  2: Vigenere	3: Both
	Queues.compression = 0;			// 0: None, 1: RLE,	  2: Huffman,	3: Both






	for (int i = 0; i < 10; i++)
	{
		if (*txrx == 1)
		{
			
			randnum = frandNum(0, fnumQuotes());
			GetMessageFromFile(buffer, 140, randnum, fnumQuotes(), fquoteIndices(fnumQuotes()), fquoteLength(fnumQuotes(), fquoteIndices(fnumQuotes())));
			transmit(&Queues, buffer, hComTx, COMPORT1, nComRate, nComBits, timeout);
			Sleep(4500);
		}
		else
		{
			p = (link)malloc(sizeof(Node));
			receive(&Queues, (void**)p->Data.message, hComRx, COMPORT2, nComRate, nComBits, timeout);
			AddToQueue(p);
		}
	}


}


void ShowQueues(link q)
{
	
	printf("\n	The number of nodes sent  is %d.",count(returnHead()));
	//More to come lol

	while (!IsQueueEmpty)
	{
		q = DeQueue();
		printf("MESSAGE:\n\n	%s\n", q->Data.message);
		free(q);
	}
}



void InputText(char *Message, int *TextBufSize)
{

	fflush(stdin); // Clear input buffer
	printf("Please enter a message:\n");
	scanf_s("%139[^\n]s", Message, *TextBufSize);
	//printf("%s", Message);

}




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




int TextSettings(int *TextBufSize)
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


int messageloop()
{
	char messType = {NULL};
	int pass = FALSE;
	char c;
	
	do
	{
		printf("\nWhat type of message would you like to send? (Audio = A, Text = T):\n");
		scanf_s(" %c", &messType, 1);
		while ((c = getchar()) != '\n' && c != EOF) {}
		if (messType == 'A' || messType == 'T')
		{
			pass = TRUE;
		}
		else
		{
			printf("\nERROR: invalid input. Please retry.\n");
		}
	}while (!pass);

	return(messType);
}







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