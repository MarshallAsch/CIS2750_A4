/****************************** input.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: February 17, 2017	                  	 Course Name: CIS*2750
Assignment: A2

This file is responicble for geting and formating the users input


****************************************************************************/


#ifndef __INPUT_H__
#define __INPUT_H__

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mystring.h"


/* the size of the input buffer when reading input */
#ifndef BUFFSIZE
#define BUFFSIZE 40
#endif

/**
 * getInt
 * Gets a integer value from the users
 *
 * IN:	none
 * OUT: the number that the user enetered on success
 *		0 if the number the entered is not a valid number
 * POST: blocks untill the user enters input and presses enter
 * ERROR: 0 if the input is not a number
 */
int getInt();

/**
 * fgetInt
 * Gets a integer value from the given stream
 *
 * IN:	stream, the stream that is being read from
 * OUT: the number that the user enetered on success
 *		0 if the number the entered is not a valid number
 * POST: blocks untill the user enters input and presses enter
 * ERROR: 0 if the input is not a number
 */
int fgetInt(FILE* stream);

/**
 * fgetstr
 * Gets buffered input from the given stream, untill the user
 * enteres new line
 *
 * IN: stream, the stream that is being read from
 * OUT: pointer to the string on success
 *		NULL if reached EOF (or ctrl-D is pressed)
 *      NULL on failure
 * POST: reads a string untill enter is pressed
 * ERROR: NULL if memory could not be allocated
 */
char* fgetstr(FILE* stream);

/**
 * getstr
 * Gets buffered input from stdin untill the user
 * enteres new line
 *
 * IN:	none
 * OUT: pointer to the string on success
 *		NULL if reached EOF (or ctrl-D is pressed)
 *      NULL on failure
 * POST: reads a string untill enter is pressed
 * ERROR: NULL if memory could not be allocated
 */
char* getstr();

#endif
