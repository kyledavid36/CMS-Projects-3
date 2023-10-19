/* Interface: Header file for File IO for getting messages from a file randomly
*  By: Michael Galle
*  Modified by: myStudentName
*/

#pragma once

#define MAX_QUOTE_LENGTH 140

//Function Prototypes (helper functions to GetMessageFromFile)

// These functions will produce the inputs to the GetMessageFromFile() function
int frandNum(int min, int max);							// Function gets a random number between min and max (max is the number of quotes in the file)
int fnumQuotes(void);									// Function returns number of quotes in the file (only need to run once)
long int* fquoteIndices(int numQuotes);					  // Function returns an array that indicates the start of every quote in the file (number of characters from the start of the file) 
int* fquoteLength(int numQuotes, long int* quoteIndices); // Function returns the smaller of the actual quote length or MAX_QUOTE_LENGTH


// Function that gets a random quote from the FortuneCookies.txt 
// int iLen the quote length, the quote is cut off at 140
// randNum is the quote number 
// numQuotes is the total number of quotes
// quoteLengths points to an array which has all of the quote lengths in it

int GetMessageFromFile(char* buff, int iLen, int randNum, int numQuotes, long int* quoteIndices, int* quoteLengths);  // buff is a character array (need top pass the node element that is a character array) 