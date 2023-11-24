#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rle.h"
#include "huffman.h"
#include "buffer.h"

int compression()
{

	char Buffer1[] = "AbcdefgHiJkLmnOpQ"; //does not compress
	char Buffer2[] = "AAAAAABBBBBBCCCCC"; //compresses well
	char choice; //for input
	char c;
	char filename[] = "FortuneCookies.txt";
	char* inputfilename = (char *)malloc(50 * sizeof(char));
	long long int filesize = 0; //output from GetFileSize()
	int buffersize = 0; //size of buffer before compression
	int compsize = 0; //size of compressed buffer (after)

	unsigned char* in, * out, * buffer = NULL; //*in is the input buffer, *out is the output buffer, *buffer is the text size

	FILE* fp;
	FILE* compress;


	//Assignment Title
	printf("Assignment 4\n\nBy: Amy Wentzell and Kyle Dick\n\n2023\n\n\n\n");
	printf("\nPlease input a file name for compressing:\n");
	scanf_s("%s", inputfilename, 50);
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	printf("%s\n", inputfilename);

	printf("	Please choose the compression type:\n\n");
	printf("		(1) RLE\n		(2) Huffman\n\n");
	printf("	Choice:");
	scanf_s("%c", &choice, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}

	fopen_s(&fp, inputfilename, "rb");
	if (fp == NULL)
	{
		printf("\n\nERROR: program could not open file.\n\n");
		return(0);
	}
	else
	{
		filesize = GetFileSize(fp);

		if (filesize < 1)
		{
			printf("\n\nERROR: empty file.\n\n");
			fclose(fp);
		}
		else
		{
			buffersize = (filesize * 104 + 50) / 100 + 384; //defined in buffer.c, basically to account for the worst case scenario
			//in is an array that contains all three buffers: in, buffer, and out.
			in = (unsigned char*)malloc(filesize + 2*buffersize); //if nothing compresses, output size will be larger than the original buffer

			buffer = &in[filesize]; // the buffer starts at the end of the file size
			out = &buffer[buffersize]; //the output starts at the end of the buffer
			fread(in, 1, filesize, fp); //read the file and copy it to the buffer
			fclose(fp);
			switch (choice)
			{
			case '1':
				compsize = RLE_Compress(in, buffer, filesize);
				//put the compressed file into an output file on your computer(function)
				RLE_Uncompress(buffer, out, compsize);
				break;
			case '2':
				compsize = Huffman_Compress(in, buffer, filesize);
				fopen_s(&compress, "whiteNoiseC.dat", "wb");
				if (compress != NULL)
				{
					fwrite(out, sizeof(char), compsize , compress);
				}
				fclose(compress);
				//put the compressed file into an output file on your computer(function)
				Huffman_Uncompress(buffer, out, compsize, filesize);
				break;
			default:
				/* Should never happen... */
				compsize = 0;
				break;
			}

			printf("\n	Compression Ratio: %d/%d bytes (%.1f%%)\n", compsize, filesize, 100 * (float)compsize / (float)filesize );
			free(inputfilename);
			free(in);
			return(0);
		
		}
	}
}