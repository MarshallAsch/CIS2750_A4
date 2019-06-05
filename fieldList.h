/****************************** fieldList.h ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: March 14, 2017                    	 Course Name: CIS*2750
Assignment: A3

Functions that create and access the field list to store the input
fields of teh forms.

****************************************************************************/

#ifndef __FIELD_LIST_H__
#define __FIELD_LIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "mystring.h"
#include "myboolean.h"



typedef struct FieldList {

	char* name;
	char* prompt;
	char* defaultValue;
	char* other;

	bool required;

	struct FieldList * next;
} FieldList;

/**
 * newField
 * Creates a struct used for creating the form input field.
 * Copies the strings into the struct. They must be freed by the caller.
 * The parameters can be NULL if they are to be left blank.
 *
 * to impove add a type / check/ selected
 *
 * IN:	name, the field
 *		prompt, the prompt for the field
 *		defaultValue, the initilal value of the field
 *		required, if the field is required
 *		other, any aditional fields
 * OUT: a pointer to the struct on success
 *		NULL on failure
 * POST: generates the struct
 * ERROR: NULL if mem can not be allocated
 */
FieldList* newField(char* name, char* prompt, char* defaultValue, bool required, char* other);

/**
 * addToList
 * Adds the form field to the list of input fields for the form
 * gets added to the end of the list
 *
 * IN:	list, the list to be added to
 *		new, the new field that is being added
 * OUT: The new head of the list
 * POST: The element gets added to the end of the list
 * ERROR: NULL if both new and list are NULL
 */
FieldList* addToList(FieldList* list, FieldList* new);

/**
 * destroyFieldList
 * Recursivly frees all the memory being
 * used by the list
 *
 * IN:	list, the list being freed
 * OUT: none
 * POST: all the memory gets freed
 * ERROR: none
 */
void destroyFieldList(FieldList* list);

#endif
