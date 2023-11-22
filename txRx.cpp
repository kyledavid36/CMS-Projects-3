
#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RS232Comm.h"
#include "Header.h"

// Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size
wchar_t COMPORT_Rx[] = L"COM6";						// COM port used for Rx 
wchar_t COMPORT_Tx[] = L"COM7";						// COM port used for Tx 
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 230400;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable


// The client - A testing main that calls the functions below
//int tx_rx_Test(wchar_t COMPORT_Rx[], wchar_t COMPORT_Tx[], HANDLE hComRx, HANDLE hComTx, int nComRate, int nComBits, COMMTIMEOUTS timeout) {
int txRx() {

	char selection;													// Tx or Rx (can run two instances of this program - double click the exe file)
	Header txHeader;												// Header transmitted 
	Header rxHeader;												// Header received
	void* rxPayload = NULL;											// Received payload (buffer) - void so it can be any data type
	DWORD bytesRead;												// Number of bytes received

	printf("Enter (1) for Transmit (2) for Receive (2): ");
	selection = getchar();

	// Transmit
	if (selection == '1') {

		// Payload (sample type is text)
		char txPayload[] = "\nHi there this is a great message for you\n"; 	// Payload is a text message in this example but could be any data	

		// Header (sample data type is text but this should work with audio and images as well)
		txHeader.sid = 1;
		txHeader.rid = 2;
		txHeader.payloadSize = strlen(txPayload) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
		txHeader.compression = 0;									// None
		txHeader.encryption = 0;									// None
		txHeader.payLoadType = 0;									// Text

		transmit(&txHeader, txPayload, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
	}
	return(0);
}
