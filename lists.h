/****************************** lists.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Used to make it easer to pass all the lists into other functions
****************************************************************************/

#ifndef __LISTS_H__
#define __LISTS_H__

#include "list.h"


/**
 * The struct to hold the elements of the list
 *
 */
typedef struct Lists {
	List* globalList;
	List* funcList;
	List* classVar;

} Lists;


/**
 * newList
 * allocates memory for a the 3 lists, func, global and classvar
 *
 * IN: none
 * OUT: Lists, a pointer to the set of lists
 * POST: creates the lsits in memory
 * ERROR: NULL on failure
 */
Lists* newLists();

/**
 * freeLists
 * Destroy all the lists and free all the memory that they used
 *
 * IN: list, the set of lists to free
 * OUT: none
 * POST: memory is freed
 * ERROR: none
 */
void freeLists(Lists* list);

#endif
