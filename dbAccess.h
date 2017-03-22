#ifndef __DB_ACCESS_H__
#define __DB_ACCESS_H__


#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mystring.h"


#ifndef HOSTNAME
#define HOSTNAME "dursley.socs.uoguelph.ca"
#endif

#ifndef USERNAME
#define USERNAME "masch"
#endif

#ifndef PASSWORD
#define PASSWORD "0929357"
#endif

#ifndef DATABASE
#define DATABASE "masch"
#endif


/* struct to hold the results of a query */
typedef struct SQL_result
{
	int numRows;
	int numFields;
	int current;
	void** data;
} SQL_result;

typedef struct SQL_post_result
{
	int id;
	char* stream_name;
	char* user_id;
	char* date;
	char* text;
} SQL_post_result;

typedef struct SQL_users_result
{
	int id;
	char* user_id;
	char* stream_name;
	int num_read;
} SQL_users_result;

typedef struct SQL_streams_result
{
	int id;
	char* stream_name;
	int num_posts;
} SQL_streams_result;


MYSQL* initSQL();

int createTable(MYSQL* mysql, char* tableName, int numCol, char** name, char** type);

int insert(MYSQL* mysql, char* tableName, int numData, char** field, char** value);

int update(MYSQL* mysql, char* tableName, int numData, char** field, char** value);

int clearTable(MYSQL* mysql, char* tableName);

int deleteTable(MYSQL* mysql, char* tableName);

void error(char* msg, MYSQL* mysql);


/******************

	CONSTRUCTORS

********************/

SQL_users_result* newUserResult(char* id, char* userID, char* stream, char* numRead);

SQL_streams_result* newStreamResult(char* id, char* stream, char* numPosts);

SQL_post_result* newPostResult(char* id, char* userID, char* stream, char* date, char* text);

SQL_result* newResult(int numRows, int numFields);


/******************

	DESTROYERS

********************/

void freeUserResults(SQL_users_result* result);

void freeStreamResults(SQL_streams_result* result);

void freePostResults(SQL_post_result* result);



void addData(SQL_result* result, void* next);

SQL_result* getUserStreams(MYSQL* mysql, char* userID);

SQL_result* getStreamPosts(MYSQL* mysql, char* stream);




#endif


