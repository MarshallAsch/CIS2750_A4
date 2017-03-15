
#include "dbAccess.h"




MYSQL* initSQL()
{
	MYSQL* mysql;

	mysql = NULL;

	/* initilize the connection */
	mysql_init(mysql);
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
	if (mysql == NULL || tableName == NULL || name == NULL || type == NULL || numCol < 1) {
		return -1;
	}

	/* create the query */
	query = strduplicate("CREATE TABLE ");
	query = join(query, tableName);
	query = join(query, " (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, ");

	/* create the columns of the table */
	for (i = 0; i < numCol-1; i++)
	{
		query = join(query, name[i]);
		query = joinC(query, ' ');
		query = join(query, type[i]);
		query = join(query, ", ");
	}

	query = join(query, name[numCol-1]);
	query = joinC(query, ' ');
	query = join(query, type[numCol-1]);
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
	if (mysql == NULL || tableName == NULL || field == NULL || value == NULL || numData < 1) {
		return -1;
	}

	/* create the query */
	query = strduplicate("INSERT INTO ");
	query = join(query, tableName);
	

	/* enter the column name */
	for (i = 0; i < numCol-1; i++)
	{
		query = join(query, field[i]);
		query = join(query, ", ");
	}
	query = join(query, field[numCol-1]);
	query = join(query, ") VALUES (");


	/* enter the values */
	for (i = 0; i < numCol-1; i++)
	{
		query = join(query, value[i]);

		query = join(query, ", ");
	}
	query = join(query, value[numCol-1]);
	query = joinC(query, ')');


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
	if (mysql == NULL || tableName == NULL || field == NULL || value == NULL || numData < 1) {
		return -1;
	}

	/* create the query */
	query = strduplicate("UPDATE ");
	query = join(query, tableName);
	query = join(query, " JOIN ");

	/* enter the column name */
	for (i = 0; i < numCol-1; i++)
	{
		query = join(query, field[i]);
		query = join(query, " = ");
		query = join(query, value[i]);
		query = join(query, ", ");
	}
	query = join(query, field[numCol-1]);
	query = join(query, " = ");
	query = join(query, value[numCol-1]);

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

int clearTable(MYSQL* mysql, char* tableName)
{
	char* query;

	/* make sure that param are valid */
	if (mysql == NULL || tableName == NULL) {
		return -1;
	}

	query = strduplicate("DELETE * FROM ");
	query = join(query, tableName);


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

int deleteTable(MYSQL* mysql, char* tableName)
{
	char* query;

	/* make sure that param are valid */
	if (mysql == NULL || tableName == NULL) {
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
 * Prints the error message
 *
 */
void error(char* msg, MYSQL* mysql)
{
	fprintf(stderr, "%s\n%s\n", msg, mysql_error(mysql));
}


/******************

	CONSTRUCTORS

********************/


SQL_users_result* new newUserResult(char* id, char* userID, char* stream, char* numRead)
{
	SQL_users_result* result;

	/* make sure param are valid */
	if (id == NULL || userID == NULL || stream == NULL || numRead == NULL) {
		return NULL;
	}

	/* allocate memory for the structure */
	result = malloc(sizeof(SQL_users_result));
	if (result == NULL) {
		return NULL;
	}

	/* set the data */
	result->id = atoi(id);
	result->user_id = strduplicate(userID);
	result->stream_name = strduplicate(stream);
	result->num_read = atoi(munRead);

	return result;
}

SQL_streams_result* new newStreamResult(char* id, char* stream, char* numPosts)
{
	SQL_streams_result* result;

	/* make sure param are valid */
	if (id == NULL || stream == NULL || numPosts == NULL) {
		return NULL;
	}

	/* allocate memory for the structure */
	result = malloc(sizeof(SQL_streams_result));
	if (result == NULL) {
		return NULL;
	}

	/* set the data */
	result->id = atoi(id);
	result->stream_name = strduplicate(stream);
	result->num_posts = atoi(numPosts);

	return result;
}

SQL_post_result* new newPostResult(char* id, char* userID, char* stream, char* date, char* text)
{
	SQL_post_result* result;

	/* make sure param are valid */
	if (id == NULL || userID == NULL || stream == NULL || date == NULL || text == NULL) {
		return NULL;
	}

	/* allocate memory for the structure */
	result = malloc(sizeof(SQL_post_result));
	if (result == NULL) {
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
	if (result == NULL) {
		return NULL;
	}

	/* allocate memory */
	data = malloc(sizeof(void*)*(result->numRows + 1));
	if (data == NULL) {
		free(result);
		return;
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

void freeUserResults(SQL_users_result* result)
{
	/* make sure the parameter is valid */
	if (result == NULL) {
		return;
	}

	free(result->user_id);
	free(result->stream_name);
	free(result);
}

void freeStreamResults(SQL_streams_result* result)
{
	/* make sure the parameter is valid */
	if (result == NULL) {
		return;
	}

	free(result->stream_name);
	free(result);
}

void freePostResults(SQL_post_result* result)
{
	/* make sure the parameter is valid */
	if (result == NULL) {
		return;
	}

	free(result->stream_name);
	free(result->user_id);
	free(result->date);
	free(result->text);
	free(result);
}


void addData(SQL_result* result, void* next)
{
	/* make sure the parameters are valid */
	if (result == NULL || result->data == NULL || next == NULL) {
		return;
	}

	result->data[result->current++] = next;
}





SQL_result* getUserStreams(MYSQL* mysql, char* userID)
{
	char* query;
	SQL_result* result;
	SQL_users_result* users;

	MYSQL_RES* res;
	MYSQL_ROW  row;

	/* check the parameters */
	if (mysql == NULL || userID == NULL) {
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
	return result;
}


SQL_result* getStreamPosts(MYSQL* mysql, char* stream)
{
	char* query;
	SQL_result* result;
	SQL_post_result* post;

	MYSQL_RES* res;
	MYSQL_ROW  row;

	/* check the parameters */
	if (mysql == NULL || stream == NULL) {
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
	return result;
}






