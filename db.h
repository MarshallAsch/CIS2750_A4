/****************************** db.h ***********************************
Student Name: Marshall Aaron Asch			Student Number:  0928357
Date: March 22, 2017						Course Name: CIS*2750
Assignment: A4

The program for accesing the database.
can create, destroy and view the tables.

Takes in 1 commandline argument which is the acction it will do.

****************************************************************************/

#ifndef __DB_H__
#define __DB_H__

#include <mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mystring.h"
#include "dbAccess.h"


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
int printUsers(MYSQL* mysql);

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
int printStreams(MYSQL* mysql);

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
int printposts(MYSQL* mysql);

/**
 * clearAll
 * Deletes all of the data in the tables.
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *                0  on success
 */
int clearAll(MYSQL* mysql);

/**
 * deleteAll
 * Deletes all of the tables.
 *
 * @param  mysql MSQL refrence struct
 * @return       -1  invalid param
 *                0  on success
 */
int deleteAll(MYSQL* mysql);

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
int createAllTables(MYSQL* mysql);

#endif
