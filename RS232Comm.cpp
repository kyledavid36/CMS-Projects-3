/* RS232Comm.cpp - Implementation for the RS232 communications module
 * By: Michael A. Galle
 *
 */
#include <Windows.h>    // Includes the functions for serial communication via RS232
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RS232Comm.h"
#include "Queues.h"
#include "Header.h"

#define EX_FATAL 1

extern const int BUFSIZE;							// Buffer size
extern wchar_t COMPORT_Rx[];
extern wchar_t COMPORT_Tx[];
extern HANDLE hCom;								// Pointer to the selected COM port (Receiver)
extern int nComRate;								// Baud (Bit) rate in bits/second 
extern int nComBits;								// Number of bits per frame
extern COMMTIMEOUTS timeout;						// A commtimeout struct variable
extern int* txrx;

//extern int com;
wchar_t COMPORT[5];


void NHtransmit(void* Payload, long int buffersize)
{
	initPort();						// Initialize the Tx port
	outputToPort(Payload, buffersize);				// Send payload
	Sleep(500);															// Allow time for signal propagation on cable 
	purgePort();													// Purge the Tx port
	CloseHandle(hCom);													// Close the handle to Tx port 
}

DWORD NHreceive(void** Payload, long int buffersize)
{
	DWORD bytesRead;
	initPort();				// Initialize the Rx port
	Payload = (void**)malloc(buffersize);				// Allocate buffer memory to receive payload. Will have to recast these bytess later to a specific data type / struct / etc - rembmer top free it in main()
	bytesRead = inputFromPort(Payload, buffersize);// Receive payload 
	purgePort();													// Purge the Rx port
	CloseHandle(hCom);													// Close the handle to Rx port 
	return bytesRead;													// Number of bytes read
}


// Main functions

void transmit(Header* txHeader, void* txPayload) {
	initPort();				// Initialize the Tx port
	outputToPort(txHeader, sizeof(Header));						// Send Header
	outputToPort(txPayload, (*txHeader).payloadSize);				// Send payload
	Sleep(500);															// Allow time for signal propagation on cable 
	purgePort();													// Purge the Tx port
	CloseHandle(hCom);													// Close the handle to Tx port 
}

DWORD receive(Header* rxHeader, void** rxPayload) {
	// Note: Pointer to rxPayload buffer (pointer to a pointer) is passed to this function since this function malloc's the amount of memory required - need to free it in main()
	DWORD bytesRead;  
	initPort();				// Initialize the Rx port
	inputFromPort(rxHeader, sizeof(Header));						// Read in Header first (which is a standard number of bytes) to get size of payload 
	*rxPayload = (void*)malloc((*rxHeader).payloadSize);				// Allocate buffer memory to receive payload. Will have to recast these bytess later to a specific data type / struct / etc - rembmer top free it in main()
	bytesRead = inputFromPort(*rxPayload, (*rxHeader).payloadSize);// Receive payload 
	purgePort();													// Purge the Rx port
	CloseHandle(hCom);													// Close the handle to Rx port 
	return bytesRead;													// Number of bytes read
}

/* *********** Support Functions ******************************************/
// Initializes the port and sets the communication parameters
void initPort() {
	
	if (*txrx == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			COMPORT[i] = COMPORT_Tx[i];
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			COMPORT[i] = COMPORT_Rx[i];
		}
	}
	createPortFile();						// Initializes hCom to point to PORT#
	purgePort();									// Purges the COM port
	SetComParms();		// Uses the DCB structure to set up the COM port
	purgePort();
}

// Purge any outstanding requests on the serial port (initialize)
void purgePort() {
	PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}

// Output/Input messages to/from ports 
void outputToPort( LPCVOID buf, DWORD szBuf) {
	int i=0;
	DWORD NumberofBytesTransmitted;
	LPDWORD lpErrors=0;
	LPCOMSTAT lpStat=0; 

	i = WriteFile(
		hCom,										// Write handle pointing to COM port
		buf,										// Buffer size
		szBuf,										// Size of buffer
		&NumberofBytesTransmitted,					// Written number of bytes
		NULL
	);
	// Handle the timeout error
	if (i == 0) {
		printf("\nWrite Error: 0x%x\n", GetLastError());
		ClearCommError(hCom, lpErrors, lpStat);		// Clears the device error flag to enable additional input and output operations. Retrieves information ofthe communications error.	
	}
	else
		printf("\nSuccessful transmission, there were %ld bytes transmitted\n", NumberofBytesTransmitted);
}

DWORD inputFromPort(LPVOID buf, DWORD szBuf) {
	int i = 0;
	DWORD NumberofBytesRead;
	LPDWORD lpErrors = 0;
	LPCOMSTAT lpStat = 0;

	i = ReadFile(
		hCom,										// Read handle pointing to COM port
		buf,										// Buffer size
		szBuf,  									// Size of buffer - Maximum number of bytes to read
		&NumberofBytesRead,
		NULL
	);
	// Handle the timeout error
	if (i == 0) {
		printf("\nRead Error: 0x%x\n", GetLastError());
		ClearCommError(hCom, lpErrors, lpStat);		// Clears the device error flag to enable additional input and output operations. Retrieves information ofthe communications error.
	}
	else
		printf("\nSuccessful reception!, There were %ld bytes read\n", NumberofBytesRead);

	return(NumberofBytesRead);
}



// Sub functions called by above functions
/**************************************************************************************/
// Set the hCom HANDLE to point to a COM port, initialize for reading and writing, open the port and set securities
void createPortFile() {
	// Call the CreateFile() function 
	hCom = CreateFile(
		COMPORT,									// COM port number  --> If COM# is larger than 9 then use the following syntax--> "\\\\.\\COM10"
		GENERIC_READ | GENERIC_WRITE,				// Open for read and write
		NULL,										// No sharing allowed
		NULL,										// No security
		OPEN_EXISTING,								// Opens the existing com port
		FILE_ATTRIBUTE_NORMAL,						// Do not set any file attributes --> Use synchronous operation
		NULL										// No template
	);
	
	if (hCom == INVALID_HANDLE_VALUE) {
		printf("\nFatal Error 0x%x: Unable to open\n", GetLastError());
	}
	else {
		printf("\nCOM is now open\n");
	}
}

static int SetComParms() {
	DCB dcb;										// Windows device control block
	// Clear DCB to start out clean, then get current settings
	memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(hCom, &dcb))
		return(0);

	// Set our own parameters from Globals
	dcb.BaudRate = nComRate;						// Baud (bit) rate
	dcb.ByteSize = (BYTE)nComBits;					// Number of bits(8)
	dcb.Parity = 0;									// No parity	
	dcb.StopBits = ONESTOPBIT;						// One stop bit
	if (!SetCommState(hCom, &dcb))
		return(0);

	// Set communication timeouts (SEE COMMTIMEOUTS structure in MSDN) - want a fairly long timeout
	memset((void *)&timeout, 0, sizeof(timeout));
	timeout.ReadIntervalTimeout = 500;				// Maximum time allowed to elapse before arival of next byte in milliseconds. If the interval between the arrival of any two bytes exceeds this amount, the ReadFile operation is completed and buffered data is returned
	timeout.ReadTotalTimeoutMultiplier = 1;			// The multiplier used to calculate the total time-out period for read operations in milliseconds. For each read operation this value is multiplied by the requested number of bytes to be read
	timeout.ReadTotalTimeoutConstant = 5000;		// A constant added to the calculation of the total time-out period. This constant is added to the resulting product of the ReadTotalTimeoutMultiplier and the number of bytes (above).
	SetCommTimeouts(hCom, &timeout);
	return(1);
}

