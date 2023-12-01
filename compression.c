#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rle.h"
#include "huffman.h"

void compression(void *message, long lBigBufSize)
{
	char choice; //for input
	char c;
	int buffersize = 0; //size of buffer before compression
	int compsize = 0; //size of compressed buffer (after)

	unsigned char * out, * buffer = NULL; //*in is the input buffer, *out is the output buffer, *buffer is the text size
	

	printf("	Please choose the compression type:\n\n");
	printf("		(1) RLE\n		(2) Huffman\n\n");
	printf("	Choice:");
	scanf_s("%c", &choice, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}

	//in is an array that contains all three buffers: in, buffer, and out.

	buffer = (unsigned char*)message + lBigBufSize; // the buffer starts at the end of the file size
	out = &buffer[buffersize]; //the output starts at the end of the buffer

	switch (choice)
	{
	case '1':
		compsize = RLE_Compress(message, message, lBigBufSize);
		//put the compressed file into an output file on your computer(function)
		RLE_Uncompress(buffer, out, compsize);
		break;
	case '2':
		compsize = Huffman_Compress(message, message, lBigBufSize);
		break;
	default:
		/* Should never happen... */
		compsize = 0;
		break;
	}
	
}