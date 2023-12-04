#pragma once
#include "Queues.h"


void setup(int* menuchoice, int* RecordTime);  //comms, text buffer size, and recording time

void mainMenu(int* menuchoice, char options[16][40]);

int QueuesTest(int NumQuotes, long int* Indices, int* LengthMessage, char* Message, const int BUFSIZE);

int CommsTest( short int* audiomessage);

void InputText(char *Message);

void RecordAudio( short* iBigBuf);

void PlaybackAudio( short* iBigBuf);

int SaveAudio( short* iBigBuf);

void CompressMessage(char* MessageType, void* message, unsigned int insize, unsigned char* buf, long* compsize);

void AddMessageToQueue(link p, void* message);

void myFlushAll();

int DD(void* message, char* messageType, int onoff);

void SendReceive(void* message, int headerOnOff,  char* MessageType);

void encryptXOR(void* message);

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