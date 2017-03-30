/****************************** db.c ***********************************
Student Name: Marshall Aaron Asch			Student Number:  0928357
Date: March 22, 2017						Course Name: CIS*2750
Assignment: A4

The program for accesing the database.
can create, destroy and view the tables.

Takes in 1 commandline argument which is the acction it will do.

****************************************************************************/


#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mystring.h"
#include "dbAccess.h"
#include "db.h"

/**
 * usage
 * Prints the usage message for this program
 */
static void usage();

/**
 * parseOption
 * Parses the commndline argument into its character flag
 *
 * @param  option	the commandline argument
 * @return c		clear
 *         r		reset
 *         p		posts
 *         u		users
 *         s		streams
 *         h		help
 *         i		initilize
 *         o		unknown option
 */
static char parseOption(const char* option);


/**
 * main
 * Main entry point to the program
 *
 * @param  argc number of commandline arguments
 * @param  argv arguments
 * @return      -1 if there are not the correct number
 *               0 on success
 */
int main(int argc, char const *argv[])
{
	MYSQL* mysql;



	/* make sure the num arguments are correct */
	if (argc != 2)
	{
		usage();
		return -1;
	}

	/* initilize connection to the database */
	mysql = initSQL();

	switch (parseOption(argv[1]))
	{
	case 'c':
		clearAll(mysql);
		break;
	case 'r':
		deleteAll(mysql);
		break;
	case 'p':
		printposts(mysql);
		break;
	case 'u':
		printUsers(mysql);
		break;
	case 's':
		printStreams(mysql);
		break;
	case 'i':
		createAllTables(mysql);
		break;
	case 'h':
		usage();
		break;
	default:
		printf("Invalid option: %s\n", argv[1]);
		usage();
		break;
	}

	mysql_close(mysql);

	return 0;
}

/**
 * printUsers
 * Prints all of the user names in the databse
 * If a user is in multiple streams then it is
 * only printed once.
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *               -2  failed to load from DB
 *               -3  failed to load the data
 *               0   on success
 */
int printUsers(MYSQL* mysql)
{
	char* query;

	MYSQL_RES* results;
	MYSQL_ROW  row;

	/* make sure parameters are valid */
	if (mysql == NULL)
	{
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

	/* check if the table is empty */
	if (mysql_num_rows(results) == 0)
	{
		printf("table is empty.\n");
	}

	mysql_free_result(results);

	return 0;
}

/**
 * printStreams
 * Prints all of the streams in the databse
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *               -2  failed to load from DB
 *               -3  failed to load the data
 *               0   on success
 */
int printStreams(MYSQL* mysql)
{
	char* query;

	MYSQL_RES* results;
	MYSQL_ROW  row;

	/* make sure parameters are valid */
	if (mysql == NULL)
	{
		return -1;
	}

	query = strduplicate("SELECT DISTINCT stream_name FROM streams");

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

	/* check if the table is empty */
	if (mysql_num_rows(results) == 0)
	{
		printf("table is empty.\n");
	}

	mysql_free_result(results);

	return 0;
}

/**
 * printposts
 * Prints all of the posts in each stream. in the from:
 *
 * 		stream: ...
 *		userID: ...
 *		date: ...
 *		text: ...
 *		----------------------------------------
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *               -2  failed to load from DB
 *               -3  failed to load the data
 *                0  on success
 */
int printposts(MYSQL* mysql)
{
	char* query;

	MYSQL_RES* results;
	MYSQL_ROW  row;

	/* make sure parameters are valid */
	if (mysql == NULL)
	{
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

	/* check if the table is empty */
	if (mysql_num_rows(results) == 0)
	{
		printf("table is empty.\n");
	}

	mysql_free_result(results);
	return 0;
}

/**
 * clearAll
 * Deletes all of the data in the tables.
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *                0  on success
 */
int clearAll(MYSQL* mysql)
{
	/* make sure the parameters is valid */
	if (mysql == NULL)
	{
		return -1;
	}

	deleteFromTable_DB(mysql, "users", NULL);
	deleteFromTable_DB(mysql, "streams", NULL);
	deleteFromTable_DB(mysql, "posts", NULL);

	printf("Done.\n");
	return 0;
}

/**
 * deleteAll
 * Deletes all of the tables.
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *                0  on success
 */
int deleteAll(MYSQL* mysql)
{
	/* make sure the parameters is valid */
	if (mysql == NULL)
	{
		return -1;
	}

	dropTable_DB(mysql, "users");
	dropTable_DB(mysql, "streams");
	dropTable_DB(mysql, "posts");

	printf("Done.\n");
	return 0;
}

/**
 * createAllTables
 * Creates the 3 tables (if they do not exist)
 * The tables are case sensitive
 *
 * users:
 * 		ID		user_id		stream_name		num_read
 *   	int		text		text			int
 *	   	PK
 *
 * streams:
 * 		ID		stream_name		num_posts
 *   	int		text			int
 *	   	PK
 *
 * posts:
 * 		ID		stream_name		user_id		date		text
 *   	int		text			text		datetime	text
 *	   	PK
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *                0  on success
 */
int createAllTables(MYSQL* mysql)
{
	int status;
	/* make sure the parameters is valid */
	if (mysql == NULL)
	{
		return -1;
	}

	/* create the data lists */
	char* namesUsers[] = {"user_id", "stream_name", "num_read"};
	char* typesUsers[] = {"TEXT", "TEXT", "INT DEFAULT 0"};

	char* namesStreams[] = {"stream_name", "num_posts"};
	char* typesStreams[] = {"TEXT", "INT DEFAULT 0"};

	char* namesPosts[] = {"stream_name", "user_id", "date", "text"};
	char* typesPosts[] = {"TEXT", "TEXT", "DATETIME", "TEXT"};

	/* try to create the users table */
	status = createTable_DB(mysql, "users", 3, namesUsers, typesUsers);
	if (status != 0)
	{
		printf("Failed to create the users table.\n");
	}

	/* try to create the posts table */
	status = createTable_DB(mysql, "posts", 4, namesPosts, typesPosts);
	if (status != 0)
	{
		printf("Failed to create the posts table.\n");
	}

	/* try to create the streams table */
	status = createTable_DB(mysql, "streams", 2, namesStreams, typesStreams);
	if (status != 0)
	{
		printf("Failed to create the streams table.\n");
	}

	printf("Done.\n");
	return 0;
}

/**
 * usage
 * Prints the usage message for this program
 */
static void usage()
{
	printf("Usage: ./db\nuse to access the database\n");
	printf("  -clear : removes all of the posts, users, streams from the tables in the database\n");
	printf("  -reset : deletes the tables from the database\n");
	printf("  -posts : prints out all posts stored in the database\n");
	printf("  -users : prints out all user names stored in the database\n");
	printf("  -streams : prints out all stream names stored in the database\n");
	printf("  -init : Creates the users, streams, and posts tables\n");
	printf("  -help : displays this help message\n\n");
}

/**
 * parseOption
 * Parses the commndline argument into its character flag
 *
 * @param  option	the commandline argument
 * @return c		clear
 *         r		reset
 *         p		posts
 *         u		users
 *         s		streams
 *         h		help
 *         i		initilize
 *         o		unknown option
 */
static char parseOption(const char* option)
{
	/* make sure an option is given */
	if (option == NULL)
	{
		return 'o';
	}

	if (strcmp_nocase(option, "-clear") == 0)
	{
		return 'c';
	}

	if (strcmp_nocase(option, "-reset") == 0)
	{
		return 'r';
	}
	if (strcmp_nocase(option, "-posts") == 0)
	{
		return 'p';
	}
	if (strcmp_nocase(option, "-users") == 0)
	{
		return 'u';
	}
	if (strcmp_nocase(option, "-streams") == 0)
	{
		return 's';
	}
	if (strcmp_nocase(option, "-help") == 0)
	{
		return 'h';
	}
	if (strcmp_nocase(option, "-init") == 0)
	{
		return 'i';
	}

	return 'o';
}

