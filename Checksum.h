
/* Checksum.h : Header file for parity calculations. */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#ifndef CHECKSUM_H
	
	#define CHECKSUM_H
	enum CHECKSUM { CHK_16BIT, CHK_8BIT, CHK_ERROR };
	unsigned int Checksum(void* buf, int iBufLen, CHECKSUM iType);

#endif