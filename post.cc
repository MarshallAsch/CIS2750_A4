/****************************** post.cc ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: Febuary 17, 2017                    	 Course Name: CIS*2750
Assignment: A2

Contains the functions wo create a new post.
To build this must be converted to to a .c using A1
****************************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "mystring.h"
#include "input.h"
#include "stream.h"


/**
 * PostEntry thte class that holds the the 
 * functions for loading the users input to 
 * create a post
 *
 */
class PostEntry {

	char * streamName;
	char * text;
	char * date;
	char * userID;
	struct userPost * post;

	/**
	 * readInput
	 * Prompts user to enter the name of the stream to post to and the text
	 * for the post. Saves the data to class variables. 
	 *
	 * IN:	none
	 * OUT:  0, if the input is aquired successflly
	 *		-1, if something goes wrong
	 * POST: The input is saved into the class
	 * ERROR: -1 if something goes wrong 
	 */
	int readInput(char* fileName)
	{
		FILE* file;
		long int size;


		/* make sure that the file name exists */
		if (fileName == NULL) {
			return -1;
		}

		/* try opening the file */
		file = fopen(fileName, "r");
		if (file == NULL) {
			return -2;
		}

		/* the the num bytes in the file */
		fseek(file, 0L, SEEK_END);
		size = ftell(file);
		fseek(file, 0L, SEEK_SET);

		/* allocate mem for the buffer*/
		text = malloc(size + sizeof(char)*4);
		if (text == NULL) {
			fclose(file);
			return -3;
		}

		/* read the file */
		memset(text, '\0', size+2);
		fread(text, size, 1, file);


		fclose(file);
		return 0;
	}

	/**
	 * formatEntry
	 * Creates a new userPost struct to post to the files
	 * creates a new userPost struct in memory,
	 * all of the parameters needs to be freed by the caller
	 * adds the struct to the class variable.
	 *
	 * IN:	none
	 * OUT: the userPost object on success
	 *		NULL on failure
	 * POST: creates the userPost object
	 * ERROR: NULL if something goes wrong
	 */
	struct userPost * formatEntry()
	{
		/* creates a struct used by updateStream()*/
		post = malloc(sizeof(struct userPost));

		/* make sure that malloc worked */
		if (post == NULL) {
			return NULL;
		}

		/* copy the data into the new struct */
		post->username = strduplicate(userID);
		post->streamname = strduplicate(streamName);
		post->date = strduplicate(date);
		post->text = strduplicate(text);

		return post;
	}

	/**
	 * getTimeDate
	 * Creates a time string in the format of 
	 *
	 * YYYY-MM-DD:HH:mm:ss
	 * MM  range, 01 to 12
	 * DD  range, 01 to 31
	 * HH  range, 00 to 23
	 * mm  range, 00 to 59
	 * ss  range, 00 to 59
	 *
	 * adds the string to the class varaible date 
	 *
	 * IN:	none
	 * OUT: the date string on success
	 *		NULL on failure
	 * POST: adds the datestring to the class
	 * ERROR: NULL if something goes wrong
	 */
	char* getTimeDate()
	{
		/* gets date time formats and returns it as a string*/
		time_t rawtime;
		struct tm * currentTime;
		
		time( &rawtime );

		currentTime = localtime( &rawtime );

		date = malloc(sizeof(char)*(21));

		/* make sure amlloc worked */
		if (date == NULL) {
			return NULL;
		}

		/* create the date string*/
		sprintf(date, "%04d-%02d-%02d:%02d:%02d:%02d", currentTime->tm_year + 1900, 
			currentTime->tm_mon + 1, 
			currentTime->tm_mday, 
			currentTime->tm_hour, 
			currentTime->tm_min, 
			currentTime->tm_sec);


		return date;
	}

	/**
	 * submitPost
	 * Make sure that they have permissions to write to that stream
	 * print an error message if something goes wrong.
	 * otherwise it will add the post to the file.
	 * If the files do not exist for the stream then create them
	 *
	 * IN:	none
	 * OUT: none
	 * POST: creates the stream files, and posts the message to it if it can
	 * ERROR: stopes early if something goes wrong
	 */
	void submitPost()
	{
		int status;

		status = checkPermissions(streamName, userID);

		/* check the result if the permission check */
		if (status == -1) 
		{
			fprintf(stderr, "Error\n");
			return;
		}
		else if (status == -2)
		{
			createFiles(streamName);
			fprintf(stderr, "Permission denied new empty stream [%s] created.\n", streamName);
			return;
		}
		else if (status == -3)
		{
			createFiles(streamName);
			fprintf(stderr, "Permission denied [%s] in [%s].\n", userID, streamName);
			return;
		}

		/* submit the post */
		updateStream(post);

		printf("Success!  Ending...\n");
	}

	/**
	 * destroy
	 * Frees all the memory associated with the object
	 *
	 * IN: 	none
	 * OUT: none
	 * POST: all the memory is freed
	 * ERROR: none
	 */
	void destroy()
	{
		free(streamName);
		free(text);
		free(date);
		free (userID);
		destroyPost(post);
	}

	/**
	 * init
	 * Initilizes all the class variables to NULL
	 * IN: 	none
	 * OUT: none
	 * POST: all the class variables are set to NULL
	 * ERROR: none
	 */
	void init()
	{
		streamName = NULL;
		text = NULL;
		date = NULL;
		post = NULL;
		userID = NULL;
	}
};

/**
 * main
 * The main entry point to the program. Creates a 
 * PostEntry object, initilizes it then creates a post
 * to submit.
 * Defauts to accepting multi work userID's
 * can be turned off using the NO_MULTI_WORD_ID macro
 *
 * IN: 	Takes command line argumnts of the userID
 * OUT: 0 on success
 *		1 if invalid arguments given
 *		(-1 if multi word ID and NO_MULTI_WORD_ID is defined)
 * POST: Creates and submits a post
 * ERROR: 1 or -1
 */
int main(int argc, char const *argv[])
{
	char * userID;
	char * streamName;
	char * postFile;

	/* check that the correct parameters were given*/
	if (argc != 4) {
		printf("usage ./post \"userID\" \"stream name\" \"postfile\"\n");
		return 1;
	}

	userID = strduplicate(argv[1]);
	streamName = strduplicate(argv[2]);
	postFile = strduplicate(argv[3]);

	class PostEntry master;

	/* initilize the object */
	master.init();

	master.userID = userID;
	master.streamName = streamName;

	master.readInput(postFile);
	master.getTimeDate();
	master.formatEntry();

	/* Save the post to the stream*/
	master.submitPost();

	master.destroy();

	free(postFile);
	return 0;
}
