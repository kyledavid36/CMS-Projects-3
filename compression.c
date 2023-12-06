#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rle.h"
#include "huffman.h"

void compression(void *message, int insize, unsigned char* buf, long* compsize)
{
	char choice; //for input
	char c;
	
	unsigned char* in = (unsigned char*)message;

	printf("	Please choose the compression type:\n\n");
	printf("		(1) RLE\n		(2) Huffman\n\n");
	printf("	Choice:");
	scanf_s("%c", &choice, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}

	

	
	switch (choice)
	{
	case '1':
		*compsize = RLE_Compress((unsigned char*)message, buf, insize);
		break;
	case '2':
		
		*compsize = Huffman_Compress((unsigned char*)message, buf, insize);
		break;
	default:
		/* Should never happen... */
		*compsize = 0;
		break;
	}
}