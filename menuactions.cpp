/******************************************************************************

Name: menuactions.cpp
Purpose: All menu functions for the CMS Projects III
Author: Amy Wentzell


******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "sound.h"
#include "Queues.h"

void myFlushAll()
{
	int c;
	do
	{
		c = getchar();

	} while (c != EOF && c != '\n');
}


int TextSettings(int TextBufSize)
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
			TextBufSize = MAX_QUOTE_LENGTH;
			pass1 = TRUE;
		}
		else if (0 < choice && choice <= 140)
		{
			TextBufSize = choice;
			printf("New Buffer Length is: %d", choice);
			
			pass1 = TRUE;

		}
		else
		{
			printf("\nERROR: invalid input. Please retry.\n");
		}
	} while (!pass1);
	return(TextBufSize);
}

int AudioSettings(int Case)
{
	int pass = FALSE;
	char choice;
	char c;


	do
	{
		printf("\nAUDIO SETTINGS\n");
	} while (!pass);
}




int menu(int TextBufSize)
{
	int pass = FALSE;
	int amount = FALSE;
	char x = NULL; //Tx or Rx
	char c;
	char setting = '0';
	char loc;

	char options[5][25] = { "Test Functionality","Audio Settings", "Text Settings", "Run the program (Tx/Rx)", "Exit" };

	printf("Coded Messaging System\n	By: Amy Wentzell and Kyle Dick\n	Version: 2023\n\n\n");

	do
	{

		switch (setting)
		{
			
			case '0':
				printf("\nWelcome! Choose your setting: \n\n");
				for (int i = 1; i < 6; i++)
				{
					printf("(%d)	", i);
					for (int j = 0; j < 25; j++)
					{
						printf("%c", options[i-1][j]);
					}
					printf("\n");
				}
				printf("Choice:\n");
				scanf_s("%c", &loc, 1);
				while ((c = getchar()) != '\n' && c != EOF) {}
				setting = loc;
				break;
			case '1':
				// TEST
				setting = '0';
				break;
			case '2':
				//AudioSettings
				setting = '0';
				break;
			case '3':
				//Text Settings

				TextBufSize = TextSettings(TextBufSize);
				setting = '0';
				printf("\n			Setting is %c, TextBufSize is %d.\n", setting, TextBufSize);
				break;
			case '4':
				printf("Are you transmitting or receiving? (Transmitting = 1, Receiving = 0):\n");
				scanf_s("%c", &x, 1);
				while ((c = getchar()) != '\n' && c != EOF) {}
				if (x == '1')
				{
					printf("How many messages would you like to send?(1 - 10):\n");
					scanf_s("%d", &amount);
					if (1 <= amount && amount <= 10)
					{
						pass = TRUE;
					}
					else
					{
						printf("\nERROR: invalid input. Please retry.\n");
					}
				}
				else if (x == '0')
				{
					pass = TRUE;
				}
				else
				{
					printf("\nERROR: invalid input. Please retry.\n");
				}
				break;
			case '5':
				exit(0);
				break;
			default:
				break;
			}
			
	} while (!pass);

	return(amount);
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


void getMessageFromUser(char* Message) {
	

	fflush(stdin); // Clear input buffer
	printf("Please enter a message:\n");
	scanf_s("%139[^\n]s", Message, 139);
	//printf("%s", Message);

}


int getAudioFromUser(long lBigBufSize, short *iBigBuf, short *iBigBufNew)
{

	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;

	// initialize playback and recording
	InitializePlayback();
	InitializeRecording();

	// start recording
	RecordBuffer(iBigBuf, lBigBufSize);
	CloseRecording();

	// playback recording 
	printf("\nPlaying recording from buffer\n");
	PlayBuffer(iBigBuf, lBigBufSize);
	ClosePlayback();

	// save audio recording  
	printf("Would you like to save your audio recording? (y/n): ");
	scanf_s("%c", &save, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	if ((save == 'y') || (save == 'Y')) {
		/* Open input file */
		fopen_s(&f, "recording.dat", "wb");
		if (!f) {
			printf("unable to open %s\n", "recording.dat");
			return 0;
		}
		printf("Writing to sound file ...\n");
		fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
		fclose(f);
	}

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
		fread(iBigBufNew, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
		fclose(f);
		InitializePlayback();
		printf("\nPlaying recording from saved file ...\n");
		PlayBuffer(iBigBufNew, lBigBufSize);
		ClosePlayback();
	}

	printf("\n");
		

}


void testAll()
{

}

