#include <stdio.h>
#include <windows.h>
#include "menuactions.h"
#include "Queues.h"
#include "RS232Comm.h"
#include "sound.h"

// Declare constants, variables and communication parameters
extern const int BUFSIZE;							// Buffer size
extern wchar_t COMPORT_Rx[];						// COM port used for Rx 
extern wchar_t COMPORT_Tx[];						// COM port used for Tx 
extern HANDLE hComRx;								// Pointer to the selected COM port (Receiver)
extern HANDLE hComTx;								// Pointer to the selected COM port (Transmitter)
extern int nComRate;								// Baud (Bit) rate in bits/second 
extern int nComBits;								// Number of bits per frame
extern COMMTIMEOUTS timeout;						// A commtimeout struct variable


/*void COMPORT(char comT, char comR)
{
	
	/*switch (comT)
	{
	case '0':

		COMPORT_Tx[] = L"COM0";
		break;
	case '1':
		COMPORT_Tx[] = L"COM1";
		break;
	case '2':
		COMPORT_Tx[] = L"COM2";
		break;
	case '3':
		COMPORT_Tx[] = L"COM3";
		break;
	case '4':
		COMPORT_Tx[] = L"COM4";
		break;
	case '5':
		COMPORT_Tx[] = L"COM5";
		break;
	case '6':
		COMPORT_Tx[] = L"COM6";
		break;
	default:
		COMPORT_Tx[] = L"COM6";
		break;
	}

	switch (comR)
	{
	case '0':

		COMPORT_Rx[] = L"COM0";
		break;
	case '1':
		COMPORT_Rx[] = L"COM1";
		break;
	case '2':
		COMPORT_Rx[] = L"COM2";
		break;
	case '3':
		COMPORT_Rx[] = L"COM3";
		break;
	case '4':
		COMPORT_Rx[] = L"COM4";
		break;
	case '5':
		COMPORT_Rx[] = L"COM5";
		break;
	case '6':
		COMPORT_Rx[] = L"COM6";
		break;
	default:
		COMPORT_Rx[] = L"COM3";
		break;
	}

}*/



void menu(int *menuchoice, int *TextBufSize, int *RecordTime, int *com1, int *com2, long lBigBufSize, short* iBigBuf, int *txrx)
{
	int woohoo = FALSE;
	char *Message = (char*)malloc(*TextBufSize * sizeof(char));
	link p, q;


	char* MessageType;
	char Text = 'T';
	MessageType = &Text;


	char options[25][40] = {
		"AUDIO TEST (Tx/Rx)",
		"QUEUES TEST (Tx/Rx)",
		"COMMS TEST (Tx/Rx)",
		"INPUT TEXT MESSAGE (Tx)",
		"RECORD AUDIO MESSAGE (Tx)",
		"SAVE AUDIO MESSAGE TO FILE (Tx)",
		"PLAYBACK AUDIO MESSAGE (Tx)",
		"COMPRESS MESSAGE (Tx)",
		"ENCRYPT MESSAGE (Tx)",
		"HEADER SETTINGS (Tx)",
		"SEND MESSAGE (Tx)",
		"RECEIVE MESSAGE (Rx)",
		"ADD MESSAGE TO QUEUE (Rx)",
		"SHOW QUEUE (Rx)",
		"POP FROM THE HEAD (Rx)",
		"ERROR DETECTION / CORRECTION (Rx)",
		"PHONE BOOK (Tx)",
		"Exit" };

	do
	{
		switch (*menuchoice)
		{
			/*******************************	SETUP	*******************************/
		case 'S':

			
			setup(menuchoice, TextBufSize, RecordTime, com1, com2, txrx);
			
			printf("\n			SETUP SUMMARY:\n");
			
			printf("				Transmitting COM Port: COM%d\n", *com1);
			printf("				Receiving COM Port: COM%d\n", *com2);
			printf("				Text Buffer Size: %d\n", *TextBufSize);
			printf("				Record Time: %d seconds\n\n", *RecordTime);
			myFlushAll();



			break;

			/******************************		MAIN MENU	***************************/
		case 0: //Choose the setting here

			mainMenu(menuchoice, options, txrx);
			
			break;
			/*****************************	AUDIO TEST	*****************************/
		case 1:
			*menuchoice = 0;
			break;
			/*****************************	QUEUES TEST	*****************************/
		case 2:
			QueuesTest(140, txrx, &hComTx, &hComRx, COMPORT_Tx, COMPORT_Rx, nComRate, nComBits, timeout, p);
			
			*menuchoice = 0;
			break;
			/*************************		COMMS TEST		****************************/
		case 3: //Transmit and Receive Test
			*menuchoice = 0;
			break;
			/************************	INPUT TEXT MESSAGE	****************************/
		case 4:
			InputText(Message, TextBufSize);
			*MessageType = 'T';
			*menuchoice = 0;
			break;
			/***********************	RECORD AUDIO MESSAGE	************************/
		case 5:
			lBigBufSize = *RecordTime * SAMPLES_SEC;
			RecordAudio(lBigBufSize, iBigBuf);
			*MessageType = 'A';
			*menuchoice = 0;
			break;
			/*******************	SAVE AUDIO MESSAGE TO FILE		*********************/
		case 6:
			SaveAudio(lBigBufSize, iBigBuf);
			*menuchoice = 0;
			break;
			/***********************	PLAYBACK AUDIO MESSAGE	*************************/
		case 7:
			PlaybackAudio(lBigBufSize, iBigBuf);
			*menuchoice = 0;
			break;
			/**********************		COMPRESS MESSAGE		*************************/
		case 8:
			SaveAudio(lBigBufSize, iBigBuf);
			//Huffman_Compress("recording.dat");
			*menuchoice = 0;
			break;
			/***********************	ENCRYPT MESSAGE		**************************/
		case 9:
			*menuchoice = 0;
			break;
			/************************	HEADER SETTINGS		**************************/
		case 10:

			*menuchoice = 0;
			break;
			/************************	SEND MESSAGE		**************************/
		case 11:
			if (*MessageType == 'A')
			{

			}
			else
			{

			}
			*menuchoice = 0;
			break;
			/**********************		SHOW QUEUE		******************************/
		case 12:
			ShowQueue(q);
			*menuchoice = 0;
			break;
			/***************		ERROR DETECTION AND CORRECTION		*******************/
		case 13:
			*menuchoice = 0;
			break;
			/**************************		PHONE BOOK		****************************/
		case 14:
			*menuchoice = 0;
			break;
			/*******************************	EXIT	*******************************/
		case 15:
			woohoo = TRUE;
			break;
		default:
			woohoo = TRUE;
			break;
		}
	} while (!woohoo);

	free(Message);
}

