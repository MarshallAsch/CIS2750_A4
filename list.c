/****************************** list.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Contains all the functions for adding, searching and creating elements in
the list.
****************************************************************************/

#include "list.h"

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
List* addToListAll(List* list, char* name, char* newName, char* typeList, bool needsOBJ) {
	List* element;

	/* create the new element*/
	element = newElementAll(name, newName, typeList, needsOBJ);

	return add(list, element);
}

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
List* newElementAll(char* name, char* newName, char* typeList, bool needsOBJ) {
	List* element;

	/*  make sure parameter is valid */
	if (name == NULL) {
		return NULL;
	}

	element = malloc(sizeof(List));

	/* check if malloc failed*/
	if (element == NULL) {
		return NULL;
	}

	/* copy the rest of the data into the struct */
	element->name = strduplicate(name);
	element->newName = strduplicate(newName);

	if (typeList != NULL) {
		element->typeList = strduplicate(typeList);
	} else {
		element->typeList = strduplicate(" ");;
	}

	/* if something went wrong destroy it */
	if (element->name == NULL || element->newName == NULL) {
		freeList(element);
		return NULL;
	}

	/* set the parameters*/
	element->next = NULL;
	element->needsOBJ = needsOBJ;

	return element;
}

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
List* addToList(List* list, char* name, char* typeList) {
	List* element;

	/* check the parameters */
	if (name == NULL) {
		return list;
	}

	element = newElement(name, typeList);


	return add(list, element);
}

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
List* add(List* list, List* element) {
	List* temp;

	/* check if the list is empty */
	if (list == NULL) {
		return element;
	}

	/* make sure the element being added exists */
	if (element == NULL) {
		return list;
	}

	/*  find the last element in the list */
	temp = list;
	while (temp->next != NULL) {
		temp = temp->next;
	}

	/* add the element to the end */
	temp->next = element;
	return list;
}

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
List* newElement(char* name, char* typeList) {
	List* element;

	/*  make sure parameter is valid */
	if (name == NULL) {
		return NULL;
	}

	element = malloc(sizeof(List));

	/* malloc failed*/
	if (element == NULL) {
		return NULL;
	}

	element->name = strduplicate(name);
	element->newName = malloc(sizeof(char) * (strlen(name) + 2));

	if (typeList != NULL) {
		element->typeList = strduplicate(typeList);
	} else {
		element->typeList = strduplicate(" ");
	}

	/* if something went wrong destroy it */
	if (element->name == NULL || element->newName == NULL) {
		freeList(element);
		return NULL;
	}

	/* set the rest of the fields */
	(element->newName)[0] = '\0';
	element->next = NULL;
	element->needsOBJ = false;

	return element;
}

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
char* search(List* list, string name) {
	/* check the parameters */
	if (list == NULL || name == NULL) {
		return NULL;
	}

	/* check to see if the  name matches */
	if (strcmp(list->name, name) == 0) {
		return list->newName;
	}

	/* continue searching */
	return search(list->next, name);
}

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
char* getVarType(List* list, string name) {
	/* check the parameters */
	if (list == NULL || name == NULL) {
		return NULL;
	}

	/* check if the name matches */
	if (strcmp(list->name, name) == 0) {
		return list->typeList;
	}

	/* continue searching */
	return getVarType(list->next, name);
}

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
char* searchFunc(List* list, string name, char* typeList) {
	/* check the parameters */
	if (list == NULL || name == NULL) {
		return NULL;
	}

	/* set typelist if is NULL*/
	if (typeList == NULL) {
		typeList = " ";
	}

	/* check if it is found */
	if (strcmp(list->name, name) == 0 && strcmp(list->typeList, typeList) == 0) {
		return list->newName;
	}

	/* keep searching */
	return searchFunc(list->next, name, typeList);
}

/**
 * freeList
 * Recusrivly free the list and all the data that is used by it
 *
 * IN:	list, the head of the list to free
 * OUT: none
 * POST: frees all the memory used by the list
 * ERROR: none
 */
void freeList(List* list) {
	/* check the parameter */
	if (list == NULL) {
		return;
	}

	/* the the elements data*/
	free(list->name);
	free(list->newName);
	free(list->typeList);

	freeList(list->next);

	free(list);
}

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
void printList(List* list) {
	/* check if parameter is valid */
	if (list == NULL) {
		return;
	}

	/* Print the name of the element in the list*/
	printf("%s\n", list->name);

	printList(list->next);
}

/**
 * addNewName
 * Adds a new name to an element in the list
 * DEPRECATED
 *
 * IN:	list, the head of the list to search
 *		oldName, the name of the element
 *		newName, the new name to add to the element
 * OUT: true on success
 *		false on failure
 * POST: adds the new name to the element
 * ERROR: false if the parameters are not valid
 */
bool addNewName(List* list, char* oldName, char* newName) {
	List* temp;

	/* check the parameters are valid */
	if (list == NULL || oldName == NULL || newName == NULL) {
		return false;
	}

	/* search for the element with the matching old name */
	temp = list;
	while (temp !=  NULL && strcmp(temp->name, oldName) != 0) {
		temp = temp->next;
	}

	/* make sure that it didnt run off the end of the list */
	if (temp == NULL) {
		return false;
	}

	free(temp->newName);
	temp->newName = strduplicate(newName);

	return true;
}

/**
 * setNeedsOBJ
 * Sets that the function requires the object to true
 * DEPRECATED
 *
 * IN:	list, the head of the list to search
 *		name, the name of the element
 * OUT: true on success
 *		false on failure
 * POST: the needsOBJ field is set to true
 * ERROR: false if the parameters are not valid
 */
bool setNeedsOBJ(List* list, char* name) {
	List* temp;

	/* check parameters */
	if (list == NULL || name == NULL) {
		return false;
	}

	/* search for the element with the matching old name */
	temp = list;
	while (temp !=  NULL && strcmp(temp->newName, name) != 0) {
		temp = temp->next;
	}

	/* make sure that it didnt run off the end of the list */
	if (temp == NULL) {
		return false;
	}

	temp->needsOBJ = true;
	return true;
}

/**
 * getNeedsOBJ
 * Recursivly search the list for the name
 *
 * IN:	list, the list to search
 *		name, the name to search for
 * OUT: returns wether or not the element needs obj if it is found
 *		false if it is not found
 * POST: none
 * ERROR: false if something goes wrong
 */
bool getNeedsOBJ(List* list, char* name) {
	List* temp;

	/* check that the parameters are valid */
	if (list == NULL || name == NULL) {
		return false;
	}

	/* search for the element with the matching old name */
	temp = list;
	while (temp !=  NULL && strcmp(temp->newName, name) != 0) {
		temp = temp->next;
	}

	/* make sure that it didnt run off the end of the list */
	if (temp == NULL) {
		return false;
	}

	return temp->needsOBJ;
}

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
List* addToHead(List* list, char* name, char* typeList) {
	List* element;

	element = newElement(name, typeList);

	return addFront(list, element);
}

/**
 * addFront
 * add the element to he end of the list
 * IN: list, the list it is added too
 *		element, the element that is being added
 * OUT: the head of the list
 * POST: element is added to the end of the list
 * ERROR: NULL if soemthing goes wrong
 */
List* addFront(List* list, List* element) {
	if (element == NULL) {
		return list;
	}

	element->next = list;
	return element;
}

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
void freeLimit(List* toFree, List* limit) {
	/* check to make sure it has not reached the end of the list */
	if (toFree == NULL) {
		return;
	}

	/* if it has reached the limit of elements to free */
	if (toFree == limit) {
		return;
	}

	free(toFree->name);
	free(toFree->newName);

	free(toFree->typeList);
	freeLimit(toFree->next, limit);

	free(toFree);
}
