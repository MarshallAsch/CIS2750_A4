
/****************************** input.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: February 17, 2017                   	 Course Name: CIS*2750
Assignment: A2

This file is responicble for geting and formating the users input
and reading from files

****************************************************************************/

#include "input.h"


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
int getInt() {
	char* buffer;
	int  option;

	/* get the string input and convert it to an integer */
	buffer = trim(fgetstr(stdin));
	option = atoi(buffer);

	free(buffer);

	return option;
}

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
int fgetInt(FILE* stream) {
	char* buffer;
	int  num;

	/* get the string input and convert it to an integer */
	buffer = trim(fgetstr(stream));
	num = atoi(buffer);

	free(buffer);

	return num;
}

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
char* fgetstr(FILE* stream) {
	char buffer[BUFFSIZE + 2];
	char* input;
	char temp;
	int pos;
	int first;

	input = NULL;
	pos = 0;
	first = 0;

	/* get input untill EOF or /n */
	while ((temp = fgetc(stream))) {
		/* if the user entered ctrl-D as the first character */
		if (temp == EOF && first == 0) {
			free(input);
			return NULL;
		}

		/* then flush the buffer */
		if (pos + 1 > BUFFSIZE) {
			input = join (input, buffer);
			pos = 0;
		}

		buffer[pos++] = temp;
		buffer[pos] = '\0';

		/* if the user entered enter then end the string */
		if (temp == '\n') {
			input = join (input, buffer);
			break;
		}
		first++;
	}

	return input;
}

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
char* getstr() {
	return fgetstr(stdin);
}
