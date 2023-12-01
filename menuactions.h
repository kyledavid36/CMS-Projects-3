#pragma once
#include "Queues.h"


void setup(int* menuchoice, int* TextBufSize, int* RecordTime, int* compointer, int *txrx);  //comms, text buffer size, and recording time

void mainMenu(int* menuchoice, char options[16][40], int *txrx);

int QueuesTest(int NumQuotes, long int* Indices, int* LengthMessage, char* Message, const int BUFSIZE, int* txrx, LPCSTR COMPORT);

int CommsTest(int* txrx, long int lBigBufSize, short int* audiomessage, LPCSTR COMPORT);

void InputText(char *Message, int *TextBufSize);

void RecordAudio(long lBigBufSize, short* iBigBuf);

void PlaybackAudio(long lBigBufSize, short* iBigBuf);

int SaveAudio(long lBigBufSize, short* iBigBuf);

void CompressMessage(char* MessageType, void* message, long lBigBufSize);

void SendReceive(void* message, int headerOnOff, int* txrx, char* MessageType, LPCSTR COMPORT);

void AddMessageToQueue(link p, void* message);

void myFlushAll();

int DD(void* message, char* messageType, int* textBufSize, long lBigBufSize);

void encryptXOR(void* message, int* TextBufSize);

void decryptXOR(int messageLen, char* decBuf, char* encBuf);

void compressionRatio(int compSize, int fileSize);
//
//int CRC(void* message, void* sentMessage);


//void getMessageFromUser(char* Message);
//
//int messageloop();
//void myFlushAll();
//
//int TextSettings(int TextBufSize);
//void NoQueues(long lBigBufSize, short* iBigBuf, char* Message);
////void TestAll();