/****************************** dbAccess.h ***********************************
Student Name: Marshall Aaron Asch			Student Number:  0928357
Date: March 28 2017							Course Name: CIS*2750
Assignment: A4

Contains all the functions for accessing and manipulating the database

****************************************************************************/

#ifndef __DB_ACCESS_H__
#define __DB_ACCESS_H__

#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystring.h"


#ifndef HOSTNAME
#define HOSTNAME "localhost"
#endif

#ifndef USERNAME
#define USERNAME "cis2750"
#endif

#ifndef PASSWORD
#define PASSWORD "angleOfDeath"
#endif

#ifndef DATABASE
#define DATABASE "cis2750"
#endif


/* struct to hold the results of a query */
typedef struct SQL_result {
	int numRows;
	int numFields;
	int current;
	void** data;
} SQL_result;

typedef struct SQL_post_result {
	int id;
	char* stream_name;
	char* user_id;
	char* date;
	char* text;
} SQL_post_result;

typedef struct SQL_users_result {
	int id;
	char* user_id;
	char* stream_name;
	int num_read;
} SQL_users_result;

typedef struct SQL_streams_result {
	int id;
	char* stream_name;
	int num_posts;
} SQL_streams_result;


/**
 * initSQL
 * Initilize the mysql object and connect to the database
 *
 * @return pointer to the object on success
 *         NULL on failure
 */
MYSQL* initSQL();

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
int createTable_DB(MYSQL* mysql, char* tableName, int numCol, char** name, char** type);

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
int insert_DB(MYSQL* mysql, char* tableName, int numData, char** field, char** value);

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
int update_DB(MYSQL* mysql, char* tableName, int numData, char** field, char** value);

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
int deleteFromTable_DB(MYSQL* mysql, char* tableName, char* condition);

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
int dropTable_DB(MYSQL* mysql, char* tableName);

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
int newStream_DB(MYSQL* mysql, char* streamName);

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
int addUser_DB(MYSQL* mysql, char* streamName, char* userID);

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
int removeUser_DB(MYSQL* mysql, char* streamName, char* userID);

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
int count_DB(MYSQL* mysql, char* tableName, char* condition);

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
int markAllRead_DB(MYSQL * mysql, char* streamName, char* userID);

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
int markOneRead_DB(MYSQL * mysql, char* streamName, char* userID);

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
int getNumPosts_DB(MYSQL * mysql, char* streamName);

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
int getNumRead_DB(MYSQL * mysql, char* streamName, char* userID);

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
int submitPost_DB(MYSQL* mysql, char* streamName, char* userID, char* text, char* date);

/**
 * error
 * Prints an error message to the stderr
 *
 * @param msg   the given message to print
 * @param mysql the mysql object that caused the error
 */
void error(char* msg, MYSQL* mysql);



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
SQL_users_result* newUserResult(char* id, char* userID, char* stream, char* numRead);

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
SQL_streams_result* newStreamResult(char* id, char* stream, char* numPosts);

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
SQL_post_result* newPostResult(char* id, char* userID, char* stream, char* date, char* text);

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
SQL_result* newResult(int numRows, int numFields);


/******************

	DESTROYERS

********************/

/**
 * freeUserResults
 * Destroys all the memory that is used by the user results
 *
 * @param result the struct to be freed
 */
void freeUserResults(SQL_users_result* result);

/**
 * freeStreamResults
 * Destroys all the memory that is used by the
 * streams results
 *
 * @param result the struct to be freed
 */
void freeStreamResults(SQL_streams_result* result);

/**
 * freePostResults
 * Destroys all the memory that is used by the
 * posts results
 *
 * @param result the struct to be freed
 */
void freePostResults(SQL_post_result* result);

/**
 * addData
 * Adds a data row to the list of results
 *
 * @param result the list to add it to
 * @param next   the new result to add
 */
void addData(SQL_result* result, void* next);

/**
 * getUserStreams_DB
 * Gets all the streams that a user has permission in
 *
 * @param  mysql  the mysql object
 * @param  userID the user that it is searching for
 * @return        pointer to the result struct on success
 *                NULL on error
 */
SQL_result* getUserStreams_DB(MYSQL* mysql, char* userID);

/**
 * getStreamPosts_DB
 * Gets all the posts that are in the stream
 *
 * @param  mysql  the mysql object
 * @param  stream the stream that it is searching for
 * @return        pointer to the result struct on success
 *                NULL on error
 */
SQL_result* getStreamPosts_DB(MYSQL* mysql, char* stream);

#endif
