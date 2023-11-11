/*	Filename: main.cpp
Author: Michael Galle
Date: Updated 2022
Details: Testing mainline for Windows sound API
*/

#include "sound.h"
#include <stdio.h>
#include <windows.h>

int	audioTest()
{
	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize*sizeof(short));		// buffer used for reading recorded sound from file


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
		//fopen_s(&f, "C:\\Users\\kyled\\Documents\\Conestoga\\Current Semester\\F2.23\\Projects III\\CMS\\W1\\recording.dat", "wb");
		fopen_s(&f, "recording.dat", "wb");


		if (!f) {
			printf("unable to open %s\n", "C:\\Users\\kyled\\Documents\\Conestoga\\Current Semester\\F2.23\\Projects III\\CMS\\W1\\recording.dat");
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
		//fopen_s(&f, "C:\\Users\\kyled\\Documents\\Conestoga\\Current Semester\\F2.23\\Projects III\\CMS\\W1\\recording.dat", "rb");
		fopen_s(&f, "recording.dat", "rb");
		if (!f) {
			printf("unable to open %s\n", "C:\\Users\\kyled\\Documents\\Conestoga\\Current Semester\\F2.23\\Projects III\\CMS\\W1\\recording.dat");
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
	system("pause");
	return(0);
}