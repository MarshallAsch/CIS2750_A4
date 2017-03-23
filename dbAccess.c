
#include "dbAccess.h"




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


int createTable(MYSQL* mysql, char* tableName, int numCol, char** name, char** type)
{
	int i;				/* loop var */
	char* query;

	/* make sure the parameters are valid */
	if (mysql == NULL || tableName == NULL || name == NULL || type == NULL || numCol < 1)
	{
		return -1;
	}

	/* create the query */
	query = strduplicate("CREATE TABLE ");
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
	query = joinC(query, ')');

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

int insert(MYSQL* mysql, char* tableName, int numData, char** field, char** value)
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

int update(MYSQL* mysql, char* tableName, int numData, char** field, char** value)
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

int deleteFromTable(MYSQL* mysql, char* tableName, char* condition)
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

int dropTable(MYSQL* mysql, char* tableName)
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


int newStream(MYSQL* mysql, char* streamName)
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

	num = count(mysql, "streams", condition);
	free(condition);

	char* namesStreams[] = {"stream_name", "num_posts"};
	char* data[] = {streamName, "0"};

	/* if it does not exist then add it */
	if (num == 1)
	{
		return 0;
	}
	else if (num == 0)
	{
		return insert(mysql, "streams", 2, namesStreams, data);
	}
	else
	{
		return num;
	}
}



int addUser(MYSQL* mysql, char* streamName, char* userID)
{
	char* condition;
	int num;
	/* make sure that param are valid */
	if (mysql == NULL || streamName == NULL || userID == NULL)
	{
		return -1;
	}

	/* check to see if the user is already in the stream */
	condition = strduplicate("stream_name = \"");
	condition = join(condition, streamName);
	condition = join(condition, "\" AND user_id = \"");
	condition = join(condition, userID);
	condition = join(condition, "\"");

	num = count(mysql, "users", condition);
	free(condition);

	char* namesStreams[] = {"user_id", "stream_name", "num_read"};
	char* data[] = {userID, streamName, "0"};

	/* if it does not exist then add it */
	if (num == 1)
	{
		return 0;
	}
	else if (num == 0)
	{
		return insert(mysql, "users", 3, namesStreams, data);
	}
	else
	{
		return num;
	}
}

int removeUser(MYSQL* mysql, char* streamName, char* userID)
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
	status = deleteFromTable(mysql, "users", condition);
	free(condition);

	return status;
}


int count(MYSQL * mysql, char* tableName, char* condition)
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

	/* print the userIDs */
	while ((row = mysql_fetch_row(results)))
	{
		printf("%s\n", row[0]);
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
 * Prints the error message
 *
 */
void error(char* msg, MYSQL * mysql)
{
	fprintf(stderr, "%s\n%s\n", msg, mysql_error(mysql));
}


/******************

	CONSTRUCTORS

********************/


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
 * The data needs to be filled and freed by caller
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


void addData(SQL_result * result, void* next)
{
	/* make sure the parameters are valid */
	if (result == NULL || result->data == NULL || next == NULL)
	{
		return;
	}

	result->data[result->current++] = next;
}





SQL_result* getUserStreams(MYSQL * mysql, char* userID)
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
	query = strduplicate("SELECT * FROM users WHERE user_id = \"");
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


SQL_result* getStreamPosts(MYSQL * mysql, char* stream)
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
	query = strduplicate("SELECT * FROM posts WHERE stream_name = \"");
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






