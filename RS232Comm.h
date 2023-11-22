/* RS232Comm.h - Interface for the RS232 communications module
 * By: Michael A. Galle
 *
 */
#pragma once
#include "Header.h"
//#include "menuactions.h"

// Main functions
void transmit(Header* txHeader, void* txPayload, HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout);
DWORD receive(Header* rxHeader, void** rxPayload, HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout);
 
// Prototype the functions to be used
void initPort(HANDLE* hCom, wchar_t* COMPORT, int nComRate, int nComBits, COMMTIMEOUTS timeout);
void purgePort(HANDLE* hCom);
void outputToPort(HANDLE* hCom, LPCVOID buf, DWORD szBuf);
DWORD inputFromPort(HANDLE* hCom, LPVOID buf, DWORD szBuf);

// Sub functions
void createPortFile(HANDLE* hCom, wchar_t* COMPORT);
static int SetComParms(HANDLE* hCom, int nComRate, int nComBits, COMMTIMEOUTS timeout);