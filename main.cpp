/*	Filename: main.cpp
Author: Michael Galle
Date: Updated 2022
Details: Testing mainline for Windows sound API
*/

#include "sound.h"
//#include "Queues.h"
#include <stdio.h>
#include <windows.h>

int	main(int argc, char *argv[])
{
	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize*sizeof(short));		// buffer used for reading recorded sound from file

	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;
	FILE* fp;

	int messagetype; //user input
	//int nummessages; //how many messages the user will send
	link p, q;

	printf("Record Audio, or Send Text Message? (Audio = 0, Text = 1)\n");
	scanf_s("%d", &messagetype);
	if (messagetype == 0)
	{
		//printf("How many messages? (0 - 5):\n");
		//scanf_s("%d", &nummessages);

		//Initiate Playback and Recording
		
		InitQueue();
		p = (link)malloc(sizeof(Node));
		for (int i = 0; i < 2; i++) {
			InitializePlayback();
			InitializeRecording();
			//p->Data.sid = i;
			//GetMessageFromFile(p->Data.message, MessLength[randomNumber], randomNumber, NumQuotes, Indexes, MessLength);
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
				fopen_s(&f, "C:\\myfiles\\recording.dat", "wb");
				if (!f) {
					printf("unable to open %s\n", "C:\\myfiles\\recording.dat");
					return 0;
				}
				printf("Writing to sound file ...\n");
				fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
				p->Data.audio = fread(iBigBuf, sizeof(short), lBigBufSize, f); //what datatype should be stored in the queue??
				fclose(f);

			}
		}

			// replay audio recording from file -- read and store in buffer, then use playback() to play it
			printf("Would you like to replay the saved audio recording from the file? (y/n): ");
			scanf_s("%c", &replay, 1);
			while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
			if ((replay == 'y') || (replay == 'Y')) {
				/* Open input file */
				fopen_s(&f, "C:\\myfiles\\recording.dat", "rb");
				if (!f) {
					printf("unable to open %s\n", "C:\\myfiles\\recording.dat");
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


		while (!IsQueueEmpty()) {
			q = DeQueue();
			//PlayBuffer(q->Data.audio, lBigBufSize);
			//printf("\n Playing audio\n", q->Data.audio);


			free(q);
		}
	}

	
	printf("\n");
	system("pause");
	return(0);
}