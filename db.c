/*
tables:
	users (ID userName streamName numRead)

	streams (ID streamName numPosts)

	posts (ID streamName userName date text)

*/

#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include "mystring.h"



/* prints all the data */
int printUsers(MYSQL* mysql);
int printStreams(MYSQL* mysql);
int printposts(MYSQL* mysql);

void usage();


int main(int argc, char const *argv[])
{
	MYSQL* mysql;

	mysql = initSQL();



	mysql_close(&mysql);	

	return 0;
}




int printUsers(MYSQL* mysql)
{
	char* query;

	MYSQL_RES* results;
	MYSQL_ROW  row;

	/* make sure parameters are valid */
	if (mysql == NULL) {
		return -1;
	}

	query = strduplicate("SELECT DISTINCT user_id FROM users");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to select users.", mysql);
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

	/* print the userIDs */
	while ((row = mysql_fetch_row(results)))
	{
		printf("%s\n", row[0]);
	}


	return 0;
}

int printStreams(MYSQL* mysql)
{
	char* query;

	MYSQL_RES* results;
	MYSQL_ROW  row;

	/* make sure parameters are valid */
	if (mysql == NULL) {
		return -1;
	}

	query = strduplicate("SELECT DISTINCT steam_name FROM streams");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to select streams.", mysql);
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

	/* print the userIDs */
	while ((row = mysql_fetch_row(results)))
	{
		printf("%s\n", row[0]);
	}


	return 0;
}

int printposts(MYSQL* mysql)
{
	char* query;

	MYSQL_RES* results;
	MYSQL_ROW  row;

	/* make sure parameters are valid */
	if (mysql == NULL) {
		return -1;
	}

	query = strduplicate("SELECT stream_name,user_id,date,text FROM posts");

	if (mysql_query(mysql, query) != 0)
	{
		error("failed to select posts.", mysql);
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

	/* print the userIDs */
	while ((row = mysql_fetch_row(results)))
	{

		printf("stream: %s\n", row[0]);
		printf("userID: %s\n", row[1]);
		printf("date: %s\n", row[2]);
		printf("text: %s\n", row[3]);
		printf("----------------------------------------\n\n");
	}


	return 0;
}




void usage()
{
	prinf("Usage: ./db\nuse to access the database\n");
	printf("  -clear : removes all of the posts, users, streams from the tables in the database\n");
	printf("  -reset : deletes the tables from the database\n");
	printf("  -posts : prints out all posts stored in the database\n");
	printf("  -users : prints out all user names stored in the database\n");
	printf("  -streams : prints out all stream names stored in the database\n");
	printf("  -help : displays this help message\n\n");
}

