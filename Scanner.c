/*
 * Scanner - a Scanner to read through a file that uses spaces, line feeds and
 * carriage returns as delimiters by default.
 * Version: 12.04.2008
 * Author: Yama H
 */
#include "Scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

/*
 * Gives the Scanner a file to scan.
 */
void Scanner_init(FILE* inFile)
{
	Scanner_file = inFile;
	Scanner_delay = 0;
	Scanner_increment = 0;
	Scanner_case = 0;
	Scanner_lineCount = 1;
	// Assign default delimiters
	Scanner_delimiterIndex = 3;
	Scanner_delimiters[0] = ' ';
	Scanner_delimiters[1] = 9;	// horizontal tab
	Scanner_delimiters[2] = 0;	// nul
	strcpy(Scanner_token, "");
	Scanner_temp = ' ';
	while(strchr(Scanner_delimiters, Scanner_temp) != NULL ||
			Scanner_temp == 10 || Scanner_temp == 13)
	{
		Scanner_temp = fgetc(Scanner_file);
		if(Scanner_temp == 10)
			Scanner_lineCount++;
	}
	if(feof(Scanner_file) || ferror(Scanner_file))
		Scanner_ready = 0;
	else
		Scanner_ready = 1;
}

/*
 * Remove all delimiters and read whole lines at a time.
 */
void Scanner_removeDelimiters()
{
	strcpy(Scanner_delimiters, "");
	Scanner_delimiterIndex = 0;
}

/*
 * Add a delimiter to the scanner. Nonzero if delimiter string is full.
 */
int Scanner_addDelimiter(char delimiter)
{
	if(Scanner_delimiterIndex+1 < MAX_DELIMITERS)
	{
		strcat(Scanner_delimiters, &delimiter);
		Scanner_delimiterIndex++;
		return 0;
	}
	return 1;
}

/*
 * Returns 1 if Scanner has another token, 0 if not.
 */
int Scanner_hasNext()
{
	return Scanner_ready;
}

/*
 * Changes Scanner_case to 1.
 */
void Scanner_toUpperCase()
{
	Scanner_case = 1;
}

/*
 * Changes Scanner_case to 2.
 */
void Scanner_toLowerCase()
{
	Scanner_case = 2;
}

/*
 * Changes Scanner_case to 0.
 */
void Scanner_toDefaultCase()
{
	Scanner_case = 0;
}
/*
 * Skips current line and moves on to next one. Nonzero if end of file is reached.
 */
int Scanner_nextLine()
{
	if(Scanner_increment == 1 && Scanner_delay == 0)
	{
		Scanner_delay = 1;
		return 0;
	}
	while(Scanner_temp == 10 || Scanner_temp == 13)
	{
		Scanner_temp = fgetc(Scanner_file);
	}
	while(Scanner_temp != 10 && Scanner_temp != 13 && Scanner_temp != EOF)
	{
		Scanner_temp = fgetc(Scanner_file);
	}
	while((strchr(Scanner_delimiters, Scanner_temp) != NULL ||
			Scanner_temp == 10 || Scanner_temp == 13) && Scanner_temp != EOF)
	{
		Scanner_temp = fgetc(Scanner_file);
	}
	if(Scanner_temp == EOF)
	{
		Scanner_ready = 0;
		return 1;
	}
	Scanner_lineCount++;
	return 0;
}
/*
 * Assigns the next token (if it exists), nonzero on failure.
 */
int Scanner_next()
{
	if(Scanner_ready == 0)
		return 1;
	if(Scanner_increment)
	{
		Scanner_lineCount++;
		Scanner_increment = 0;
	}
	Scanner_delay = 0;
	Scanner_tokenIndex = 0;
	strcpy(Scanner_token, "");
	while(strchr(Scanner_delimiters, Scanner_temp) == NULL &&
			Scanner_temp != 10 && Scanner_temp != 13)
	{
		if(Scanner_case == 1)
			Scanner_token[Scanner_tokenIndex++] = toupper(Scanner_temp);
		else if(Scanner_case == 2)
			Scanner_token[Scanner_tokenIndex++] = tolower(Scanner_temp);
		else
			Scanner_token[Scanner_tokenIndex++] = Scanner_temp;
		if(Scanner_tokenIndex == MAX_TOKEN_LENGTH)
			return 2;
		Scanner_temp = fgetc(Scanner_file);
		if(feof(Scanner_file))
		{
			Scanner_ready = 0;
			Scanner_token[Scanner_tokenIndex++] = '\0';
			return 0;
		}
		if(ferror(Scanner_file))
		{
			Scanner_ready = 0;
			Scanner_token[Scanner_tokenIndex++] = '\0';
			return 3;
		}
	}
	while(strchr(Scanner_delimiters, Scanner_temp) != NULL ||
			Scanner_temp == 10 || Scanner_temp == 13)
	{
		if(Scanner_temp == 10)
			Scanner_increment = 1;
		Scanner_temp = fgetc(Scanner_file);
	}
	if(feof(Scanner_file))
	{
		Scanner_ready = 0;
		Scanner_token[Scanner_tokenIndex] = '\0';
		return 0;
	}
	if(ferror(Scanner_file))
	{
		Scanner_ready = 0;
		Scanner_token[Scanner_tokenIndex] = '\0';
		return 3;
	}
	Scanner_token[Scanner_tokenIndex] = '\0';
	return 0;
}
