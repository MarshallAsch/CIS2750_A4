/****************************** removeauthor.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: March 14, 2017                    	 Course Name: CIS*2750
Assignment: A3

Mofified from A2 addauthor

This program will remove the given userID from the given list of streams

****************************************************************************/

#include "stream.h"
#include "mystring.h"
#include "myboolean.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Main enty point to the program
 * Takes in arguments and an optional
 *
 *
 */
int main(int argc, char const *argv[]) {
	char* userID;
	char* list;
	FILE* errorStream;

	errorStream  = freopen("error.log", "a", stderr);
	/* make sure that the arguments given are valid */
	if (argc != 3) {
		printf("Invalid usage: ./removeauthor \"userID\" \"stream list\"\n");
		fclose(errorStream);
		return -1;
	}

	userID = trim(strduplicate(argv[1]));
	list = trim(strduplicate(argv[2]));

	removeUser(userID, list);

	/* free the memory that this program uses */
	free(userID);
	free(list);
	fclose(errorStream);
	return 0;
}

