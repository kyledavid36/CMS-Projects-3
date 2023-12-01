#pragma once
#include "Queues.h"


void setup(int* menuchoice, int* TextBufSize, int* RecordTime, int* compointer, int *txrx);  //comms, text buffer size, and recording time

void mainMenu(int* menuchoice, char options[25][40], int *txrx);

int QueuesTest(int NumQuotes, long int* Indices, int* LengthMessage, char* Message, const int BUFSIZE, int* txrx);

int CommsTest(int* txrx, long int lBigBufSize, short int* audiomessage);

void InputText(char *Message, int *TextBufSize);


void RecordAudio(long lBigBufSize, short* iBigBuf);

void PlaybackAudio(long lBigBufSize, short* iBigBuf);

int SaveAudio(long lBigBufSize, short* iBigBuf);

void CompressMessage(char* MessageType, char* inputfilename, unsigned char* in, long lBigBufSize, short* iBigBuf);

void SendReceive(void* message, int headerOnOff, int* txrx, char* MessageType);

void AddMessageToQueue(link p, void* message);

void myFlushAll();



//void getMessageFromUser(char* Message);
//
//int messageloop();
//void myFlushAll();
//
//int TextSettings(int TextBufSize);
//void NoQueues(long lBigBufSize, short* iBigBuf, char* Message);
////void TestAll();