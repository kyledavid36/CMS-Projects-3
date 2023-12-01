#include <stdio.h>
#include <windows.h>
#include "menuactions.h"
#include "Queues.h"
#include "RS232Comm.h"
#include "sound.h"
#include "compression.h"
#include "rle.h"
#include "huffman.h"
#include "message.h"
#include "headerSettings.h"

// Declare constants, variables and communication parameters
extern const int BUFSIZE;							// Buffer size
//extern wchar_t COMPORT_Rx[];						// COM port used for Rx 
//extern wchar_t COMPORT_Tx[];						// COM port used for Tx 
extern HANDLE hCom;								// Pointer to the selected COM port (Receiver)
extern int nComRate;								// Baud (Bit) rate in bits/second 
extern int nComBits;								// Number of bits per frame
extern COMMTIMEOUTS timeout;						// A commtimeout struct variable


void menu(int *menuchoice, int *TextBufSize, int *RecordTime, int *compointer, long lBigBufSize, int *txrx, LPCSTR COMPORT, Header txHeader, Header rxHeader)
{
	
	int woohoo = FALSE;	//do while loop of menu

	/*******************	BUFFERS ****************************/
	void* message = (void*)malloc(lBigBufSize * sizeof(short));

	/**************		QUEUES	LINKS			***********/
	link p;

	/************		AUDIO FILE		 ************/
	char* inputfilename = (char*)malloc(30 * sizeof(char));
	char AudioFile[30] = "recording.dat";
	inputfilename = AudioFile;

	/************		Message Type	*********************/
	char* MessageType;
	char Text = 'T';
	MessageType = &Text;

	/***********	HEADER ON/OFF VARIABLE***********/
	int headerOnOff = 0;

	/***********	MESSAGE.CPP REQUIRED MALLOC	***************/
	int NumQuotes = fnumQuotes();
	long int* Indices = (long int*)malloc(NumQuotes * sizeof(long int));
	int * LengthMessage = (int*)malloc(NumQuotes * sizeof(int));

	

	char options[16][40] = {
		"QUEUES TEST (Tx/Rx)",
		"AUDIO / COMMS TEST (Tx/Rx)",
		"INPUT TEXT MESSAGE (Tx)",
		"RECORD AUDIO MESSAGE (Tx)",
		"SAVE AUDIO MESSAGE TO FILE (Tx)",
		"PLAYBACK AUDIO MESSAGE (Tx)",
		"COMPRESS MESSAGE (Tx)",
		"ENCRYPT MESSAGE (Tx)",
		"HEADER SETTINGS (Tx)",
		"SEND / RECEIVE MESSAGE (Tx/Rx)",
		"ADD MESSAGE TO QUEUE (Rx)",
		"SHOW QUEUE (Rx)",
		"POP FROM THE HEAD (Rx)",
		"CLEAR QUEUE (Rx)",
		"ERROR DETECTION / CORRECTION (Rx)",
		"Exit" };

	do
	{
		switch (*menuchoice)
		{
			/*******************************	SETUP	*******************************/
		case 'S':

			
			setup(menuchoice, TextBufSize, RecordTime, compointer, txrx);
			
			printf("\n			SETUP SUMMARY:\n");
			
			printf("				COM Port: COM%d\n", *compointer);
			printf("				Text Buffer Size: %d\n", *TextBufSize);
			printf("				Record Time: %d seconds\n\n", *RecordTime);
			myFlushAll();

			if (*compointer == 0) { COMPORT = "COM0"; }
			if (*compointer == 1) { COMPORT = "COM1"; }
			if (*compointer == 2) { COMPORT = "COM2"; }
			if (*compointer == 3) { COMPORT = "COM3"; }
			if (*compointer == 4) { COMPORT = "COM4"; }
			if (*compointer == 5) { COMPORT = "COM5"; }
			if (*compointer == 6) { COMPORT = "COM6"; }

			printf("\nYou entered: %d, and the COMPORT is: %s.\n", *compointer, COMPORT);

			break;

			/******************************		MAIN MENU	***************************/
		case 0: //Choose the setting here

			
			mainMenu(menuchoice, options, txrx);
			
			break;
			/*****************************	QUEUES TEST	*****************************/
		case 1:
			QueuesTest(NumQuotes, Indices, LengthMessage, (char*)message, 140, txrx, COMPORT);
			
			*menuchoice = 0;
			break;
			/*************************		COMMS TEST		****************************/
		case 2: //Transmit and Receive Test
			printf("\nYou entered: %d, and the COMPORT is: %s.\n", *compointer, COMPORT);
			
			CommsTest(txrx, lBigBufSize, (short*)message, COMPORT);
			
			*menuchoice = 0;
			break;
			/************************	INPUT TEXT MESSAGE	****************************/
		case 3:
			InputText((char*)message, TextBufSize);
			*MessageType = 'T';
			*menuchoice = 0;
			break;
			/***********************	RECORD AUDIO MESSAGE	************************/
		case 4:
			lBigBufSize = *RecordTime * SAMPLES_SEC;
			RecordAudio(lBigBufSize, (short *)message);
			*MessageType = 'A';
			*menuchoice = 0;
			break;
			/*******************	SAVE AUDIO MESSAGE TO FILE		*********************/
		case 5:
			SaveAudio(lBigBufSize, (short*)message);
			*menuchoice = 0;
			break;
			/***********************	PLAYBACK AUDIO MESSAGE	*************************/
		case 6:
			PlaybackAudio(lBigBufSize, (short*)message);
			*menuchoice = 0;
			break;
			/**********************		COMPRESS MESSAGE		*************************/
		case 7:
			CompressMessage(MessageType, (short*)message, lBigBufSize);
			PlaybackAudio(lBigBufSize, (short*) message);
			*menuchoice = 0;
			break;
			/***********************	ENCRYPT MESSAGE		**************************/
		case 8:
			encryptXOR(message, TextBufSize);
			*menuchoice = 0;
			break;
			/************************	HEADER SETTINGS		**************************/
		case 9:
			headerOnOff = headerSettings();
			*menuchoice = 0;
			break;
			/************************	SEND / RECEIVE MESSAGE		**************************/
		case 10:

			SendReceive(message, headerOnOff, txrx, MessageType, COMPORT);
			*menuchoice = 0;
			break;
			/***************		ADD MESSAGE TO QUEUE		*******************/
		case 11:
			p = (link)malloc(sizeof(Node));
			AddMessageToQueue(p, message);
			*menuchoice = 0;
			break;
			/**************************		SHOW QUEUE		****************************/
		case 12:
			traverse(returnHead(), visit);
			*menuchoice = 0;
			break;
			/**************************		POP FROM THE HEAD		************************/
		case 13:
			
			//deleteR(returnHead(), returnHead()->pNext, p->Data);
			*menuchoice = 0;
			break;
			/**************************		CLEAR QUEUE		************************/
		case 14:
			while (!IsQueueEmpty())
			{
				DeQueue();
			}
			
			*menuchoice = 0;
			break;
			/******************		ERROR DETECTION / CORRECTION		***************/
		case 15:
			
			*menuchoice = 0;
			break;
			/*****************************		EXIT	*****************************/
		case 16:
			woohoo = TRUE;
			break;
		default:
			woohoo = TRUE;
			break;
		}
	} while (!woohoo);

	
	free(message);
	free(inputfilename);
	/*free(Indices);
	free(LengthMessage);*/
}

