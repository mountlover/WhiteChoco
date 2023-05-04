/**
 * Interface for a Scanner
 */
#define MAX_TOKEN_LENGTH 64 // Max length of token string
#define MAX_DELIMITERS 60 // Max amount of delimiters
#include <stdio.h>

// File to scan
FILE* Scanner_file;
// Flag for case of tokens to be returned -- 1 for uppercase, 2 for lowercase, 0 for default
int Scanner_case;
// String to contain token
char Scanner_token[MAX_TOKEN_LENGTH];
// Token index;
int Scanner_tokenIndex;
// Temporarily holds the first char of the next token, EOF if the end of file was reached
char Scanner_temp;
// 1 when the Scanner has a file
int Scanner_ready;
// Delimiter string
char Scanner_delimiters[MAX_DELIMITERS];
// Delimiter string index
int Scanner_delimiterIndex;
// Keeps track of lines.
int Scanner_lineCount;
// 1 if Scanner should increment upon next call of next()
int Scanner_increment;
// Tells nextLine() not to function once between natural line skips
int Scanner_delay;

/*
 * Remove all delimiters and read whole lines at a time.
 */
void Scanner_removeDelimiters();
/*
 * Add a delimiter to the scanner. Nonzero if delimiter string is full.
 */
int Scanner_addDelimiter(char delimiter);
/*
 * Gives the Scanner a file to scan.
 */
void Scanner_init(FILE* inFile);
/*
 * Changes Scanner_case to 1.
 */
void Scanner_toUpperCase();
/*
 * Changes Scanner_case to 2.
 */
void Scanner_toLowerCase();
/*
 * Changes Scanner_case to 0.
 */
void Scanner_toDefaultCase();
/*
 * Returns 1 if Scanner has another token, 0 if not.
 */
int Scanner_hasNext();
/*
 * Assigns the next token (if it exists), nonzero on failure.
 */
int Scanner_next();
/*
 * Skips current line and moves on to next one. Nonzero if end of file is reached.
 */
int Scanner_nextLine();
