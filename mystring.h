/****************************** myString.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Version 2

Part of the library of functions that contain the functions to deal
with strings
****************************************************************************/

#ifndef __MY_STRING_H__
#define __MY_STRING_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "myboolean.h"


typedef char* string;

/**
 * A linked list structure for strings
 *
 */
typedef struct StringList
{
	char* str;
	struct StringList * next;
} StringList;



/**
 * toLower
 * Takes a string as input and returns a new string that is a copy
 * of the origonal but is in lowercase. if overwrite is set to TRUE (1)
 * then the origonal is freed, otherwise the caller mist free the string.
 *
 * IN:	str, the sring that it is going to make lowercase
 *		overwrite, wether or not the orgional gets destroyed
 * OUT: pointer to the new string on success
 *		NULL on failure
 * POST: a new string is created in memory
 * ERROR: NULL if the parameter is invalid or mem could not be allocated
 */
char* toLower(char* str, bool overwrite);

/**
 * join
 * Joins 2 strings together. src is put after
 * dest, src needs to be freed by the caller.
 * dest is destroyed.
 *
 * IN:	dest, the string that is being added to
 *		src, the string that is being added
 * OUT: NULL, on failure
 *		pointer to the new string on success
 * POST: the old dest gets freed and the new string is created
 * ERROR: NULL if something goes wrong.
 */
char* join(char* dest, const char* src);

/**
 * joinC
 * Joins a character to the end of the string
 *
 * IN:	dest, the string that is being added to
 *		src, the character that is being added
 * OUT: a new string that contains both the old and the new
 * POST: makes a string
 * ERROR: NULL if something goes wrong
 */
char* joinC(char* dest, char src);

/**
 * strduplicate
 * Copies a sting into a new string
 *
 * IN: 	str, the sting to duplicate
 * OUT: NULL, on failure
 *		returns a pointer to the new string
 * POST: creates a new string in memory
 * ERROR: NULL if mem can not be allocated
 */
char* strduplicate(const char* str);

/**
 * substring
 * Creates a new string from the old string.
 * ranges from start inclusive to end inclusive.
 *
 * IN: 	input, the base string
 *		start, the start index
 *		end, the end index of the substring
 * OUT: a pointer to the new string on success
 *		NULL pointer on failure
 * POST: a new smaller string is created
 * ERROR: NULL if parameters are not valid or
 *		  mem can not be allocated
 */
char* substring(char* input, int start, int end);

/**
 * lastIndex
 * Gets the last index of the given character in the string.
 *
 * IN:	str, the string that is being searched
 *		ch, the char that it is searching for
 * OUT: returns the index of last time the character apears
 *		-1 on failure or if the characer is not in the string
 * POST: none
 * ERROR: -1 if the string does not exist
 */
int lastIndex(char* str, char ch);

/**
 * firstIndex
 * Gets the index of the first occurance of the given
 * character in the string.
 *
 * IN:	str, the string that is being searched
 *		ch, the char that it is searching for
 * OUT: returns the index of first time the character apears
 *		-1 on failure or if the characer is not in the string
 * POST: none
 * ERROR: -1 if the string does not exist
 */
int firstIndex(char* str, char ch);

/**
 * firstIndexOffset
 * Gets the index of the first occurance of the given
 * character in the string after the offset index
 *
 * IN:	str, the string that is being searched
 *		ch, the char that it is searching for
 *		offset, how far into the string to start the search
 * OUT: returns the index of first time the character apears
 *		-1 on failure or if the characer is not in the string
 * POST: none
 * ERROR: -1 if the string does not exist
 */
int firstIndexOffset(char* str, char ch, int offset);

/**
 * trim
 * Removes any leading or trailing white space characters
 * Makes a copy and destroyes the origonal.
 *
 * IN: 	input, the string that is being trimmed
 * OUT: NULL on failure
 *      a pointer the the new string that has been trimmed
 * POST: origonal memory is freed, new memory is allocated
 * ERROR: if input does not exist returns NULL
 */
char* trim(char* input);

/**
 * strcmp_nocase
 * Does a case insensitive string comparison.
 * If ether paramater is NULL then it segfaults.
 *
 *	if Return value < 0 then it indicates str1 is less than str2.
 *	if Return value > 0 then it indicates str2 is less than str1.
 *	if Return value = 0 then it indicates str1 is equal to str2.
 *
 * IN: 	str1, This is the first string to be compared.
 *		str2, This is the second string to be compared.
 * OUT: number
 * POST: none
 * ERROR: SEGFAULT on error
 */
int strcmp_nocase(const char* str1, const char* str2);

/**********************************
 *
 *		FOR THE LIST STRUCTURE
 *
 **********************************/

/**
 * newStringList
 * Creates a new string list element from the given string.
 * str is copied and needs to be freed by the caller
 *
 * IN: 	str, the string of the element
 * OUT:	a pointer the the new string list element
 * 		NULL on failure
 * POST: new string list is created
 * ERROR: if malloc fails return NULL
 */
StringList * newStringList(char* str);

/**
 * addToStringList
 * Adds the new element to the head of the list
 *
 * IN: 	list, the list it is being added too
 *		new, the element eing added to the list
 * OUT:	the new head of the list
 * POST: element is added to the head of the list
 * ERROR: none
 */
StringList * addToStringList(StringList* list, StringList * new);

/**
 * addToStringListEnd
 * Adds the new element to the tail of the list
 *
 * IN: 	list, the list it is being added too
 *		new, the element eing added to the list
 * OUT:	the new head of the list
 * POST: element is added to the tail of the list
 * ERROR: none
 */
StringList * addToStringListEnd(StringList* list, StringList * new);

/**
 * destroyStringList
 * Adds the new element to the head of the list
 *
 * IN: 	stream, the list to be freed
 * OUT:	none
 * POST: all the memory used is freed
 * ERROR: none
 */
void destroyStringList(StringList * stream);


/**********************************
 *
 *			NOT ALLWAYS USED
 *
 **********************************/


/**
 * createFuncPtr
 * Creates the function pointer to go in the class
 *
 * IN:	funcName, the new name of the function
 *		typeList, the list of the perameter types of the function
 *		className, the name of the class it is a part of
 *		funcType, the return type of the function
 *		needsOBJ, boolean weather it needs the class OBJ to be added as a param
 * OUT: returns the string of the function pointer to be added on success
 *		NULL on failure
 * POST: creates a string
 * ERROR: NULL if something goes wrong
 */
char* createFuncPtr(char* funcName, char* typeList, char* className, char* funcType, bool needsOBJ);

/**
 * genOutName
 * Creates the name of the file to print to from the input file.
 *
 * Takes a "path/path/filename.cc" in and returns "filename.c" out
 * IN:	fileName, the name of the file it is reading from
 * OUT: returns a new string with the filename on success
 *		NULL on failure
 * POST: creates a new string in memory
 * ERROR: NULL if mem can not be allocates
 *		  or if input is invalid
 */
char* genOutName(char* fileName);




char* unWrapQuote(char* str);
char* wrapQuote(char* str);
bool startEndQuote(char* str);


#endif
