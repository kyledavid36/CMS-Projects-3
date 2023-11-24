/* main.cpp - Client for the Tx/Rx Program the sets up framed data where the size of payload is FLEXIBLE 
 *            as it is given to the receiving side in the Header portion of the frame 
 * Author: Michael A. Galle
 *
 */

#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RS232Comm.h"
#include "Header.h"
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
extern short iBigBuf[SAMPLES_SEC * RECORD_TIME];
extern long  lBigBufSize;

 // The client - A testing main that calls the functions below


 int tx_rx_Audio() {

	char selection;													// Tx or Rx (can run two instances of this program - double click the exe file)
	Header txHeader;												// Header transmitted 
	Header rxHeader;												// Header received
	void* rxPayload = NULL;											// Received payload (buffer) - void so it can be any data type
	DWORD bytesRead;												// Number of bytes received

	printf("Enter (1) for Transmit Audio (2) for Receive Audio: ");
	selection = getchar();

	// Transmit
	if (selection == '1') {
		
		// Payload (sample type is Audio)
		// initialize recording
		InitializeRecording();

		// start recording
		RecordBuffer(iBigBuf, lBigBufSize);
		CloseRecording();

		// Header (sample data type is text but this should work with audio and images as well)
		txHeader.sid = 1;											
		txHeader.rid = 2;
		txHeader.payloadSize = lBigBufSize*sizeof(short);			//(audio lBigBufSize*2)  Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
		txHeader.compression = 0;									// None
		txHeader.encryption = 0;									// None
		txHeader.payLoadType = 1;									// Audio

		transmit(&txHeader, (char*)iBigBuf, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
	}
	
	// Receive 
	else if (selection == '2') {
		// initialize playback and recording
		InitializePlayback();

		// playback recording 
		printf("\nPlaying recording from buffer\n");
	
		bytesRead = receive(&rxHeader, &rxPayload, &hComRx, COMPORT_Rx, nComRate, nComBits, timeout);		// Pass pointer to rxPayload so can access malloc'd memory inside the receive function from main()
		
		// Use header info to determine if payload needs to be decrypted or decompressed
		if (rxHeader.encryption != 0) {
			printf("\nMessage is encrypted so need to decrypt!\n");
			// rxPayload = decrypt(rxPayload)
		}
		else {
			printf("\nMessage is not encrypted\n");
		}
		if (rxHeader.compression != 0) {
			printf("\nMessage is compressed so need to decompress!\n");
			// rxPayload = decompress(rxPayload)
		}
		else {
			printf("\nMessage is not compressed\n");
		}

		// Determine type of payload from header data and corresponding action to complete (e.g. display text, play audio, etc)
		if (rxHeader.payLoadType == 0) {
			printf("\nPayload is text\n");
			printf("\nMessage recieved: %s\n", (char*)rxPayload);		// May need to set rxPayload[bytesRead - 1] = '\0'
			// Enqueue()
			free(rxPayload);											// malloc'd in the receive function
		}
		else if (rxHeader.payLoadType == 1) {
			printf("\nPayload is audio\n");
			PlayBuffer((short*)rxPayload,rxHeader.payloadSize/sizeof(short));
			ClosePlayback();
			// Enqueue
			free(rxPayload);											// malloc'd in the receive function
		}
		else {
			printf("\Payload is an image or something else ...\n");
			// DisplayImage(); 
			free(rxPayload);											// malloc'd in the receive function
		}
	}
	else {
		printf("\nPlease make a valid selection\n");
	}
	system("pause");

	return(0);
}