/****************************** dbAccess.c ***********************************
Student Name: Marshall Aaron Asch			Student Number:  0928357
Date: March 28 2017							Course Name: CIS*2750
Assignment: A4

Contains all the functions for accessing and manipulating the database

****************************************************************************/

#include "dbAccess.h"

/**
 * initSQL
 * Initilize the mysql object and connect to the database
 *
 * @return pointer to the object on success
 *         NULL on failure
 */
MYSQL* initSQL()
{
	MYSQL* mysql;

	mysql = NULL;

	/* initilize the connection */
	mysql = mysql_init(mysql);
	mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");

	if (!mysql_real_connect(mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0))
	{
		error("Failed to connect to server.", mysql);
		mysql_close(mysql);
		return NULL;
	}

	return mysql;
}

/**
 * createTable_DB
 * Create a new table with the given name if one does not exist.
 * has a default column
 * 	id INT NOT NULL AUTO_INCREMENT PRIMARY KEY
 *
 * 	adds the list of names and types for the columns/
 *
 *
 * @param  mysql     the mysql object
 * @param  tableName the name of the table to create
 * @param  numCol    the number of additional columns to add
 * @param  name      the list of additional column names
 * @param  type      the list of column types
 * @return           -1 if param are invalid
 *                   -2 if query failed
 *                    0 on success
 */
int createTable_DB(MYSQL* mysql, char* tableName, int numCol, char** name, char** type)
{
	int i;				/* loop var */
	char* query;

	/* make sure the parameters are valid */
	if (mysql == NULL || tableName == NULL || name == NULL || type == NULL || numCol < 1)
	{
		return -1;
	}

	/* create the query */
	query = strduplicate("CREATE TABLE IF NOT EXISTS ");
	query = join(query, tableName);
	query = join(query, " (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, ");

	/* create the columns of the table */
	for (i = 0; i < numCol - 1; i++)
	{
		query = join(query, name[i]);
		query = joinC(query, ' ');
		query = join(query, type[i]);
		query = join(query, ", ");
	}

	query = join(query, name[numCol - 1]);
	query = joinC(query, ' ');
	query = join(query, type[numCol - 1]);
	query = join(query, ") CHARACTER SET utf8 COLLATE utf8_bin");

	/* mysql_query returns 0 on success */
	if (mysql_query(mysql, query) != 0)
	{
		error("failed to create table.", mysql);
		free(query);
		return -2;
	}
	free(query);

	return 0;
}

/**
 * insert_DB
 * Insert a new row into the table with the given values
 * for each of the columns.
 *
 * @param  mysql     the mysql object
 * @param  tableName the name of the table to insert into
 * @param  numData   the number of elements
 * @param  field     the names of the columns the data goes into
 * @param  value     the values
 * @return           -1 if param are not valid
 *                   -2 if the query fails
 *                    0 on success
 */
int insert_DB(MYSQL* mysql, char* tableName, int numData, char** field, char** value)
{
	int i;				/* loop var */
	char* query;

	/* make sure the parameters are valid */
	if (mysql == NULL || tableName == NULL || field == NULL || value == NULL || numData < 1)
	{
		return -1;
	}

	/* create the query */
	query = strduplicate("INSERT INTO ");
	query = join(query, tableName);

	query = join(query, " (");

	/* enter the column name */
	for (i = 0; i < numData - 1; i++)
	{
		query = join(query, field[i]);
		query = join(query, ", ");
	}
	query = join(query, field[numData - 1]);
	query = join(query, ") VALUES (\"");


	/* enter the values */
	for (i = 0; i < numData - 1; i++)
	{
		query = join(query, value[i]);

		query = join(query, "\", \"");
	}
	query = join(query, value[numData - 1]);
	query = join(query, "\")");

	/* mysql_query returns 0 on success */
	if (mysql_query(mysql, query) != 0)
	{
		error("failed to insert.", mysql);
		free(query);
		return -2;
	}

	free(query);
	return 0;
}

/**
 * update_DB
 * Updates a new row into the table with the given values
 * for each of the columns.
 *
 * @param  mysql     the mysql object
 * @param  tableName the name of the table to insert into
 * @param  numData   the number of elements
 * @param  field     the names of the columns the data goes into
 * @param  value     the values
 * @return           -1 if param are not valid
 *                   -2 if the query fails
 *                    0 on success
 */
int update_DB(MYSQL* mysql, char* tableName, int numData, char** field, char** value)
{
	int i;				/* loop var */
	char* query;

	/* make sure the parameters are valid */
	if (mysql == NULL || tableName == NULL || field == NULL || value == NULL || numData < 1)
	{
		return -1;
	}
	/* create the query */
	query = strduplicate("UPDATE ");
	query = join(query, tableName);
	query = join(query, " JOIN ");

	/* enter the column name */
	for (i = 0; i < numData - 1; i++)
	{
		query = join(query, field[i]);
		query = join(query, " = ");
		query = join(query, value[i]);
		query = join(query, ", ");
	}
	query = join(query, field[numData - 1]);
	query = join(query, " = ");
	query = join(query, value[numData - 1]);

	/* mysql_query returns 0 on success */
	if (mysql_query(mysql, query) != 0)
	{
		error("failed to insert.", mysql);
		free(query);
		return -2;
	}

	free(query);
	return 0;
}

/**
 * deleteFromTable_DB
 * Deletes rows from the given table.
 * If condition is NULL then all rows will be deleted.
 * else only the rows that satisfy the condition will be removed
 *
 * @param  mysql     the mysql object
 * @param  tableName the name of the table to insert into
 * @param  condition optional condtion to be met to delete only some rows
 * @return           -1 if param are invalid
 *                   -2 if it fails
 *                    0 on success
 */
int deleteFromTable_DB(MYSQL* mysql, char* tableName, char* condition)
{
	char* query;

	/* make sure that param are valid */
	if (mysql == NULL || tableName == NULL)
	{
		return -1;
	}

	query = strduplicate("DELETE FROM ");
	query = join(query, tableName);

	if (condition != NULL)
	{
		query = join(query, " WHERE ");
		query = join(query, condition);
	}

	/* mysql_query returns 0 on success */
	if (mysql_query(mysql, query) != 0)
	{
		error("failed to clear table.", mysql);
		free(query);
		return -2;
	}

	free(query);
	return 0;
}

/**
 * dropTable_DB
 * Removed the entire table from the database
 *
 * @param  mysql     the mysql object
 * @param  tableName the name of the table to drop
 * @return           -1 if param are invalid
 *                   -2 if it fails
 *                    0 on success
 */
int dropTable_DB(MYSQL* mysql, char* tableName)
{
	char* query;

	/* make sure that param are valid */
	if (mysql == NULL || tableName == NULL)
	{
		return -1;
	}

	query = strduplicate("DROP TABLE ");
	query = join(query, tableName);

	/* mysql_query returns 0 on success */
	if (mysql_query(mysql, query) != 0)
	{
		error("failed to delete table.", mysql);
		free(query);
		return -2;
	}

	free(query);
	return 0;
}

/**
 * newStream_DB
 * Add a new stream to the stream table
 *
 * @param  mysql     the mysql object
 * @param  streamName the name of the stream to add
 * @return           -1 if param are invalid
 *                   -2 if it fails
 *                    0 on success
 */
int newStream_DB(MYSQL* mysql, char* streamName)
{
	char* condition;
	int num;

	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL)
	{
		return -1;
	}

	/* check to see if the stream already exists */
	condition = strduplicate("stream_name = \"");
	condition = join(condition, streamName);
	condition = join(condition, "\"");

	num = count_DB(mysql, "streams", condition);
	free(condition);

	char* namesStreams[] = {"stream_name", "num_posts"};
	char* data[] = {streamName, "0"};

	/* if it does not exist then add it */
	if (num == 1)
	{
		/* it exists*/
		return 1;
	}
	else if (num == 0)
	{
		/* status of the insert*/
		return insert_DB(mysql, "streams", 2, namesStreams, data);
	}
	else
	{
		/* error counting the streams */
		return num;
	}
}

/**
 * addUser_DB
 * Adds a user to the given stream if they are not already in the stream
 * If the stream does not exist then it is created
 *
 * @param  mysql      the mysql object
 * @param  streamName the name of the stream to add
 * @param  userID     the name of the user to add
 * @return 0 		  On success added
 *         >0		  If the user is already in the stream
 *         <0		  On error
 */
int addUser_DB(MYSQL* mysql, char* streamName, char* userID)
{
	char* condition;
	int num;
	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL)
	{
		return -1;
	}

	/* make sure the stream exists */
	newStream_DB(mysql, streamName);

	/* check to see if the user is already in the stream */
	condition = strduplicate("stream_name = \"");
	condition = join(condition, streamName);
	condition = join(condition, "\" AND user_id = \"");
	condition = join(condition, userID);
	condition = join(condition, "\"");

	num = count_DB(mysql, "users", condition);
	free(condition);

	char* namesStreams[] = {"user_id", "stream_name", "num_read"};
	char* data[] = {userID, streamName, "0"};

	if (num > 0)
	{
		/* if the user is alreay in the stream */
		return 1;
	}
	else if (num == 0)
	{
		/* status of insert*/
		return insert_DB(mysql, "users", 3, namesStreams, data);
	}
	else
	{
		/* error code */
		return num;
	}
}

/**
 * removeUser_DB
 * Removes a user from the given stream if they are
 * in the stream
 *
 * @param  mysql      the mysql object
 * @param  streamName the name of the stream to remove from
 * @param  userID     the name of the user to remove
 * @return 0 		  On success remove
 *         -1 		  if param are invalid
 *         			if delete failed
 */
int removeUser_DB(MYSQL * mysql, char* streamName, char* userID)
{
	char* condition;
	int status;

	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL)
	{
		return -1;
	}

	condition = strduplicate("user_id = \"");
	condition = join(condition, userID);
	condition = join(condition, "\" AND stream_name = \"");
	condition = join(condition, streamName);
	condition = join(condition, "\"");

	/* add the user to the if they were not already there */
	status = deleteFromTable_DB(mysql, "users", condition);
	free(condition);

	return status;
}

/**
 * count_DB
 * Counts the number of rows in the table that match the
 * given condition.
 *
 * @param  mysql     the mysql object
 * @param  tableName the name of the stream to remove from
 * @param  condition optional condtion to be met
 * @return           -1 if param are invalid
 *                   -2 if query fails
 *                   -3 if failed to load data
 *                   -4 if there are too many rows
 *                   >=0 on success
 */
int count_DB(MYSQL * mysql, char* tableName, char* condition)
{
	char* query;
	MYSQL_RES* results;
	MYSQL_ROW  row;
	int num;

	/* make sure that param are valid */
	if (mysql == NULL || tableName == NULL)
	{
		return -1;
	}

	query = strduplicate("SELECT COUNT(*) FROM ");
	query = join(query, tableName);

	if (condition != NULL)
	{
		query = join(query, " WHERE ");
		query = join(query, condition);
	}

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to perform the query", mysql);
		free(query);
		return -2;
	}
	free(query);

	/* store the results of the query */
	if (!(results = mysql_store_result(mysql)))
	{
		error("failed to load the data.", mysql);
		return -3;
	}

	/* make sure there is only 1 result row */
	if (mysql_num_rows(results) != 1)
	{
		mysql_free_result(results);
		return -4;
	}

	/* get the number from the results */
	row = mysql_fetch_row(results);
	num = atoi(row[0]);

	mysql_free_result(results);
	return num;
}

/**
 * markAllRead_DB
 * Mark all the posts in the given stream as read by the user.
 *
 * @param  mysql      the mysql object
 * @param  streamName the name of the stream that is read
 * @param  userID     the ID of the user that has read them
 * @return            -1 if param are invalid
 *                    -2 if the query failed
 *                    0 on success
 */
int markAllRead_DB(MYSQL * mysql, char* streamName, char* userID)
{
	char* query;
	int numPosts;
	char buff[10];

	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL)
	{
		return -1;
	}

	/* get the number of posts in the stream */
	numPosts = getNumPosts_DB(mysql, streamName);
	if (numPosts < 0)
	{
		return numPosts;
	}

	sprintf(buff, "%d", numPosts);

	/* change tbe total number of posts the user has read */
	query = strduplicate("UPDATE users SET num_read =\"");
	query = join(query, buff);
	query = join(query, "\" WHERE stream_name = \"");
	query = join(query, streamName);
	query = join(query, "\" AND user_id = \"");
	query = join(query, userID);
	query = join(query, "\"");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to perform the query", mysql);
		free(query);
		return -2;
	}
	free(query);

	return 0;
}

/**
 * markOneRead_DB
 * Mark one of the posts in the given stream as read by the user.
 *
 * @param  mysql      the mysql object
 * @param  streamName the name of the stream that is read
 * @param  userID     the ID of the user that has read them
 * @return            -1 if param are invalid
 *                    -2 if the query failed
 *                    0 on success
 */
int markOneRead_DB(MYSQL * mysql, char* streamName, char* userID)
{
	char* query;
	int numPosts;
	char buff[10];

	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL)
	{
		return -1;
	}

	/* get the number of posts in the stream */
	numPosts = getNumPosts_DB(mysql, streamName);
	if (numPosts < 0)
	{
		return numPosts;
	}

	sprintf(buff, "%d", numPosts);

	/* change tbe total number of posts the user has read */
	query = strduplicate("UPDATE users SET num_read = num_read + 1 WHERE stream_name = \"");
	query = join(query, streamName);
	query = join(query, "\" AND user_id = \"");
	query = join(query, userID);
	query = join(query, "\" AND num_read < ");
	query = join(query, buff);

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to perform the query", mysql);
		free(query);
		return -2;
	}
	free(query);

	return 0;
}

/**
 * getNumPosts_DB
 * Get the total number of posts that are in the stream.
 *
 * @param  mysql      the mysql object
 * @param  streamName the name of the stram to get the number of posts for
 * @return            -1 if param are invalid
 *                    -2 if query failed
 *                    -3 if failed to load data
 *                    -4 if there are too many reasults
 *                    >=0 on success
 */
int getNumPosts_DB(MYSQL * mysql, char* streamName)
{
	char* query;
	MYSQL_RES* results;
	MYSQL_ROW  row;
	int num;

	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL)
	{
		return -1;
	}

	/* get the total number of posts */
	query = strduplicate("SELECT num_posts FROM streams WHERE stream_name = \"");
	query = join(query, streamName);
	query = join(query, "\"");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to perform the query", mysql);
		free(query);
		return -2;
	}
	free(query);

	/* store the results of the query */
	if (!(results = mysql_store_result(mysql)))
	{
		error("failed to load the data.", mysql);
		return -3;
	}

	/* make sure there is only 1 result row */
	if (mysql_num_rows(results) != 1)
	{
		mysql_free_result(results);
		return -4;
	}

	/* get the number from the results */
	row = mysql_fetch_row(results);
	num = atoi(row[0]);

	mysql_free_result(results);
	return num;
}

/**
 * getNumRead_DB
 * Get the number of posts that has been read by the user
 *
 * @param  mysql      the mysql object
 * @param  streamName the stream that the use has read
 * @param  userID     the user that it is checking
 * @return            -1 if param are invalid
 *                    -2 if query failed
 *                    -3 if failed to load data
 *                    -4 if there are too many reasults
 *                    >=0 on success
 */
int getNumRead_DB(MYSQL * mysql, char* streamName, char* userID)
{
	char* query;
	MYSQL_RES* results;
	MYSQL_ROW  row;
	int num;

	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL)
	{
		return -1;
	}

	/* get the total number of posts */
	query = strduplicate("SELECT num_read FROM users WHERE stream_name = \"");
	query = join(query, streamName);
	query = join(query, "\" AND user_id = \"");
	query = join(query, userID);
	query = join(query, "\"");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to perform the query", mysql);
		free(query);
		return -2;
	}
	free(query);

	/* store the results of the query */
	if (!(results = mysql_store_result(mysql)))
	{
		error("failed to load the data.", mysql);
		return -3;
	}

	/* make sure there is only 1 result row */
	if (mysql_num_rows(results) != 1)
	{
		mysql_free_result(results);
		return -4;
	}

	/* get the number from the results */
	row = mysql_fetch_row(results);
	num = atoi(row[0]);

	mysql_free_result(results);
	return num;
}

/**
 * submitPost_DB
 * Submit the post to the database
 *
 * @param  mysql      the mysql object
 * @param  streamName the name of the stream that the post is in
 * @param  userID     the user ID that is summiting the post
 * @param  text       the text of the post
 * @param  date       the date the post was submitted
 * @return            -1 if param are invaild
 *                    -2 if query fails
 *                     0 on success
 */
int submitPost_DB(MYSQL* mysql, char* streamName, char* userID, char* text, char* date)
{
	char* condition;
	int num;
	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL || text == NULL || date == NULL)
	{
		return -1;
	}

	/* check to see if the user is already in the stream */
	condition = strduplicate("stream_name = \"");
	condition = join(condition, streamName);
	condition = join(condition, "\" AND user_id = \"");
	condition = join(condition, userID);
	condition = join(condition, "\"");

	num = count_DB(mysql, "users", condition);
	free(condition);

	char* fieldName[] = {"user_id", "stream_name", "date", "text"};
	char* data[] = {userID, streamName, date, text};

	if (num == 1)
	{
		/* if the user is alreay in the stream */
		return insert_DB(mysql, "posts", 4, fieldName, data);
	}
	else if (num == 0)
	{
		/* status of insert*/
		return -2;
	}
	else
	{
		/* error code */
		return num;
	}
}

/**
 * error
 * Prints an error message to the stderr
 *
 * @param msg   the given message to print
 * @param mysql the mysql object that caused the error
 */
void error(char* msg, MYSQL * mysql)
{
	FILE* errorStream;
	errorStream  = freopen("error.log", "a", stderr);

	fprintf(stderr, "%s\n%s\n", msg, mysql_error(mysql));

	fclose(errorStream);
}


/******************

	CONSTRUCTORS

********************/


/**
 * newUserResult
 * Creates a new result struct for a row from the
 * users table.
 *
 * @param  id      the row ID (primary key)
 * @param  userID  the users name
 * @param  stream  the stream name
 * @param  numRead the number of posts they have read
 * @return         pointer to the struct on success
 *                 NULL on failure
 */
SQL_users_result* newUserResult(char* id, char* userID, char* stream, char* numRead)
{
	SQL_users_result* result;

	/* make sure param are valid */
	if (id == NULL || userID == NULL || stream == NULL || numRead == NULL)
	{
		return NULL;
	}

	/* allocate memory for the structure */
	result = malloc(sizeof(SQL_users_result));
	if (result == NULL)
	{
		return NULL;
	}

	/* set the data */
	result->id = atoi(id);
	result->user_id = strduplicate(userID);
	result->stream_name = strduplicate(stream);
	result->num_read = atoi(numRead);

	return result;
}

/**
 * newStreamResult
 * Creates a new result struct for a row from the
 * streams table.
 *
 * @param  id      the row ID (primary key)
 * @param  stream  the stream name
 * @param  numPosts the number of posts in the stream
 * @return         pointer to the struct on success
 *                 NULL on failure
 */
SQL_streams_result* newStreamResult(char* id, char* stream, char* numPosts)
{
	SQL_streams_result* result;

	/* make sure param are valid */
	if (id == NULL || stream == NULL || numPosts == NULL)
	{
		return NULL;
	}

	/* allocate memory for the structure */
	result = malloc(sizeof(SQL_streams_result));
	if (result == NULL)
	{
		return NULL;
	}

	/* set the data */
	result->id = atoi(id);
	result->stream_name = strduplicate(stream);
	result->num_posts = atoi(numPosts);

	return result;
}

/**
 * newPostResult
 * Creates a new result struct for a row from the
 * posts table.
 *
 * @param  id      the row ID (primary key)
 * @param  userID  the users name
 * @param  stream  the stream name
 * @param  date    the date that the post was created
 * @param  text    the text of the post
 * @return         pointer to the struct on success
 *                 NULL on failure
 */
SQL_post_result* newPostResult(char* id, char* userID, char* stream, char* date, char* text)
{
	SQL_post_result* result;

	/* make sure param are valid */
	if (id == NULL || userID == NULL || stream == NULL || date == NULL || text == NULL)
	{
		return NULL;
	}

	/* allocate memory for the structure */
	result = malloc(sizeof(SQL_post_result));
	if (result == NULL)
	{
		return NULL;
	}

	/* set the data */
	result->id = atoi(id);
	result->stream_name = strduplicate(stream);
	result->user_id = strduplicate(userID);
	result->date = strduplicate(date);
	result->text = strduplicate(text);

	return result;
}

/**
 * newResult
 * Creates a new result struct that holds the list
 * of results. The list needs to be freed by the caller
 *
 * @param  numRows 		the number of rows in the list
 * @param  numFields    the number of columns in the results
 * @return         		pointer to the struct on success
 *                      NULL on failure
 */
SQL_result* newResult(int numRows, int numFields)
{
	SQL_result* result;
	void** data;
	int i;

	/* allocate memory */
	result = malloc(sizeof(SQL_result));
	if (result == NULL)
	{
		return NULL;
	}

	/* allocate memory */
	data = malloc(sizeof(void*) * (numRows + 1));
	if (data == NULL)
	{
		free(result);
		return NULL;
	}

	/* fill with NULL's */
	for (i = 0; i <= numRows; i++)
	{
		data[i] = NULL;
	}


	/* put the data in */
	result->numRows = numRows;
	result->numFields = numFields;
	result->current = 0;
	result->data = data;

	return result;
}

/******************

	DESTROYERS

********************/

/**
 * freeUserResults
 * Destroys all the memory that is used by the user results
 *
 * @param result the struct to be freed
 */
void freeUserResults(SQL_users_result * result)
{
	/* make sure the parameter is valid */
	if (result == NULL)
	{
		return;
	}

	free(result->user_id);
	free(result->stream_name);
	free(result);
}

/**
 * freeStreamResults
 * Destroys all the memory that is used by the
 * streams results
 *
 * @param result the struct to be freed
 */
void freeStreamResults(SQL_streams_result * result)
{
	/* make sure the parameter is valid */
	if (result == NULL)
	{
		return;
	}

	free(result->stream_name);
	free(result);
}

/**
 * freePostResults
 * Destroys all the memory that is used by the
 * posts results
 *
 * @param result the struct to be freed
 */
void freePostResults(SQL_post_result * result)
{
	/* make sure the parameter is valid */
	if (result == NULL)
	{
		return;
	}

	free(result->stream_name);
	free(result->user_id);
	free(result->date);
	free(result->text);
	free(result);
}

/**
 * addData
 * Adds a data row to the list of results
 *
 * @param result the list to add it to
 * @param next   the new result to add
 */
void addData(SQL_result * result, void* next)
{
	/* make sure the parameters are valid */
	if (result == NULL || result->data == NULL || next == NULL)
	{
		return;
	}

	result->data[result->current++] = next;
}

/**
 * getUserStreams_DB
 * Gets all the streams that a user has permission in
 *
 * @param  mysql  the mysql object
 * @param  userID the user that it is searching for
 * @return        pointer to the result struct on success
 *                NULL on error
 */
SQL_result* getUserStreams_DB(MYSQL * mysql, char* userID)
{
	char* query;
	SQL_result* result;
	SQL_users_result* users;

	MYSQL_RES* res;
	MYSQL_ROW  row;

	users = NULL;
	/* check the parameters */
	if (mysql == NULL || userID == NULL)
	{
		return NULL;
	}

	/* generate the query */
	query = strduplicate("SELECT id, user_id, stream_name, num_read FROM users WHERE user_id = \"");
	query = join(query, userID);
	query = join(query, "\"");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to select users Streams.", mysql);
		free(query);
		return NULL;
	}

	free(query);

	/* store the results of the query */
	if (!(res = mysql_store_result(mysql)))
	{
		error("failed to load the data.", mysql);
		return NULL;
	}
	result = newResult(mysql_num_rows(res), mysql_num_fields(res));

	/* put the data into the struct */
	while ((row = mysql_fetch_row(res)))
	{
		users = newUserResult(row[0], row[1], row[2], row[3]);
		addData(result, users);
	}

	mysql_free_result(res);
	return result;
}

/**
 * getStreamPosts_DB
 * Gets all the posts that are in the stream
 *
 * @param  mysql  the mysql object
 * @param  stream the stream that it is searching for
 * @return        pointer to the result struct on success
 *                NULL on error
 */
SQL_result* getStreamPosts_DB(MYSQL * mysql, char* stream)
{
	char* query;
	SQL_result* result;
	SQL_post_result* post;

	MYSQL_RES* res;
	MYSQL_ROW  row;

	/* check the parameters */
	if (mysql == NULL || stream == NULL)
	{
		return NULL;
	}

	/* generate the query */
	query = strduplicate("SELECT id, stream_name, user_id, date, text FROM posts WHERE stream_name = \"");
	query = join(query, stream);
	query = join(query, "\"");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to select posts from the.", mysql);
		free(query);
		return NULL;
	}

	free(query);

	/* store the results of the query */
	if (!(res = mysql_store_result(mysql)))
	{
		error("failed to load the data.", mysql);
		return NULL;
	}
	result = newResult(mysql_num_rows(res), mysql_num_fields(res));

	/* put the data into the struct */
	while ((row = mysql_fetch_row(res)))
	{
		post = newPostResult(row[0], row[2], row[1], row[3], row[4]);
		addData(result, post);
	}
	mysql_free_result(res);
	return result;
}
