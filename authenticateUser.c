/****************************** authenticateUser.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: March 14, 2017                    	 Course Name: CIS*2750
Assignment: A3

Checks the given userID to see if it is valid
Can also check if the userID has permission in the given stream
Also can get all the list of streams that a user can access

Note: when passing in a userID or a stream name any " character must be
escaped

this program will write to a file called error.log with the authentication
attempts and the result.

****************************************************************************/


#include "dbAccess.h"
#include "stream.h"
#include "mystring.h"

#include <stdlib.h>
#include <stdio.h>


/**
 * loadStreams
 * Creates a child process to run the Python script
 * that will load the list of streams that the user
 * has permission in.
 * will out put 1 on each line on success
 *
 * IN:	userID, the user ID it is checking
 * OUT: none
 * POST: prints the streams the user has permission in
 * ERROR: none
 */
void loadStreams(char* userID);

/**
 * main entry point to the program
 *
 * return values:
 * 		-1 if args invalid
 * 		>= 0 then the user is authenticated
 * 		-2 invalid user
 * 		-3 then the get the stream names
 */
int main(int argc, char const *argv[])
{
	char* userID;
	char* streamName;
	int numRead;
	int status;

	MYSQL* mysql;

	FILE* errorStream;

	/* make sure the correct arguments are given */
	if (argc != 3)
	{
		printf("Incorrect usage. ./auth \"userID\" \"stream name\"\n");
		return -1;
	}


	/* connect to the database */
	mysql = initSQL();
	if (mysql == NULL)
	{
		return -2;
	}

	/* open  the file for errors */
	errorStream  = freopen("error.log", "a", stderr);
	userID = strduplicate(argv[1]);
	streamName = strduplicate(argv[2]);

	/* authenticate user in the stream */
	if (strlen(userID) != 0 && strlen(streamName) != 0)
	{
		numRead = getNumRead_DB(mysql, streamName, userID);

		/* then user is authenticated */
		if (numRead >= 0)
		{
			fprintf(stderr, "authenticated %s \n", userID);
			printf("true\n");
			status = numRead;
		}
		else
		{
			fprintf(stderr, "authentication denied: %s \n", userID);
			printf("false\n");

			status = -2;
		}
	}
	else if (strlen(userID) != 0 && strlen(streamName) == 0)
	{
		/* userID is given load the streams */
		loadStreams(userID);
		status = -3;
	}
	else
	{
		status = -1;
	}

	fprintf(stderr, "userID and streamName not given.\n");
	free(userID);
	free(streamName);
	fclose(errorStream);
	mysql_close(mysql);
	return status;
}

/**
 * loadStreams
 * Prints all teh stream names teh user has permission in
 * will out put 1 on each line on success
 *
 * IN:	userID, the user ID it is checking
 * OUT: none
 * POST: prints the streams the user has permission in
 * ERROR: none
 */
void loadStreams(char* userID)
{
	SQL_result* result;
	MYSQL* mysql;
	int i;

	mysql = initSQL();

	result = getUserStreams_DB(mysql, userID);

	/* print all the users streams */
	for (i = 0; i < result->numRows; i++)
	{
		printf("%s\n", ((SQL_users_result*)(result->data[i]))->stream_name);
		freeUserResults((SQL_users_result*)(result->data[i]));
	}

	free(result->data);
	free(result);
	mysql_close(mysql);
}






