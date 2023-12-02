/* RS232Comm.h - Interface for the RS232 communications module
 * By: Michael A. Galle
 *
 */
#pragma once
#include "Header.h"
//#include "menuactions.h"

// Main functions

//No Header Transmit
void NHtransmit(void* Payload, long int buffersize);

//No Header Receive
DWORD NHreceive(void** Payload, long int buffersize);

void transmit(Header* txHeader, void* txPayload);
DWORD receive(Header* rxHeader, void** rxPayload);
 
// Prototype the functions to be used
void initPort();
void purgePort();
void outputToPort( LPCVOID buf, DWORD szBuf);
DWORD inputFromPort(LPVOID buf, DWORD szBuf);

// Sub functions
void createPortFile();
static int SetComParms();