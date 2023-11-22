#pragma once

void getMessageFromUser(char* Message);
int menu(int TextBufSize, long lBigBufSize, short* iBigBuf, char* Message);
int messageloop();
void myFlushAll();
int getAudioFromUser(long lBigBufSize, short* iBigBuf);
int TextSettings(int TextBufSize);
void NoQueues(long lBigBufSize, short* iBigBuf, char* Message);
//void testAll();

