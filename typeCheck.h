/****************************** typecheck.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Contains the 2 function headers nessisary to check if a token is a
varaible type

****************************************************************************/

#ifndef __TYPE_CHECK_H__
#define __TYPE_CHECK_H__

#include "tokenStruct.h"
#include "mystring.h"

/**
 * isType
 * Checks wether or not the token is a type keyword,
 * if it is then join all the following keywords together.
 *
 * IN:	token, the first token that might be a type
 * OUT: a string that contains the first letter of each type keyword
 *		NULL, on failure
 * POST: all the tokens that are part of the type are joined together
 * ERROR: NULL if the token does not exist
 */
char* isType(Keyword* token);

/**
 * isDupList
 * Checks wether ot not the type string is alreaddy in the list or not.
 *
 * IN: 	type_list, the list of the types
 *		type, the current string that is being checked
 *		top, the number of elements that are currently in the list
 * OUT: TRUE, if the string is in the list
 *		FALSE, if it is not in the list
 * POST: none
 * ERROR: FALSE, on error
 */
bool isDupList(char type_list[200000][2][200], char* type, int top);

#endif
