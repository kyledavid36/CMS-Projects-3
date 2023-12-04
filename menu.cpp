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
#include "VoteOn.h"

// Declare constants, variables and communication parameters
extern const int BUFSIZE;							// Buffer size
extern wchar_t COMPORT_Rx[];						// COM port used for Rx 
extern wchar_t COMPORT_Tx[];						// COM port used for Tx 
extern HANDLE hCom;								// Pointer to the selected COM port (Receiver)
extern int nComRate;								// Baud (Bit) rate in bits/second 
extern int nComBits;								// Number of bits per frame
extern COMMTIMEOUTS timeout;						// A commtimeout struct variable
extern int* txrx;
extern long lBigBufSize;

/**************		message.h		****************/
extern int NumQuotes;
extern int* TextBufSize;
/************		AUDIO FILE		 ************/
extern char inputfilename[30];

void menu(void* message, long int* Indices, int* LengthMessage, int *menuchoice, int *RecordTime, Header txHeader, Header rxHeader)
{
	
	int woohoo = FALSE;	//do while loop of menu

	

	/**************		QUEUES	LINKS			***********/
	link p;

	/************		Message Type	*********************/
	char* MessageType;
	char Text = 'T';
	MessageType = &Text;

	/***********	HEADER ON/OFF VARIABLE***********/
	int headerOnOff = 0;

	

	

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
			/******************************		MAIN MENU	***************************/
		case 0: //Choose the setting here

			
			mainMenu(menuchoice, options);
			
			break;
			/*****************************	QUEUES TEST	*****************************/
		case 1:
			QueuesTest(NumQuotes, Indices, LengthMessage, (char*)message, 140);
			
			*menuchoice = 0;
			break;
			/*************************		COMMS TEST		****************************/
		case 2: //Transmit and Receive Test
			
			CommsTest((short*)message);
			
			*menuchoice = 0;
			break;
			/************************	INPUT TEXT MESSAGE	****************************/
		case 3:
			InputText((char*)message);
			*MessageType = 'T';
			*menuchoice = 0;
			break;
			/***********************	RECORD AUDIO MESSAGE	************************/
		case 4:
			lBigBufSize = *RecordTime * SAMPLES_SEC;
			RecordAudio((short *)message);
			*MessageType = 'A';
			*menuchoice = 0;
			break;
			/*******************	SAVE AUDIO MESSAGE TO FILE		*********************/
		case 5:
			SaveAudio((short*)message);
			*menuchoice = 0;
			break;
			/***********************	PLAYBACK AUDIO MESSAGE	*************************/
		case 6:
			PlaybackAudio((short*)message);
			*menuchoice = 0;
			break;
			/**********************		COMPRESS MESSAGE		*************************/
		case 7:
			CompressMessage(MessageType, (short*)message);
			
			*menuchoice = 0;
			break;
			/***********************	ENCRYPT MESSAGE	DECRYPT	**************************/
		case 8:
		
			encryptXOR(message);
			
			*menuchoice = 0;
			break;
			/************************	HEADER SETTINGS		**************************/
		case 9:
			headerOnOff = headerSettings();
			*menuchoice = 0;
			break;
			/************************	SEND / RECEIVE MESSAGE		**************************/
		case 10:
			SendReceive( message, headerOnOff, MessageType);
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
			voteOnRid();
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
}

