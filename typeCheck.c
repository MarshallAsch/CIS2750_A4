/****************************** typecheck.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Contains the 2 functions nessisary to check if a token is a varaible type
****************************************************************************/

#include "mystring.h"
#include "typeCheck.h"

/**
 * isType
 * Checks wether or not the token is a type keyword,
 * if it is then join all the following keywords together.
 *
 * IN:	token, the first token that might be a type
 * OUT: a string that contains the first letter of each type keyword
 *		NULL, on failure
 * POST: all the tokens that are part of the type are joined together
 * ERROR: NULL if the token does not exist
 */
char* isType(Keyword* token) {
	char* type;
	static char type_list[200000][2][200];
	static int topList = 17;
	bool isVar;
	int i;

	/* check that the parameter is valid */
	if (token == NULL) {
		return NULL;
	}

	/* init the type list with base types*/
	strcpy(type_list[0][0], "int");
	strcpy(type_list[1][0], "short");
	strcpy(type_list[2][0], "long");
	strcpy(type_list[3][0], "signed");
	strcpy(type_list[4][0], "unsigned");
	strcpy(type_list[5][0], "void");
	strcpy(type_list[6][0], "long");
	strcpy(type_list[7][0], "float");
	strcpy(type_list[8][0], "char");
	strcpy(type_list[9][0], "double");
	strcpy(type_list[10][0], "extern");
	strcpy(type_list[11][0], "static");
	strcpy(type_list[12][0], "auto");
	strcpy(type_list[13][0], "register");
	strcpy(type_list[14][0], "const");
	strcpy(type_list[15][0], "volitile");
	strcpy(type_list[16][0], "typedef");		/* special */

	/* strcpy(type_list[16], "class");			special */

	strcpy(type_list[0][1], "i");
	strcpy(type_list[1][1], "s");
	strcpy(type_list[2][1], "l");
	strcpy(type_list[3][1], "s");
	strcpy(type_list[4][1], "u");
	strcpy(type_list[5][1], "v");
	strcpy(type_list[6][1], "l");
	strcpy(type_list[7][1], "f");
	strcpy(type_list[8][1], "c");
	strcpy(type_list[9][1], "d");
	strcpy(type_list[10][1], "e");
	strcpy(type_list[11][1], "s");
	strcpy(type_list[12][1], "a");
	strcpy(type_list[13][1], "r");
	strcpy(type_list[14][1], "c");
	strcpy(type_list[15][1], "v");
	strcpy(type_list[16][1], "\0");		/* special */

	type = NULL;
	isVar = FALSE;

	/* check if the current token is a type keyword */
	for (i = 0; i < topList; i++) {
		if (strcmp(token->token, type_list[i][0]) == 0) {
			isVar = TRUE;
			type = join(type, type_list[i][1]);
			break;
		}
	}

	if (isVar != TRUE) {
		/* check if it is a struct*/
		if (strcmp(token->token, "struct") == 0) {
			type = joinC(type, 's');
			type = joinC(type, (token->next->token)[0]);

			concat(token);

			while (token->next != NULL && strcmp(token->next->token, "*") == 0) {
				type = joinC(type, 'P');
				concat(token);
			}

			if (isDupList(type_list, token->token, topList) == FALSE) {
				strcpy(type_list[topList][0], token->token);
				strcpy(type_list[topList][1], type);
				topList++;
			}

			return type;
		}


		free(type);
		return NULL;
	}

	/* join all the type strings into 1 */
	while (token->next != NULL && isVar == TRUE) {
		isVar = FALSE;

		if (strcmp(token->next->token, "*") == 0) {
			isVar = TRUE;
			concat(token);
			type = joinC(type, 'P');
			continue;
		}

		for (i = 0; i < topList; i++) {
			if (strcmp(token->next->token, type_list[i][0]) == 0) {
				isVar = TRUE;

				type = join(type, type_list[i][1]);
				concat(token);
				break;
			}
		}
	}

	/* check if it is a struct with other type info */
	if (strcmp(token->token, "struct") == 0) {
		type = joinC(type, 's');
		type = joinC(type, (token->next->token)[0]);
		concat(token);

		while (token->next != NULL && strcmp(token->next->token, "*") == 0) {
			concat(token);
			type = joinC(type, 'P');
		}
		return type;
	}

	if (isDupList(type_list, token->token, topList) == FALSE) {
		strcpy(type_list[topList][0], token->token);
		strcpy(type_list[topList][1], type);
		topList++;
	}
	return type;
}

/**
 * isDupList
 * Checks wether ot not the type string is alreaddy in the list or not.
 *
 * IN: 	type_list, the list of the types
 *		type, the current string that is being checked
 *		top, the number of elements that are currently in the list
 * OUT: TRUE, if the string is in the list
 *		FALSE, if it is not in the list
 * POST: none
 * ERROR: FALSE, on error
 */
bool isDupList(char type_list[200000][2][200], char* type, int top) {
	int i;

	/* check that the paremeters are valid*/
	if (type_list == NULL || type == NULL) {
		return FALSE;
	}

	/* check the list*/
	for (i = 0; i < top; i++) {
		if (strcmp(type_list[i][0], type) == 0) {
			return TRUE;
		}
	}
	return FALSE;
}
