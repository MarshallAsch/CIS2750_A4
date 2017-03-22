/****************************** lists.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Used to make it easer to pass all the lists into other functions
****************************************************************************/


#include "lists.h"

/**
 * newList
 * allocates memory for a the 3 lists, func, global and classvar
 *
 * IN: none
 * OUT: Lists, a pointer to the set of lists
 * POST: creates the lsits in memory
 * ERROR: NULL on failure
 */
Lists* newLists() {
	Lists* lists;

	lists = malloc(sizeof(Lists));

	/* check that malloc worked */
	if (lists == NULL) {
		return NULL;
	}

	/* init each list to NULL*/
	lists->globalList	= NULL;
	lists->funcList		= NULL;
	lists->classVar		= NULL;

	return lists;
}

/**
 * freeLists
 * Destroy all the lists and free all the memory that they used
 *
 * IN: list, the set of lists to free
 * OUT: none
 * POST: memory is freed
 * ERROR: none
 */
void freeLists(Lists* list) {
	/* check parameter */
	if (list == NULL) {
		return;
	}

	/* free sub lists */
	freeList(list->globalList);
	freeList(list->funcList);
	freeList(list->classVar);

	/* set to NULL so if it gets freed again it does not cause problems */
	list->globalList = NULL;
	list->funcList = NULL;
	list->classVar = NULL;

	/* free list holder */
	free(list);
}
