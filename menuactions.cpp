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


int menu()
{
	int pass = FALSE;
	int amount = FALSE;
	char x = NULL; //Tx or Rx
	char c;
	printf("Coded Messaging System\n	By: Amy Wentzell and Kyle Dick\n	Version: 2023\n\n\n");
	
	
	do
	{
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


int getAudioFromUser(short* Buffer, long lBufSize)
{

	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;
	FILE* fp;

	
	InitializeRecording();
	RecordBuffer(Buffer, lBufSize);
	CloseRecording();

	printf("\nPlaying recording from buffer\n");
	InitializePlayback();
	PlayBuffer(Buffer, lBufSize);
	ClosePlayback();

	
	// save audio recording
	printf("Would you like to save your audio recording? (y/n): ");
	scanf_s("%c", &save, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	if ((save == 'y') || (save == 'Y')) {
		// Open input file

		fopen_s(&f, "recording.dat", "wb");
		if (!f) {
			printf("unable to open %s\n", "recording.dat");
			return (0);
		}
		printf("Writing to sound file ...\n");
		fwrite(Buffer, sizeof(short), lBufSize, f);			//what datatype should be stored in the queue??

		fclose(f);

	}
	// replay audio recording from file -- read and store in buffer, then use playback() to play it
	printf("Would you like to replay the saved audio recording from the file? (y/n): ");
	scanf_s("%c", &replay, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	if ((replay == 'y') || (replay == 'Y')) {
		// Open input file
		fopen_s(&f, "recording.dat", "rb");
		if (!f) {
			printf("unable to open %s\n", "recording.dat");
			return 0;
		}
		printf("Reading from sound file ...\n");
		fread(Buffer, sizeof(short), lBufSize, f);				// Record to new buffer iBigBufNew
		fclose(f);
		InitializePlayback();
		printf("\nPlaying recording from saved file ...\n");
		PlayBuffer(Buffer, lBufSize);
		ClosePlayback();
	}
		

}


void testAll()
{

}