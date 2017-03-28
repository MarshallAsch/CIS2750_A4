/****************************** stream.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: March 28, 2017                    	 Course Name: CIS*2750
Assignment: A4

Contains all the functions for writing to creating and modifying the streams


changeLog:
	- for A3 any output was moved from standard out to stderr
	- for A4 changed to use the database instead of the files
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
 * Saves the post to the table. If the stream does not exist
 * then it is not created.
 * The user must have permisoin in the stream.
 *
 * IN: 	st, the struct containing the data to be posted
 * OUT: none
 * POST: files are created if nesisay, the files are modified
 * ERROR: returns if the parameters are invalid
 */
void updateStream(struct userPost *st)
{
	MYSQL* mysql;

	if (st == NULL)
	{
		return;
	}

	/* connect to datase */
	mysql = initSQL();
	if (mysql == NULL)
	{
		return;
	}

	/* write post to the DB */
	submitPost_DB(mysql, st->streamname, st->username, st->text, st->date);

	mysql_close(mysql);
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

	/* something went wrong or none were given */
	streamList = parseList(list);
	if (streamList == NULL)
	{
		mysql_close(mysql);
		return;
	}

	/* loop through every name in the list */
	temp = streamList;
	while (temp != NULL)
	{
		status = removeUser_DB(mysql, temp->str, username);

		/* check if the user already has permission to veiew a stream */
		if (status < 0)
		{
			fprintf(stderr, "Error removing user [%s] from [%s].\n", username, temp->str);
		}
		else
		{
			fprintf(stderr, "The user [%s] removed from stream [%s].\n", username, temp->str);

		}
		temp = temp->next;
	}

	mysql_close(mysql);
	destroyStringList(streamList);
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
