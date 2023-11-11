/* Implementation: Functions for File IO - Getting random messages from a file
*  By: Michael Galle
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>			// Dynamic memory allocation for arrays that store quote location and length
#include <errno.h>
#include <time.h>
#include <windows.h>

#define MAX_QUOTE_LENGTH 140

// Function gets a random number between min and max (max is the number of quotes in the file)
int frandNum(int min, int max) {
	int temp = rand();
	temp = (temp % (max - min + 1)) + min;
	//printf("\nthe random number is here hehe: %d\n", temp);
	return (temp);
}

// Function returns number of quotes in the file (only need to run once)
int fnumQuotes(void) {
	FILE* fp;
	errno_t err;
	char c = NULL;
	char o = NULL;
	int counter = -1;

	if ((err = fopen_s(&fp, "FortuneCookies.txt", "r") == 0)) {
		printf("\nFile opened successfully\n");

		// Read file one character at a time
		while ((c = fgetc(fp)) != EOF) {
			if (c == '%' && o == '%') {
				counter++;
			}
			o = c;
		}
		//printf("There were %d quotes in the file", counter);
		fclose(fp);
		return(counter);
	}
	else {
		printf("\nError: Failed to open file\n");
		fclose(fp);
	}

}



// Function returns an array that indicates the start of every quote in the file (number of characters from the start of the file) 
long int* fquoteIndices(int numQuotes)
{
	FILE* fp;
	char c = NULL;
	char o = NULL;
	char a = NULL;
	long int counter = 0;
	long int r = 0;
	int tempIndex;
	long int* Indices;
	long int i = 0;
	//Indices = (long int*)malloc(numQuotes * sizeof(long int));

	if ((fopen_s(&fp, "FortuneCookies.txt", "r") == 0)) {
		printf("\nFile opened successfully\n");


		//Read file one character at a time
		for (int j = 0; counter < numQuotes; j++)
		{
			r = 0;

			for (i = 0; (c = fgetc(fp)) != EOF; i++)
			{
				if (c == '%' && o == '%')
				{
					Indices[counter++] = i + r + 1;
				}
				else if (c == '\n' || c == '\r') //r is the extra char when c = '\n'
				{
					r++;
				}

				o = c;
			}
		}
		//printf("There were %d quotes in the file", counter);
		fclose(fp);
		return(Indices);
	}
	else {
		printf("\nError: Failed to open file\n");
		fclose(fp);
	}


}

// Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH
int* fquoteLength(int numQuotes, long int* Indices)
{
	int* LengthMessage = NULL;
	//LengthMessage = (int*)malloc((numQuotes) * sizeof(int));
	char c;
	long int counter = 0;
	int check = 0;
	FILE* fp;

	if ((fopen_s(&fp, "FortuneCookies.txt", "r") == 0)) {

		printf("\nFile opened successfully\n");
		for (int i = 0; i < numQuotes; i++)
		{
			fseek(fp, Indices[i], SEEK_SET);

			check = 0;
			counter = 0;
			while (!check)
			{
				c = fgetc(fp);
				counter++;
				if (c == '%')
				{
					if (fgetc(fp) == '%')
					{
						check = 1;
					}
				}

			}

			LengthMessage[i] = counter - 1;
		}
		fclose(fp);
		for (int j = 0; j < numQuotes; j++)
		{
			if (LengthMessage[j] > MAX_QUOTE_LENGTH)
			{
				LengthMessage[j] = MAX_QUOTE_LENGTH;
			}

		}
	}
	else {
		printf("\nError: Failed to open file\n");
		fclose(fp);
	}
	return(LengthMessage);
}


// Function that gets q random quote from the FortuneCookies file 
int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long int* quoteIndices, int* quoteLengths) {

	FILE* fp;
	char* temp;
	temp = buff;

	if ((fopen_s(&fp, "FortuneCookies.txt", "r") == 0)) {
		printf("\nFile opened successfully\n");

		int qLoc = fseek(fp, quoteIndices[randNum], SEEK_SET); //qLoc is the location of the quote


		//printf("\nHELP ME! heLP. me. %ld\n", ftell(fp)); //Test file Location (fseek)

		for (int i = 0; i < quoteLengths[randNum]; i++)
		{
			*(temp++) = fgetc(fp);
		}

		*temp = '\0';

		fclose(fp);
	}
	else
	{
		printf("\nError: Failed to open file\n");
		fclose(fp);

	}

	return(0);
}