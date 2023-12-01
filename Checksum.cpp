

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "Checksum.h"
		unsigned int Checksum(void* buf, int iBufLen, CHECKSUM iType)
		{
			unsigned short* newBuf16; //for 16-bit data (this is quite large so not used often)
			unsigned char* newBuf8; //for 8-bit data
			int i;
			unsigned int iSum = 0;
			switch (iType)
			{
				case CHK_16BIT: //16-bit check
				newBuf16 = (unsigned short*)buf;
				for (i = 0; i < iBufLen; i++)
				{
					iSum += *newBuf16++;
				}
				return(iSum % 0x100);

				case CHK_8BIT: //8-bit check
				newBuf8 = (unsigned char*)buf; //find what is stored in pointer
				for (i = 0; i < iBufLen; i++)
				{
					iSum += *newBuf8++; //add every result together,
				}
				return(iSum % 0x100);
				default:
					return(0x111111); //error
			}
		}