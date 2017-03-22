/****************************** token.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Contains all the functions to modify, create and display the tokens.

The tokens hold 2 strings and point to the next string in the list.
The token also keeps track of how long its strings are.
****************************************************************************/

#ifndef __TOKEN_STRUCT_H__
#define __TOKEN_STRUCT_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "myboolean.h"

/**
 * 	length that gets added to the strings to make
 *  them longer if they are too small
 */
#ifndef START_LEN
#define START_LEN 10
#endif


/**
 * The struct to hold the tokens
 *
 */
typedef struct Keyword {
	char* token;
	char* whitespace;		/* the whitespace from the end of the token to the start of the next */

	int token_size;
	int white_size;

	struct Keyword* next;
} Keyword;

/***********************************************
*
*
*			CREATE DESTOY INSERT
*
*
*************************************************/


/**
 * newToken
 * Create and allocate memory for a new Keyword.
 * allocates an initial ammount of memory for both
 * the whitespace and the token strings, that is
 * the size of the macro START_LEN (10 characters)
 *
 * IN:	none
 * OUT: pointer to the location in memory of the Keyword struct
 *		NULL, on failure
 * POST: allocates memory for the keyword and for its 2 strings
 * ERROR: NULL if memory could not be allocated
 */
Keyword* newToken();

/**
 * freeToken
 * Recursivly free all the memory associated with the Keyword.
 * the 2 strings and the nect token then itself.
 *
 * IN: token, the head of the list of tokens to be freed
 * OUT: none
 * POST: all the memory that is used by this list is freed
 * ERROR: none
 */
void freeToken(Keyword* token);

/**
 * addToken
 * Adds a token that was created after the first token
 * used when adding a new token to the list when it is getting created
 *
 * IN:	token_old, the token that is before the one that is being added
 *		token_new, the new token that is being added
 * OUT: returns a pointer to the keyword
 * POST: inserts the new token after the old token.
 * ERROR: none
 */
Keyword* addToken(Keyword* token_old, Keyword* token_new);

/**
 * insertToken
 * Adds a token that was created after the token
 * used when a token is eing inserted into the middle of the list
 *
 * IN:	token_old, the token that is before the one that is being added
 *		token_new, the new token that is being added
 * OUT: returns a pointer to the keyword
 * POST: inserts the new token after the old token.
 * ERROR: none
 */
int insertToken(Keyword* first, Keyword* new);


/***********************************************
*
*
*			WRITE TO THE TOKEN
*
*
*************************************************/

/**
 * printToken_str
 * Prints a string of characters to the Keywords
 * token string.
 * msg needs to be freed by the caller
 *
 * IN:	keyword, the struct that it is being added to
 *		msg, the string that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the new string is added to the end of the old string
 * ERROR: -1, or -2 if an error occurs
 */
int printToken_str(Keyword* keyword, char* msg);

/**
 * printWhite_str
 * Prints a string of characters to the Keywords
 * whitespace string.
 * msg needs to be freed by the caller
 *
 * IN:	keyword, the struct that it is being added to
 *		msg, the string that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the new string is added to the end of the old string
 * ERROR: -1, or -2 if an error occurs
 */
int printWhite_str(Keyword* keyword, char* msg);

/**
 * printToken_char
 * Prints a single character to the Keywords tokeb string
 *
 * IN:	keyword, the struct that it is being added to
 *		next, the character that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the character is added to the end of the string
 * ERROR: -1, or -2 if an error occurs
 */
int printToken_char(Keyword* keyword, char next);

/**
 * printWhite_char
 * Prints a single character to the Keywords whitespace string
 *
 * IN:	keyword, the struct that it is being added to
 *		next, the character that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the character is added to the end of the string
 * ERROR: -1, or -2 if an error occurs
 */
int printWhite_char(Keyword* keyword, char next);

/**
 * changeToken
 * Replace the keywords token string with the new string.
 * the string str needs to be freed by the caller
 *
 * IN:	keyword, the word that the token is being replaced on
 *		str, the new string that is going into the token
 * OUT: -1, if a parameter is invalid
 *		-2, if memory could not be allocated
 *		 0, on success
 * POST: the old token is freeded and replaced
 * ERROR: -1, or -2
 */
int changeToken(Keyword* keyword, char* str);


/***********************************************
*
*
*					PRINT
*
*
*************************************************/

/**
 * printTokensOnly
 * Recursivly print all the tokens onto a new line
 * used for testing
 *
 * IN:	output, the stream for where to print the output (stdout or a file)
 *		head, the head of the list of tokens to print
 * OUT: none
 * POST: tokens get printed 1 per line
 * ERROR: none
 */
void printTokensOnly(FILE* output, Keyword* head);

/**
 * printWhiteOnly
 * Recursivly print all the whitespace tokens onto a new line
 * used for testing
 *
 * IN:	output, the stream for where to print the output (stdout or a file)
 *		head, the head of the list of tokens to print
 * OUT: none
 * POST: tokens get printed 1 per line
 * ERROR: none
 */
void printWhiteOnly(FILE* output, Keyword* head);

/**
 * printTokensAll
 * Recursivly print all the tokens.
 * Prints the token then the whitespace.
 *
 * IN:	output, the stream for where to print the output (stdout or a file)
 *		head, the head of the list of tokens to print
 * OUT: none
 * POST: tokens get printed, seperated by the whitespace tokens
 * ERROR: none
 */
void printTokensAll(FILE* output, Keyword* head);


/***********************************************
*
*
*					OTHER
*
*
*************************************************/


/**
 * concat
 * Joins 2 tokens together, the tokens are seperated by a space.
 * the seconds tokens whitespace is moved to the first.
 *
 * IN:	start, the first token that is being joined.
 * OUT: TRUE, on success
 *		FALSE, on failure
 * POST: the second token is freed and there strings are joined together
 * ERROR: FALSE, if ether token does not exist
 */
bool concat(Keyword* start);


#endif
