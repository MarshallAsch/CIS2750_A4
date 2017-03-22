/****************************** mysting.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Version 2

Part of the library of functions that contain the functions to deal
with strings
****************************************************************************/
#include "mystring.h"

#include <signal.h>

/**
 * toLower
 * Takes a string as input and returns a new string that is a copy
 * of the origonal but is in lowercase. if overwrite is set to TRUE (1)
 * then the origonal is freed, otherwise the caller mist free the string.
 *
 * IN:	str, the sring that it is going to make lowercase
 *		overwrite, wether or not the orgional gets destroyed
 * OUT: pointer to the new string on success
 *		NULL on failure
 * POST: a new string is created in memory
 * ERROR: NULL if the parameter is invalid or mem could not be allocated
 */
char* toLower(char* str, bool overwrite)
{
	char * new;
	int i;			/*   loop variable */
	int length;

	new = strduplicate(str);

	/* make sure that string duplicate was successfull */
	if (new == NULL)
	{
		return NULL;
	}

	length = strlen(new);

	/* loop through entire string */
	for (i = 0; i < length; i++)
	{
		new[i] = tolower(new[i]);
	}

	/* destroy the origonal */
	if (overwrite == TRUE)
	{
		free(str);
	}

	return new;
}

/**
 * join
 * Joins 2 strings together. src is put after
 * dest, src needs to be freed by the caller.
 * dest is destroyed.
 *
 * IN:	dest, the string that is being added to
 *		src, the string that is being added
 * OUT: NULL, on failure
 *		pointer to the new string on success
 * POST: the old dest gets freed and the new string is created
 * ERROR: NULL if something goes wrong.
 */
char* join(char* dest, const char* src)
{
	char* new;

	/* make sure that there is a source to add */
	if (src == NULL)
	{
		return dest;
	}

	/* if dest is NULL then make a copy of the sorce to return */
	if (dest == NULL)
	{
		new = malloc (sizeof(char) * (strlen(src) + 2));

		if (new == NULL)
		{
			return NULL;
		}

		/* put into new string */
		strcpy(new, src);
		return new;
	}

	/* join the 2 strings*/
	new = malloc (sizeof(char) * (strlen(dest) + strlen(src) + 2));

	if (new == NULL)
	{
		return dest;
	}

	/* put into new string */
	sprintf(new, "%s%s", dest, src);

	free(dest);

	return new;
}

/**
 * joinC
 * Joins a character to the end of the string
 *
 * IN:	dest, the string that is being added to
 *		src, the character that is being added
 * OUT: a new string that contains both the old and the new
 * POST: makes a string
 * ERROR: NULL if something goes wrong
 */
char* joinC(char* dest, char src)
{
	char* new;

	/* if dest is NULL then make a copy of the sorce to return */
	if (dest == NULL)
	{
		new = malloc (sizeof(char) * 2);

		if (new == NULL)
		{
			return NULL;
		}

		/* put into new string */
		sprintf(new, "%c", src);

		return new;
	}

	new = malloc (sizeof(char) * (strlen(dest) + 2));

	if (new == NULL)
	{
		return dest;
	}

	/* put into new string */
	sprintf(new, "%s%c", dest, src);

	free(dest);

	return new;
}

/**
 * strduplicate
 * Copies a sting into a new string
 *
 * IN: 	str, the sting to duplicate
 * OUT: NULL, on failure
 *		returns a pointer to the new string
 * POST: creates a new string in memory
 * ERROR: NULL if mem can not be allocated
 */
char* strduplicate(const char* str)
{
	char* new;

	/* make sure that the parameter is valid */
	if (str == NULL)
	{
		return NULL;
	}

	/* allocate new memory for the string */
	new = malloc(sizeof(char) * (strlen(str) + 1));
	if (new == NULL)
	{
		return NULL;
	}

	strcpy(new, str);

	return new;
}

/**
 * substring
 * Creates a new string from the old string.
 * ranges from start inclusive to end inclusive.
 *
 * IN: 	input, the base string
 *		start, the start index
 *		end, the end index of the substring
 * OUT: a pointer to the new string on success
 *		NULL pointer on failure
 * POST: a new smaller string is created
 * ERROR: NULL if parameters are not valid or
 *		  mem can not be allocated
 */
char* substring(char* input, int start, int end)
{
	char* temp;
	int i;			/* loop variable */
	int j;			/* loop variable */

	/* check the parameters */
	if (input == NULL || start < 0 || end > strlen(input) - 1 || end - start < 0)
	{
		return NULL;
	}

	/* allocate memory for the new string */
	temp = malloc(sizeof(char) * (end - start + 2));
	if (temp == NULL)
	{
		return NULL;
	}

	/* copy the section into the new string*/
	for (i = 0, j = start; j < end + 1; i++, j++)
	{
		temp[i] = input[j];
	}

	/* make sure it is NULL terminated */
	temp[i] = '\0';

	return temp;
}

/**
 * lastIndex
 * Gets the last index of the given character in the string.
 *
 * IN:	str, the string that is being searched
 *		ch, the char that it is searching for
 * OUT: returns the index of last time the character apears
 *		-1 on failure or if the characer is not in the string
 * POST: none
 * ERROR: -1 if the string does not exist
 */
int lastIndex(char* str, char ch)
{
	int length;
	int i;			/* loop variable */
	int pos;

	/* check that the string exists */
	if (str == NULL)
	{
		return -1;
	}

	length = strlen(str);
	pos = -1;

	/* loop through the entire string */
	for (i = 0; i < length; i++)
	{
		/* if the current character is the one looking for */
		if (str[i] == ch)
		{
			pos = i;
		}
	}

	return pos;
}

/**
 * firstIndex
 * Gets the index of the first occurance of the given
 * character in the string.
 *
 * IN:	str, the string that is being searched
 *		ch, the char that it is searching for
 * OUT: returns the index of first time the character apears
 *		-1 on failure or if the characer is not in the string
 * POST: none
 * ERROR: -1 if the string does not exist
 */
int firstIndex(char* str, char ch)
{
	int length;
	int i;			/* loop variable */
	int pos;

	/* check that the string exists */
	if (str == NULL)
	{
		return -1;
	}

	length = strlen(str);
	pos = -1;

	/* loop through the entire string */
	for (i = 0; i < length; i++)
	{
		/* if the current character is the one looking for */
		if (str[i] == ch)
		{
			pos = i;
			break;
		}
	}

	return pos;
}

/**
 * firstIndexOffset
 * Gets the index of the first occurance of the given
 * character in the string after the offset index
 *
 * IN:	str, the string that is being searched
 *		ch, the char that it is searching for
 *		offset, how far into the string to start the search
 * OUT: returns the index of first time the character apears
 *		-1 on failure or if the characer is not in the string
 * POST: none
 * ERROR: -1 if the string does not exist
 */
int firstIndexOffset(char* str, char ch, int offset)
{
	int length;
	int i;			/* loop variable */
	int pos;

	/* check that param are valid */
	if (str == NULL || strlen(str) <= offset )
	{
		return -1;
	}

	length = strlen(str);
	pos = -1;

	/* loop through the entire string */
	for (i = offset; i < length; i++)
	{
		/* if the current character is the one looking for */
		if (str[i] == ch)
		{
			pos = i;
			break;
		}
	}

	return pos;
}

/**
 * trim
 * Removes any leading or trailing white space characters
 * Makes a copy and destroyes the origonal.
 *
 * IN: 	input, the string that is being trimmed
 * OUT: NULL on failure
 *      a pointer the the new string that has been trimmed
 * POST: origonal memory is freed, new memory is allocated
 * ERROR: if input does not exist returns NULL
 */
char* trim(char* input)
{
	char* temp;
	int length;
	int start;
	int end;
	int i;				/* loop variable*/

	/* make sure that the input exists */
	if (input == NULL)
	{
		return NULL;
	}

	length = strlen(input);
	start = -1;
	end = -1;

	/* find the start of the string after the leading whitespace */
	for (i = 0; i < length; i++)
	{
		/* if the character is not whitespace */
		if (isspace(input[i]) == 0)
		{
			start = i;
			break;
		}
	}

	/* find the end of the string before the trailing whitespace */
	for (i = length - 1; i >=  0; i--)
	{
		/* if the character is not whitespace */
		if (isspace(input[i]) == 0)
		{
			end = i;
			break;
		}
	}

	/* something went wrong and there is only whitespace */
	if (start == -1 || end == -1)
	{
		free(input);
		return NULL;
	}

	temp = substring(input, start, end);

	free(input);

	/* there are no leading or trailing spaces */
	return temp;
}


/**
 * strcmp_nocase
 * Does a case insensitive string comparison.
 * If ether paramater is NULL then it segfaults.
 *
 *	if Return value < 0 then it indicates str1 is less than str2.
 *	if Return value > 0 then it indicates str2 is less than str1.
 *	if Return value = 0 then it indicates str1 is equal to str2.
 *
 * IN: 	str1, This is the first string to be compared.
 *		str2, This is the second string to be compared.
 * OUT: number
 * POST: none
 * ERROR: SEGFAULT on error
 */
int strcmp_nocase(const char* str1, const char* str2)
{
	int status;

	char* aLower;
	char* bLower;

	/* segfault if param are invalid */
	if (str1 == NULL || str2 == NULL)
	{
		raise(SIGSEGV);
	}

	aLower = strduplicate(str1);
	bLower = strduplicate(str2);

	/* convert to lowercase */
	aLower = toLower(aLower, TRUE);
	bLower = toLower(bLower, TRUE);

	status = strcmp(aLower, bLower);

	free(aLower);
	free(bLower);

	return status;
}


/**********************************
 *
 *		FOR THE LIST STRUCTURE
 *
 **********************************/


/**
 * newStringList
 * Creates a new string list element from the given string.
 * str is copied and needs to be freed by the caller
 *
 * IN: 	str, the string of the element
 * OUT:	a pointer the the new string list element
 * 		NULL on failure
 * POST: new string list is created
 * ERROR: if malloc fails return NULL
 */
StringList * newStringList(char* str)
{
	StringList * new;

	/* alocate memory */
	new = malloc(sizeof(StringList));
	if (new == NULL)
	{
		return NULL;
	}

	/* set the fields of the struct */
	new->next = NULL;
	new->str = strduplicate(str);

	return new;
}

/**
 * addToStringList
 * Adds the new element to the head of the list
 *
 * IN: 	list, the list it is being added too
 *		new, the element eing added to the list
 * OUT:	the new head of the list
 * POST: element is added to the head of the list
 * ERROR: none
 */
StringList * addToStringList(StringList* list, StringList * new)
{
	/* check parameters */
	if (list == NULL)
	{
		return new;
	}

	if (new == NULL)
	{
		return list;
	}

	/* add to the head of the list */
	new->next = list;
	return new;
}

/**
 * addToStringListEnd
 * Adds the new element to the tail of the list
 *
 * IN: 	list, the list it is being added too
 *		new, the element eing added to the list
 * OUT:	the new head of the list
 * POST: element is added to the tail of the list
 * ERROR: none
 */
StringList * addToStringListEnd(StringList* list, StringList * new)
{
	StringList* temp;

	/* check parameters */
	if (list == NULL)
	{
		return new;
	}

	if (new == NULL)
	{
		return list;
	}

	temp = list;

	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	/* add to the tail of the list */
	temp->next = new;
	return list;
}

/**
 * destroyStringList
 * Adds the new element to the head of the list
 *
 * IN: 	stream, the list to be freed
 * OUT:	none
 * POST: all the memory used is freed
 * ERROR: none
 */
void destroyStringList(StringList * stream)
{
	/* check that the parameter is valid */
	if (stream == NULL)
	{
		return;
	}

	/* free the rest of the nodes in the list */
	destroyStringList(stream->next);

	/* free this nodes data*/
	free(stream->str);
	free(stream);
}

/**********************************
 *
 *			NOT ALLWAYS USED
 *
 **********************************/

/**
 * createFuncPtr
 * Creates the function pointer to go in the class
 *
 * IN:	funcName, the new name of the function
 *		typeList, the list of the perameter types of the function
 *		className, the name of the class it is a part of
 *		funcType, the return type of the function
 *		needsOBJ, boolean weather it needs the class OBJ to be added as a param
 * OUT: returns the string of the function pointer to be added on success
 *		NULL on failure
 * POST: creates a string
 * ERROR: NULL if something goes wrong
 */
char* createFuncPtr(char* funcName, char* typeList, char* className, char* funcType, bool needsOBJ)
{
	char* new;

	/* make sure parameter */
	if (funcName == NULL || funcType == NULL || className == NULL)
	{
		return NULL;
	}

	new = strduplicate(funcType);
	new = join(new, " (*");
	new = join(new, funcName);
	new = join(new, " )(");

	/* if there are other paramerters and it needs the object*/
	if (typeList != NULL)
	{
		/* add the class OBJ type if needed */
		if (needsOBJ == TRUE)
		{
			new = join(new, "struct ");
			new = join(new, className);
			new = join(new, " *, ");
		}

		new = join(new, typeList);
		new[strlen(new) - 2] = '\0';
	}
	else
	{
		/* add the class OBJ type if needed */
		if (needsOBJ == TRUE)
		{
			new = join(new, "struct ");
			new = join(new, className);
			new = join(new, " *");
		}
	}

	new = join(new, "); \t /* insert function pointer here */\n\t");

	return new;
}

/**
 * genOutName
 * Creates the name of the file to print to from the input file.
 *
 * Takes a "path/path/filename.cc" in and returns "filename.c" out
 * IN:	fileName, the name of the file it is reading from
 * OUT: returns a new string with the filename on success
 *		NULL on failure
 * POST: creates a new string in memory
 * ERROR: NULL if mem can not be allocates
 *		  or if input is invalid
 */
char* genOutName(char* fileName)
{
	char* new;
	char* startName;
	int i;
	int length;

	/* check the parameter */
	if (fileName == NULL)
	{
		return NULL;
	}

	startName = fileName;
	length = strlen(fileName) - 1;

	/* loop throuch untill it finds the end of the directory name */
	for (i = 0; i < length; i++)
	{
		if (fileName[i] == '/')
		{
			startName = &fileName[i + 1];
		}
	}

	/* duplicate the name section of the fileName */
	new = strduplicate(startName);

	/* make sure that memory allocation worked */
	if (new == NULL)
	{
		return NULL;
	}

	/*  change the file ending from .cc to .c */
	new[strlen(new) - 1] = '\0';

	return new;
}


bool startEndQuote(char* str)
{
	/* make sure param is valid */
	if (str == NULL)
	{
		return FALSE;
	}

	/* check if first and last char are quoted */
	if (str[0] == '\"' && str[strlen(str) - 1] == '\"')
	{
		return TRUE;
	}

	return FALSE;
}

char* wrapQuote(char* str)
{
	char* strout;

	if (str == NULL)
	{
		strout = strduplicate("\"\"");
	}
	else
	{
		/* make sure it is wraped in quotes */
		if (startEndQuote(str) == TRUE)
		{
			strout = strduplicate(str);
		}
		else
		{
			strout = strduplicate("\"");
			strout = join(strout, str);
			strout = join(strout, "\"");
		}
	}

	return strout;
}

char* unWrapQuote(char* str)
{
	char* strout;

	if (str == NULL)
	{
		strout = strduplicate(" ");
	}
	else
	{
		/* make sure it is not wraped in quotes */
		if (startEndQuote(str) == TRUE)
		{
			strout = substring(str, 1, strlen(str) - 2);
		}
		else
		{
			strout = strduplicate(str);
		}
	}

	return strout;
}

