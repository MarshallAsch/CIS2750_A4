/****************************** list.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Contains all the functions for adding, searching and creating elements in
the list.
****************************************************************************/


#ifndef __LIST_H__
#define __LIST_H__

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mystring.h"



/**
 * The struct to hold the elements of the list
 *
 */
typedef struct List {
	char* name;
	char* newName;

	char* typeList;

	bool needsOBJ;

	struct List* next;
} List;


/**
 * addToListAll
 * Adds all the elements info to the end of the list
 *
 * IN:	list, the list it is being added to
 *		name, the old name
 *		newName, the new name of the element
 *		typeList, the datatype or list of types of the element
 *		needsOBJ, wether ot not it needs the object as a parameter for the function
 * OUT: the head of the list on success
 *		NULL, on failure
 * POST: creates a new element and it gets added to the list
 * ERROR: NULL if the element can not be created or added
 */
List* addToListAll(List* list, char* name, char* newName, char* typeList, bool needsOBJ);

/**
 * newElementAll
 * Create the new element with all the information

 * IN:	name, the old name
 *		newName, the new name of the element
 *		typeList, the datatype or list of types of the element
 *		needsOBJ, wether ot not it needs the object as a parameter for the function
 * OUT: the pointer to the element on success
 *		NULL, on failure
 * POST: new element is created in memory
 * ERROR: NULL on error
 */
List* newElementAll(char* name, char* newName, char* typeList, bool needsOBJ);

/**
 * addToList
 * Add the element to the end of the list,
 * does nto use all the info to create the element
 *
 * IN:	list, the list to be added to
 *		name, the name of the element
 *		typeList, the datatype of the element
 * OUT: pointer to the head of the list on success
 *		NULL on failure
 * POST: creates a element in memory and adds it to the list
 * ERROR: NULL on error
 */
List* addToList(List* list, char* name, char* typeList);

/**
 * add
 * Add the element to the end of the list
 *
 * IN:	list, the list it is added too
 *		element, the element that is being added
 * OUT: the head of the list
 * POST: element is added to the end of the list
 * ERROR: NULL if soemthing goes wrong
 */
List* add(List* list, List* element);

/**
 * newElement
 * Creates a new element with some of the info
 * The rest of the fields are left NULL
 *
 * IN:	name, the name of the element
 *		typeList, the data tyoe for the element
 * OUT: returns pointer to the new element on success
 *		NULL on failure
 * POST: creates a new element in memory
 * ERROR: NULL on failure
 */
List* newElement(char* name, char* typeList);

/**
 * search
 * Recursivly search the list for the name
 *
 * IN:	list, the list to search
 *		name , the name to search for
 * OUT: returns the elements newName if it is found
 *		NULL if it is not found
 * POST: none
 * ERROR: NULL if soemthing goes wrong
 */
char* search(List* list, string name);

/**
 * getVarType
 * Recursivly search the list for the name
 *
 * IN:	list, the list to search
 *		name , the name to search for
 * OUT: returns the elements type if it is found
 *		NULL if it is not found
 * POST: none
 * ERROR: NULL if soemthing goes wrong
 */
char* getVarType(List* list, string name);

/**
 * searchFunc
 * Recursivly search the list for the name and typeList
 *
 * IN:	list, the list to search
 *		name , the name to search for
 *		typeList, the type of the element it is searching for
 * OUT: returns the elements newName if it is found
 *		NULL if it is not found
 * POST: none
 * ERROR: NULL if soemthing goes wrong
 */
char* searchFunc(List* list, string name, char* typeList);

/**
 * freeList
 * Recusrivly free the list and all the data that is used by it
 *
 * IN:	list, the head of the list to free
 * OUT:	none
 * POST: frees all the memory used by the list
 * ERROR: none
 */
void freeList(List* list);

/**
 * printList
 * Recusrivly print the name of each element in the list
 * USED FOR TESTING
 *
 * IN:	list, the head of the list to free
 * OUT: none
 * POST: prints the data to stdout
 * ERROR: none
 */
void printList(List* list);

/**
 * addNewName
 * Adds a new name to an element in the list
 * DEPRECATED
 *
 * IN:	list, the head of the list to search
 *		oldName, the name of the element
 *		newName, the new name to add to the element
 * OUT: TRUE on success
 *		FALSE on failure
 * POST: adds the new name to the element
 * ERROR: FALSE if the parameters are not valid
 */
bool addNewName(List* list, char* oldName, char* newName);

/**
 * setNeedsOBJ
 * Sets that the function requires the object to true
 * DEPRECATED
 *
 * IN:	list, the head of the list to search
 *		name, the name of the element
 * OUT: TRUE on success
 *		FALSE on failure
 * POST: the needsOBJ field is set to TRUE
 * ERROR: FALSE if the parameters are not valid
 */
bool setNeedsOBJ(List* list, char* name);

/**
 * getNeedsOBJ
 * Recursivly search the list for the name
 *
 * IN:	list, the list to search
 *		name, the name to search for
 * OUT: returns wether or not the element needs obj if it is found
 *		FALSE if it is not found
 * POST: none
 * ERROR: FALSE if something goes wrong
 */
bool getNeedsOBJ(List* list, char* name);

/**
 * addToHead
 * Adds the element to the head of the list
 *
 * IN:	list, the list it is added too
 *		name, the name of the element
 *		typeList, the type of the elemtnt being added
 * OUT: the head of the list
 * POST: element is added to the start of the list
 * ERROR: NULL if soemthing goes wrong
 */
List* addToHead(List* list, char* name, char* typeList);

/**
 * addToHead
 * Adds the element to the head of the list
 *
 * IN:	list, the list it is added too
 *		name, the name of the element
 *		typeList, the type of the elemtnt being added
 * OUT: the head of the list
 * POST: element is added to the start of the list
 * ERROR: NULL if soemthing goes wrong
 */
List* addFront(List* list, List* element);

/**
 * freeLimit
 * Frees the elements of the list up untill a certain point
 *
 * IN:	list, the list it is added too
 *		limit, where to stop freeing the list
 * OUT: none
 * POST: frees a section of the list
 * ERROR: none
 */
void freeLimit(List* toFree, List* limit);

#endif
