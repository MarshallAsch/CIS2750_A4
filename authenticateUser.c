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


#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "stream.h"
#include "mystring.h"
#include "myboolean.h"

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
int main(int argc, char const *argv[]) {
	char* userID;
	char* streamName;
	int numRead;

	FILE* errorStream;

	/* make sure the correct arguments are given */
	if (argc != 3) {
		printf("Incorrect usage. ./auth \"userID\" \"stream name\"\n");
		return -1;
	}

	errorStream  = freopen("error.log", "a", stderr);
	userID = strduplicate(argv[1]);
	streamName = strduplicate(argv[2]);

	/* authenticate user in the stream */
	if (strlen(userID) != 0 && strlen(streamName) != 0) {
		numRead = checkPermissions(streamName, userID);

		if (numRead >= 0) {
			/* then user is authenticated */
			fprintf(stderr, "authenticated %s \n", userID);
			printf("true\n");
			free(userID);
			free(streamName);
			fclose(errorStream);
			return numRead;
		} else {
			fprintf(stderr, "authentication denied: %s \n", userID);
			printf("false\n");
			free(userID);
			free(streamName);
			fclose(errorStream);
			return -2;
		}
	}

	/* userID is given load the streams */
	if (strlen(userID) != 0 && strlen(streamName) == 0) {
		loadStreams(userID);
		free(userID);
		free(streamName);
		fclose(errorStream);
		return -3;
	}

	fprintf(stderr, "userID and streamName not given.\n");
	free(userID);
	free(streamName);
	fclose(errorStream);
	return -1;
}

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
void loadStreams(char* userID) {
	/* make a seperate prossess for running python */
	pid_t  pid;
	char** args;
	char* fileName;

	/* allocate mem for the arguments */
	args = malloc(sizeof(char*) * 3);
	if (args == NULL) {
		fprintf(stderr, "failled to allocate memory.\n");
		return;
	}

	fileName = strduplicate("./loadStreams.py");
	args[0] = fileName;
	args[1] = userID;
	args[2] = NULL;

	pid  = fork();

	/* if failed */
	if (pid < 0) {
		fprintf(stderr, "failed to fork()\n");
		return;
	} else if (pid == 0) {		/* if child */
		execvp(fileName, args);	/* make system call */

		exit(0);
	} else {				/* if parent proccess */
		int status = 0;

		/* wait for child to finish then continue */
		waitpid(pid, &status, 0);

		free(fileName);
		free(args);
		/* check id something went wrong */
		if (status != 0) {
			fprintf(stderr, "Something went wrong, status code = %d\n", status);
		}
	}
}






