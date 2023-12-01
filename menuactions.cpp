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
#include "rle.c"
#include "huffman.h"
#include "encrypt.h"


// Declare constants, variables and communication parameters
extern const int BUFSIZE;							// Buffer size
//extern wchar_t COMPORT_Rx[];						// COM port used for Rx 
//extern wchar_t COMPORT_Tx[];						// COM port used for Tx 
//extern LPCSTR COMPORT;
extern HANDLE hCom;								// Pointer to the selected COM port (Receiver)
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
void mainMenu(int *menuchoice, char options[16][40], int* txrx)
{

	printf("\n\n\n\n\n\nCoded Messaging System\n	By: Amy Wentzell and Kyle Dick\n	Version: 2023\n\n\n");
	printf("\nWelcome! Choose your setting: \n\n");
	
	

	for (int i = 1; i < 17; i++)
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

void CompressMessage(char *MessageType, void* message, long lBigBufSize)
{
	if (*MessageType == 'A')
	{
		SaveAudio(lBigBufSize, (short*)message);
		compression(message, lBigBufSize);
	}
	else
	{
		printf("\n	We do not like text messages here. See 'ENCRYPT MESSAGE' at the main menu.\n\n");
	}
}

int CommsTest(int* txrx, long int lBigBufSize, short int* audiomessage, LPCSTR COMPORT)
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
		printf("\nThe COMPORT is: %s.\n", COMPORT);
		printf("\n\nPress a key to transmit ...");
		getchar();
		
		NHtransmit(audiomessage, lBigBufSize * 2, &hCom, nComRate, nComBits, timeout, COMPORT);

	}
	else
	{
		printf("\n\nPress a key to receive ...");
		getchar();
		NHreceive((void**)audiomessage, lBigBufSize, &hCom, nComRate, nComBits, timeout, COMPORT);
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

int QueuesTest(int NumQuotes, long int *Indices, int * LengthMessage, char *Message, const int BUFSIZE, int* txrx, LPCSTR COMPORT)
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
			transmit(&Queues, Message, &hCom, nComRate, nComBits, timeout, COMPORT);

			//Sleep(3500);
		}
		else
		{
			p = (link)malloc(sizeof(Node));

			printf("\n\nPress a key to receive...");
			getchar();

			bytesRead = receive(&QueuesRx, &(p->Data.rxBuff), &hCom, nComRate, nComBits, timeout, COMPORT);
			
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


void SendReceive(void *message, int headerOnOff, int *txrx, char *MessageType, LPCSTR COMPORT)
{
	
	switch (*txrx)
		{
		case 0:
			if (headerOnOff == 1)
			{
				printf("\n\nPress a key to receive...");
				getchar();
				receive(&rxHeader, (void**)message, &hCom, nComRate, nComBits, timeout, COMPORT);
			}
			else
			{
				printf("\n\nPress a key to receive...");
				getchar();
				NHreceive((void**)message, sizeof(message), &hCom, nComRate, nComBits, timeout, COMPORT);
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
						transmit(&txHeader, (char*)message, &hCom, nComRate, nComBits, timeout, COMPORT);
					}
					else
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						NHtransmit((char*)message, sizeof(message), &hCom, nComRate, nComBits, timeout, COMPORT);
					}
					break;
				case 'T':
					if (headerOnOff == 1)
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						transmit(&txHeader, (short*)message, &hCom, nComRate, nComBits, timeout, COMPORT);
					}
					else
					{
						printf("\n\nPress a key to transmit ...");
						getchar();
						NHtransmit((short*)message, sizeof(message), &hCom, nComRate, nComBits, timeout, COMPORT);
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

void compressionRatio(int compSize, int fileSize)
{

	printf("\n	Compression Ratio: %d/%d bytes (%.1f%%)\n", compSize, fileSize, 100 * (float)compSize / (float)fileSize);


}

void encryptXOR(void* message, int *TextBufSize)
{
	char encBuf[140], decBuf[140], secretKey[140];
	int messageLen = *TextBufSize;
	int secretKeyLen;
	//int encType;
	int i;

	printf("Please enter a single word encryption key: ");
	scanf_s("%s", secretKey, 139);
	secretKeyLen = strlen(secretKey);

	printf("Message will be encrypted to hexidecimal using XOR encryption: \n");

	printf("\nNow encrypting ...\n");


	// Encrypt the message (xor)
	xorCipher(message, messageLen, secretKey, secretKeyLen, encBuf);
	printf("XOR Encrypted message in hex:");                               // Will not print as a string so print in HEX, one byte at a time
	for (i = 0; i < messageLen; i++) {
		printf(" %02x", encBuf[i]);
	}

}

void decryptXOR(int messageLen, char* decBuf, char* encBuf)
{
	//Decrypt the message (xor)

	int secretKeyLen;
	char secretKey[140];
	printf("Please enter a single word encryption key: ");
	scanf_s("%s", secretKey, 139);
	secretKeyLen = strlen(secretKey);
	secretKey[secretKeyLen] = '\0';
	xorCipher(encBuf, messageLen, secretKey, secretKeyLen, decBuf);
	printf("\nXOR Decrypted Message: %s\n\n\n\n", decBuf);                       // Can print as a string

}


int DD(void* message, char* messageType, int* textBufSize, long lBigBufSize)
{

	Header rxHeader{};												// Header received
	void* rxPayload = NULL;											// Received payload (buffer) - void so it can be any data type


	//bytesRead = receive(&rxHeader, &rxPayload, hComRx, COMPORT_Rx, nComRate, nComBits, timeout);		// Pass pointer to rxPayload so can access malloc'd memory inside the receive function from main()

	printf("\n\nRxHeader.payLoadtype is %d\n\n", rxHeader.payLoadType);

	// Use header info to determine if payload needs to be decrypted or decompressed
	if (rxHeader.encryption != 0) {
		printf("\nDecrypting the message:...\n");

		//Would you like to decrypt the message (Y/N)?
		decryptXOR(rxHeader.payloadSize, (char*)message, (char*)rxPayload);
	}
	else {
		printf("\nMessage is not encrypted\n");
	}
	if (rxHeader.compression == 1) {	//Uncompress Huffman compression
		printf("\nDecompressing the text from RLE... \n");
		// rxPayload = decompress(rxPayload)

		RLE_Uncompress((unsigned char*)rxPayload, (unsigned char*)message, *textBufSize * sizeof(char));

	}
	else if (rxHeader.compression == 2) //Uncompress Huffman compression
	{
		printf("\nDecompressing the text from Huffman... \n");

		Huffman_Uncompress((unsigned char*)rxPayload, (unsigned char*)message, rxHeader.payloadSize, *textBufSize * sizeof(char));
		compressionRatio(*(int*)message, *textBufSize * sizeof(char));
	}
	else if (rxHeader.compression == 3) //Uncompress both huffman and RLE compression
	{
		printf("\nDecompressing the text from both compression styles... \n");

		RLE_Uncompress((unsigned char*)rxPayload, (unsigned char*)message, *textBufSize * sizeof(char));
		Huffman_Uncompress((unsigned char*)message, (unsigned char*)rxPayload, rxHeader.payloadSize, *textBufSize * sizeof(char));

	}
	else {
		printf("\nMessage is not compressed\n");
	}

	return(0);
}



//int CRC(void* message, void* sentMessage)
//{
//	//unsigned char inBuf[] = "0123456789"; //input
//
//	int nBytes = strlen((char*)message); //number of characters
//	crc compCRC;
//	char CRCstring[8];
//
//	//char* sentMessage = (char*)malloc((nBytes + 8) * sizeof(unsigned char)); //buffer	
//
//	int i; //counter
//
//	//Complete CRCs and send message
//	//Change bits to see effect on CRC
//	for (i = 0; i <= nBytes; i++)
//	{
//		compCRC = crcSlow((unsigned char*)message, nBytes);
//		sprintf(CRCstring, " 0x%x", compCRC);
//		strcpy((char*)sentMessage, (char*)message);
//		strcat((char*)sentMessage, CRCstring);
//		printf("Sent message with CRC. %s\n", sentMessage);
//		*(char*)message = '0';
//		//*message[i++];
//	}
//	free(sentMessage);
//	return(0);
//
//}





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