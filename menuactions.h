#pragma once

void setup(int* menuchoice, int* TextBufSize, int* RecordTime, int* com1, int* com2, int *txrx);  //comms, text buffer size, and recording time

void mainMenu(int* menuchoice, char options[25][40], int *txrx);

void QueuesTest(const int BUFSIZE, int* txrx, HANDLE* hComTx, HANDLE* hComRx, wchar_t* COMPORT1, wchar_t* COMPORT2, int nComRate, int nComBits, COMMTIMEOUTS timeout, link p);


void InputText(char *Message, int *TextBufSize);

void RecordAudio(long lBigBufSize, short* iBigBuf);

void PlaybackAudio(long lBigBufSize, short* iBigBuf);

int SaveAudio(long lBigBufSize, short* iBigBuf);





void getMessageFromUser(char* Message);

int messageloop();
void myFlushAll();

int TextSettings(int TextBufSize);
void NoQueues(long lBigBufSize, short* iBigBuf, char* Message);
//void TestAll();