/****************************** stream.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: March 14, 2017                    	 Course Name: CIS*2750
Assignment: A3

Contains all the functions for writing to creating and modifying the streams


changeLog:

 - for A3 any output was moved from standard out to stderr

****************************************************************************/

#include "dbAccess.h"
#include "stream.h"


/**
 * destroyPost
 * Frees all the memory associated with the struct.
 * IN: 	post, a pointer to the struct that is being freed
 * OUT: none
 * POST: all the memory is freed
 * ERROR: none
 */
void destroyPost(struct userPost * post)
{
	/* check that all the parameters are valid */
	if (post == NULL)
	{
		return;
	}

	free(post->username);
	free(post->streamname);
	free(post->date);
	free(post->text);
	free(post);
}

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
void updateStream(struct userPost *st)
{
	FILE* stream;			/* file that contains the posts*/
	FILE* streamData;		/* file that contains the start byte of each post*/

	char* fileName;
	char* baseName;
	int filePos;

	/* create the files if they dont exist */
	createFiles(st->streamname);

	baseName = strduplicate("messages/");
	baseName = join(baseName, st->streamname);
	baseName = join(baseName, "Stream");

	fileName = strduplicate(baseName);
	fileName = join(fileName, "Data");

	/* open the file for apending only */
	stream = fopen(baseName, "a");
	streamData = fopen(fileName, "a");

	free(fileName);
	free(baseName);

	/* if the files exist then write the post to the file*/
	if (stream != NULL && streamData != NULL)
	{
		/* get the position and write it to the data file */
		filePos = ftell(stream);
		fprintf(streamData, "%d\n", filePos);

		/* write post to the file */
		fprintf(stream, "%s\n%s\n%s\n", st->username, st->date, st->text);

	}

	/* close the files*/
	fclose(stream);
	fclose(streamData);
}

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
void addUser(char *username, char *list)
{
	/* adds a user with the given user name to all of the sreams in the list*/
	StringList * streamList;
	StringList * temp;

	MYSQL* mysql;

	int status;

	/* make sure the parameters are valid */
	if (list == NULL || username == NULL)
	{
		return;
	}

	/* connect to the mysql database */
	mysql = initSQL();
	if (mysql == NULL)
	{
		return;
	}

	streamList = parseList(list);

	/* something went wrong or none were given */
	if (streamList == NULL)
	{
		mysql_close(mysql);
		return;
	}

	temp = streamList;

	/* loop through every name in the list */
	while (temp != NULL)
	{
		createFiles(temp->str);
		status = addUser_DB(mysql, temp->str, username);

		/* check if the user already has permission to veiew a stream */
		if (status >= 0 )
		{
			fprintf(stderr, "User [%s] already has permission in stream [%s]\n", username, temp->str);
		}
		else if (status == -3)
		{
			fprintf(stderr, "User [%s] added to stream [%s]\n", username, temp->str);

		}
		temp = temp->next;
	}

	destroyStringList(streamList);
	mysql_close(mysql);
}

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
void removeUser(char *username, char *list)
{
	/* adds a user with the given user name to all of the sreams in the list*/
	StringList * streamList;
	StringList * fileData;
	StringList * tempFile;

	int pos;
	StringList * temp;
	char * fileName;
	char* lineBuff;
	char * name;
	FILE* streamUsers;

	int status;

	/* make sure the parameters are valid */
	if (list == NULL || username == NULL)
	{
		return;
	}

	streamList = parseList(list);

	/* something went wrong or none were given */
	if (streamList == NULL)
	{
		return;
	}

	temp = streamList;

	/* loop through every name in the list */
	while (temp != NULL)
	{
		status = checkPermissions(temp->str, username);

		/* check if the user already has permission to veiew a stream */
		if (status == -2)
		{
			fprintf(stderr, "The stream [%s] does not exist.\n", temp->str);
		}
		else if (status == -3)
		{
			fprintf(stderr, "The user [%s] does not have permission in stream [%s].\n", username, temp->str);
		}
		else if (status == -1)
		{
			fprintf(stderr, "Error something went wrong checking permission for the stream [%s]\n", temp->str);
		}
		else
		{
			/* generate the file name */
			fileName = strduplicate("messages/");
			fileName = join(fileName, temp->str);
			fileName = join(fileName, "StreamUsers");
			streamUsers = fopen(fileName, "r");

			fileData = NULL;

			/* does not do any input buffering/ checking*/
			while ((lineBuff = trim(fgetstr(streamUsers))))
			{
				/* make sure it was successful reading the line */
				if (lineBuff == NULL)
				{
					break;
				}

				/* get the position of the devider between the name and the count */
				pos = lastIndex(lineBuff, ' ');

				/* make sure that the line has valid data */
				if (pos == -1)
				{
					break;
				}

				/* get the name part of the string*/
				name = trim(substring(lineBuff, 0, pos));

				/* check to see if the the row does not match username */
				if (strcmp(name, username) != 0)
				{
					fileData = addToStringList(fileData, newStringList(lineBuff));
				}

				free(name);
				free(lineBuff);
			}

			/* close the file so it can be reopended */
			fclose(streamUsers);
			streamUsers = fopen(fileName, "w");

			if (streamUsers != NULL)
			{
				/* print the data to the file */
				tempFile = fileData;
				while (tempFile != NULL)
				{
					fprintf(streamUsers, "%s\n", tempFile->str);
					tempFile = tempFile->next;
				}
			}

			free(fileName);
			fclose(streamUsers);
			destroyStringList(fileData);
		}
		temp = temp->next;
	}

	destroyStringList(streamList);
}

/**
 * checkStream
 * Checks if a file that belongs to the stream exist.
 *
 * IN:	streamName, the name of the stream that is being checked
 * OUT: TRUE, if the file for the stream exists
 *		FALSE, if the file for the stream does not exist or if there is an error
 * POST: The file is opened and closed if it exists
 * ERROR: FALSE is returned if the streamName is NULL
 */
bool checkStream(char* streamName)
{
	FILE* stream;
	char* fileName;

	/* check parameters */
	if (streamName == NULL)
	{
		return FALSE;
	}

	/* generate the file name to check for */
	fileName = strduplicate("messages/");
	fileName = join(fileName, streamName);
	fileName = join(fileName, "Stream");

	/* open the file */
	stream = fopen(fileName, "r");
	free(fileName);

	/* then the file does not exist and create a new stream */
	if (stream == NULL)
	{
		return FALSE;
	}

	/* the stream does exist */
	fclose(stream);
	return TRUE;
}

/**
 * checkPermissions
 * Checks if a given userID has permisions to view/ post to a given stream.
 * If they have access then it will return the number of posts that the user
 * has read in the stream, otherwise -1 is returned.
 *
 * IN:	streamName, the name of the stream that is being checked
 *		userID, the user whos permissions is being checked.
 * OUT:  the number of posts the user has read if they have permission.
 *		-1 on error
 *		-2 if the file does not exist
 *		-3 if the user does not have permissions
 * POST: The file is opened and closed if it exists
 * ERROR: -1 is returned if there is an error.
 */
int checkPermissions(char* streamName, char* userID)
{
	FILE* streamUsers;
	char* fileName;
	char* lineBuff;
	char* numBuff;
	char* name;
	int numRead;
	int pos;

	/* check the parameter list */
	if (streamName == NULL || userID == NULL)
	{
		return -1;
	}

	fileName = strduplicate("messages/");
	fileName = join(fileName, streamName);
	fileName = join(fileName, "StreamUsers");

	streamUsers = fopen(fileName, "r");

	free(fileName);

	/* if the stream does not exist */
	if (streamUsers == NULL)
	{
		return -2;
	}

	/* does not do any input buffering/ checking*/
	while ((lineBuff = trim(fgetstr(streamUsers))))
	{
		/* make sure it was successful reading the line */
		if (lineBuff == NULL)
		{
			break;
		}

		/* get the position of the devider between the name and the count */
		pos = lastIndex(lineBuff, ' ');

		/* make sure that the line has valid data */
		if (pos == -1)
		{
			break;
		}
		/* seperate the strings into name and value */
		numBuff = trim(substring(lineBuff, pos, strlen(lineBuff) - 1));
		name = trim(substring(lineBuff, 0, pos));
		numRead = atoi(numBuff);

		free(lineBuff);
		free(numBuff);

		/* check to see if the user ID matches*/
		if (strcmp(name, userID) == 0)
		{
			free(name);
			fclose(streamUsers);
			return numRead;
		}

		free(name);
	}

	fclose(streamUsers);
	return -3;
}

/**
 * createFiles
 * Creates all 3 files that are required for the stream.
 *
 * IN:	streamName, the name of the stream that is being created
 * OUT:	none
 * POST: 3 files are created if they do not exist
 * ERROR: nothing happens if the filename is invalid.
 */
void createFiles(char* streamName)
{
	char* fileName;
	char* baseName;
	FILE* file;

	/* make sure the parameters are given */
	if (streamName == NULL)
	{
		return;
	}

	/* generate the base file name for the stream */
	baseName = strduplicate("messages/");
	baseName = join(baseName, streamName);
	baseName = join(baseName, "Stream");

	/* <stream>Stream */
	file = fopen(baseName, "a");
	fclose(file);

	/* <stream>StreamData */
	fileName = strduplicate(baseName);
	fileName = join(fileName, "Data");
	file = fopen(fileName, "a");
	fclose(file);

	/* <stream>StreamUsers */
	free(fileName);
	fileName = strduplicate(baseName);
	fileName = join(fileName, "Users");
	file = fopen(fileName, "a");

	/* close the file and free the strings */
	fclose(file);
	free(fileName);
	free(baseName);
}

/**
 * parseList
 * Takes in a comma seperates list if tokens and it gives back
 * a liked list if individual tokens
 * and with leadind and trailing whitespace removed.
 * the list string needs to be freed by the caller.
 *
 * IN:	list, the string to be tokenized
 * OUT:	a linked list if the tokens on success
 *		NULL on failure
 * POST: a linked list structure is created
 * ERROR: NULL if the list is invalid
 */
StringList* parseList(char* list)
{
	char* streamName;

	StringList* streamList;
	StringList* stream;

	int start;
	int end;

	/* make sure that the list given is valid */
	if (list == NULL)
	{
		return NULL;
	}

	/* initilize the variables */
	start = 0;
	end = 0;
	stream = NULL;
	streamList = NULL;

	/* loop untill there are no more streams in the list */
	while (end != -1)
	{
		/* get the end index of the stream name */
		end = firstIndexOffset(list, ',', start);


		/* then this token is the last token */
		if (end == -1)
		{
			streamName = trim(substring(list, start, strlen(list) - 1));
			stream = newStringList(streamName);

			free(streamName);

			streamList = addToStringList(streamList, stream);
			break;
		}

		/* if there are still more streams  */
		streamName = trim(substring(list, start, end - 1));
		stream = newStringList(streamName);

		free(streamName);

		streamList = addToStringList(streamList, stream);
		start = end + 1;
	}

	return streamList;
}
