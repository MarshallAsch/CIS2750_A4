/****************************** stream.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: February 17, 2017                    	 Course Name: CIS*2750
Assignment: A2

Contains all the functions for writing to creating and modifying the streams


****************************************************************************/

#ifndef __STREAM_LIB__
#define __STREAM_LIB__

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

#include "mystring.h"
#include "input.h"


struct userPost {
	char *username;
	char *streamname;
	char *date;
	char *text;
};

/**
 * destroyPost
 * Frees all the memory associated with the struct.
 * IN: 	post, a pointer to the struct that is being freed
 * OUT: none
 * POST: all the memory is freed
 * ERROR: none
 */
void destroyPost(struct userPost * post);



/**
 * updateStream
 * Saves the post to the file. If the stream does not exist
 * then it is created and the post is saved even if the user
 * does not have permission.
 * IN: 	st, the struct containing the data to be posted
 * OUT: none
 * POST: files are created if nesisay, the files are modified
 * ERROR: returns if the parameters are invalid
 */
void updateStream(struct userPost *st);

/**
 * addUser
 * Adds the user with username to all of the files in the
 * comma seperated list. If the stream does not exist then
 * the files for the stream  are created.
 * If the user is already in the stream then a messsage is printed
 * IN: 	username, the name of the user to be added
 *		list, a string of streams to add the user to
 * OUT: none
 * POST: files are created if nesisay, the files are modified
 * ERROR: returns if the parameters are invalid
 */
void addUser(char *username, char *list);

/**
 * removeUser
 * Removes the user with username to all of the files
 * in the comma seperated list if the user is already
 * in that stream. If the stream does not exist then
 * the files for the stream  are NOT created and the
 * user is notified. If the user is already in the
 * stream then a messsage is printed.
 *
 * IN: 	username, the name of the user to be removed
 *		list, a string of streams to remove the user from
 * OUT: none
 * POST: the files are modified
 * ERROR: returns if the parameters are invalid
 */
void removeUser(char *username, char *list);

/**
 * parseList
 * Takes in a comma seperates list if tokens and it gives back
 * a liked list if individual tokens, normalized to lowercase
 * and with leadind and trailing whitespace removed.
 * the list string needs to be freed by the caller.
 *
 * IN:	list, the string to be tokenized
 * OUT:	a linked list if the tokens on success
 *		NULL on failure
 * POST: a linked list structure is created
 * ERROR: NULL if the list is invalid
 */
StringList* parseList(char* list);


#endif


