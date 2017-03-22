/****************************** token.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

Contains all the functions to modify, create and display the tokens.

The tokens hold 2 strings and point to the next string in the list.
The token also keeps track of how long its strings are.
****************************************************************************/

#include "tokenStruct.h"
#include "mystring.h"


/***********************************************
*
*
*			CREATE DESTOY INSERT
*
*
*************************************************/


/**
 * newToken
 * Create and allocate memory for a new Keyword.
 * allocates an initial ammount of memory for both
 * the whitespace and the token strings, that is
 * the size of the macro START_LEN (10 characters)
 *
 * IN:	none
 * OUT: pointer to the location in memory of the Keyword struct
 *		NULL, on failure
 * POST: allocates memory for the keyword and for its 2 strings
 * ERROR: NULL if memory could not be allocated
 */
Keyword* newToken() {
	Keyword* keyword;

	keyword = malloc(sizeof(Keyword));

	/* check that memory could be allocated */
	if (keyword == NULL) {
		return NULL;
	}

	/* allocate mem for the strings*/
	keyword->token = malloc(sizeof(char) * START_LEN);
	keyword->whitespace = malloc(sizeof(char) * START_LEN);
	keyword->next = NULL;

	keyword->token_size = START_LEN;
	keyword->white_size = START_LEN;

	/* make sure that init worked */
	if (keyword->token == NULL || keyword->whitespace == NULL) {
		freeToken(keyword);
		return NULL;
	}

	/* init first characters to NULL */
	keyword->token[0] = '\0';
	keyword->whitespace[0] = '\0';

	return keyword;
}

/**
 * freeToken
 * Recursivly free all the memory associated with the Keyword.
 * the 2 strings and the nect token then itself.
 *
 * IN: token, the head of the list of tokens to be freed
 * OUT: none
 * POST: all the memory that is used by this list is freed
 * ERROR: none
 */
void freeToken(Keyword* token) {
	if (token == NULL) {
		return;
	}

	/* free the mem associated with this token*/
	free(token->token);
	free(token->whitespace);

	freeToken(token->next);

	free(token);
}

/**
 * addToken
 * Adds a token that was created after the first token
 * used when adding a new token to the list when it is getting created
 *
 * IN:	token_old, the token that is before the one that is being added
 *		token_new, the new token that is being added
 * OUT: returns a pointer to the keyword
 * POST: inserts the new token after the old token.
 * ERROR: none
 */
Keyword* addToken(Keyword* token_old, Keyword* token_new) {
	/* make sure that the perameters are valid */
	if (token_old == NULL) {
		return token_new;
	}

	if (token_new == NULL) {
		return token_old;
	}


	token_old->next = token_new;

	return token_new;
}

/**
 * insertToken
 * Adds a token that was created after the token
 * used when a token is eing inserted into the middle of the list
 *
 * IN:	token_old, the token that is before the one that is being added
 *		token_new, the new token that is being added
 * OUT: returns a pointer to the keyword
 * POST: inserts the new token after the old token.
 * ERROR: none
 */
int insertToken(Keyword* first, Keyword* new) {
	/* check the parameters */
	if (first == NULL || new == NULL) {
		return -1;
	}

	/* the one that is being inserted must not have any tokens after it */
	if (new->next != NULL) {
		return -2;
	}

	new->next = first->next;
	first->next = new;

	return 0;
}


/***********************************************
*
*
*			WRITE TO THE TOKEN
*
*
*************************************************/


/**
 * printToken_str
 * Prints a string of characters to the Keywords
 * token string.
 * msg needs to be freed by the caller
 *
 * IN:	keyword, the struct that it is being added to
 *		msg, the string that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the new string is added to the end of the old string
 * ERROR: -1, or -2 if an error occurs
 */
int printToken_str(Keyword* keyword, char* msg) {
	/* make sure that the arguments are not null */
	if (keyword == NULL || msg == NULL) {
		return -1;
	}

	/* make sure that the mem for the string is currently allocated */
	if (keyword->token == NULL) {
		keyword->token = malloc(sizeof(char) * (strlen(msg) + 2));
		keyword->token_size = strlen(msg) + 2;

		if (keyword->token == NULL) {
			keyword->token_size = 0;
			return -2;
		} else {
			sprintf(keyword->token, "%s%s", keyword->token, msg);
			return 0;
		}
	} else {
		/* check to see if  there is enogh space */
		if (strlen(keyword->token) + strlen(msg) < keyword->token_size) {
			sprintf(keyword->token, "%s%s", keyword->token, msg);
			return 0;
		} else {
			/* make the variable bigger */
			keyword->token = realloc(keyword->token, sizeof(char) * (keyword->token_size +
			                         strlen(msg) + START_LEN));
			keyword->token_size += strlen(msg) + START_LEN;

			if (keyword->token == NULL) {
				keyword->token_size = 0;
				return -2;
			} else {
				/* copy new string into it */
				sprintf(keyword->token, "%s%s", keyword->token, msg);
				return 0;
			}
		}
	}

	return -1;
}

/**
 * printWhite_str
 * Prints a string of characters to the Keywords
 * whitespace string.
 * msg needs to be freed by the caller
 *
 * IN:	keyword, the struct that it is being added to
 *		msg, the string that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the new string is added to the end of the old string
 * ERROR: -1, or -2 if an error occurs
 */
int printWhite_str(Keyword* keyword, char* msg) {
	/* make sure that the arguments are not null */
	if (keyword == NULL || msg == NULL) {
		return -1;
	}

	/* make sure that the mem for the string is currently allocated */
	if (keyword->whitespace == NULL) {
		keyword->whitespace = malloc(sizeof(char) * (strlen(msg) + 2));
		keyword->white_size = strlen(msg) + 2;

		if (keyword->whitespace == NULL) {
			keyword->white_size = 0;
			return -2;
		} else {
			sprintf(keyword->whitespace, "%s%s", keyword->whitespace, msg);
			return 0;
		}
	} else {
		/* check to see if  there is enogh space */
		if (strlen(keyword->whitespace) + strlen(msg) < keyword->white_size) {
			sprintf(keyword->whitespace, "%s%s", keyword->whitespace, msg);
			return 0;
		} else {
			/* make the variable bigger */
			keyword->whitespace = realloc(keyword->whitespace, sizeof(char) * (keyword->white_size
			                              + strlen(msg) + START_LEN));
			keyword->white_size += strlen(msg) + START_LEN;

			if (keyword->whitespace == NULL) {
				keyword->white_size = 0;
				return -2;
			} else {
				/* copy new string into it */
				sprintf(keyword->whitespace, "%s%s", keyword->whitespace, msg);
				return 0;
			}
		}
	}

	return -1;
}

/**
 * printToken_char
 * Prints a single character to the Keywords tokeb string
 *
 * IN:	keyword, the struct that it is being added to
 *		next, the character that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the character is added to the end of the string
 * ERROR: -1, or -2 if an error occurs
 */
int printToken_char(Keyword* keyword, char next) {
	/* make sure that the arguments are not null */
	if (keyword == NULL) {
		return -1;
	}

	/* make sure that the mem for the string is currently allocated */
	if (keyword->token == NULL) {
		keyword->token = malloc(sizeof(char) * (START_LEN));
		keyword->token_size = START_LEN;

		if (keyword->token == NULL) {
			keyword->token_size = 0;
			return -2;
		} else {
			sprintf(keyword->token, "%s%c", keyword->token, next);
			return 0;
		}
	} else {
		/* check to see if  there is enogh space */
		if (strlen(keyword->token) + 1 < keyword->token_size) {
			sprintf(keyword->token, "%s%c", keyword->token, next);
			return 0;
		} else {
			/* make the variable bigger */
			keyword->token = realloc(keyword->token, sizeof(char) * (keyword->token_size +
			                         START_LEN));
			keyword->token_size +=  START_LEN;

			if (keyword->token == NULL) {
				keyword->token_size = 0;
				return -2;
			} else {
				/* copy new string into it */
				sprintf(keyword->token, "%s%c", keyword->token, next);
				return 0;
			}
		}
	}

	return -1;
}

/**
 * printWhite_char
 * Prints a single character to the Keywords whitespace string
 *
 * IN:	keyword, the struct that it is being added to
 *		next, the character that is being added
 * OUT: -1, if the keyword does not exist
 *		-2, if memory allocation failed
 *		 0, on success
 * POST: the character is added to the end of the string
 * ERROR: -1, or -2 if an error occurs
 */
int printWhite_char(Keyword* keyword, char next) {
	/* make sure that the arguments are not null */
	if (keyword == NULL) {
		return -1;
	}

	/* make sure that the mem for the string is currently allocated */
	if (keyword->whitespace == NULL) {
		keyword->whitespace = malloc(sizeof(char) * (START_LEN));
		keyword->white_size = START_LEN;

		if (keyword->whitespace == NULL) {
			keyword->white_size = 0;
			return -2;
		} else {
			sprintf(keyword->whitespace, "%s%c", keyword->whitespace, next);
			return 0;
		}
	} else {
		/* check to see if  there is enogh space */
		if (strlen(keyword->whitespace) + 1 < keyword->white_size) {
			sprintf(keyword->whitespace, "%s%c", keyword->whitespace, next);
			return 0;
		} else {
			/* make the variable bigger */
			keyword->whitespace = realloc(keyword->whitespace, sizeof(char) *
			                              (keyword->white_size + START_LEN));
			keyword->white_size +=  START_LEN;

			if (keyword->whitespace == NULL) {
				keyword->white_size = 0;
				return -2;
			} else {
				/* copy new string into it */
				sprintf(keyword->whitespace, "%s%c", keyword->whitespace, next);
				return 0;
			}
		}
	}

	return -1;
}

/**
 * changeToken
 * Replace the keywords token string with the new string.
 * the string str needs to be freed by the caller
 *
 * IN:	keyword, the word that the token is being replaced on
 *		str, the new string that is going into the token
 * OUT: -1, if a parameter is invalid
 *		-2, if memory could not be allocated
 *		 0, on success
 * POST: the old token is freeded and replaced
 * ERROR: -1, or -2
 */
int changeToken(Keyword* keyword, char* str) {
	/* make sure that perameters are valid */
	if (keyword == NULL || str == NULL) {
		return -1;
	}

	/* free the old token */
	free(keyword->token);

	/* copy and add the new one */
	keyword->token = strduplicate(str);
	keyword->token_size = strlen(str);

	/* make sure that mem allocation worked*/
	if (keyword->token == NULL) {
		return -2;
	}

	return 0;
}

/***********************************************
*
*
*					PRINT
*
*
*************************************************/


/**
 * printTokensOnly
 * Recursivly print all the tokens onto a new line
 * used for testing
 *
 * IN:	output, the stream for where to print the output (stdout or a file)
 *		head, the head of the list of tokens to print
 * OUT: none
 * POST: tokens get printed 1 per line
 * ERROR: none
 */
void printTokensOnly(FILE* output, Keyword* head) {
	/* make sure arguments are valid */
	if (head == NULL || output == NULL) {
		return;
	}

	/* print the token */
	fprintf(output, "%s\n", head->token);

	/* print the rest of the tokens */
	printTokensOnly(output, head->next);
}

/**
 * printWhiteOnly
 * Recursivly print all the whitespace tokens onto a new line
 * used for testing
 *
 * IN:	output, the stream for where to print the output (stdout or a file)
 *		head, the head of the list of tokens to print
 * OUT: none
 * POST: tokens get printed 1 per line
 * ERROR: none
 */
void printWhiteOnly(FILE* output, Keyword* head) {
	/* check the parameters */
	if (head == NULL || output == NULL) {
		return;
	}

	/* print the whitespace string */
	fprintf(output, "%s\n", head->whitespace);
	printWhiteOnly(output, head->next);
}

/**
 * printTokensAll
 * Recursivly print all the tokens.
 * Prints the token then the whitespace.
 *
 * IN:	output, the stream for where to print the output (stdout or a file)
 *		head, the head of the list of tokens to print
 * OUT: none
 * POST: tokens get printed, seperated by the whitespace tokens
 * ERROR: none
 */
void printTokensAll(FILE* output, Keyword* head) {
	/* make sure that the parameters are valid */
	if (head == NULL || output == NULL) {
		return;
	}

	/* print the strings */
	fprintf(output, "%s%s", head->token, head->whitespace);
	printTokensAll(output, head->next);
}

/***********************************************
*
*
*					OTHER
*
*
*************************************************/

/**
 * concat
 * Joins 2 tokens together, the tokens are seperated by a space.
 * the seconds tokens whitespace is moved to the first.
 *
 * IN:	start, the first token that is being joined.
 * OUT: TRUE, on success
 *		FALSE, on failure
 * POST: the second token is freed and there strings are joined together
 * ERROR: FALSE, if ether token does not exist
 */
bool concat(Keyword* start) {
	Keyword* next;
	string new;

	/* check that peram are valid */
	if (start == NULL || start->next == NULL) {
		return FALSE;
	}

	next = start->next;

	/* join this token with the next token */
	new = strduplicate(start->token);

	/* make sure malloc worked */
	if (new == NULL) {
		return FALSE;
	}

	/* join the tokens */
	new = joinC(new, ' ');
	new = join(new, next->token);

	/* free the old token */
	free(start->token);

	start->token = new;
	start->token_size = strlen(new);

	/* move the whitespace from next to this token */
	free(start->whitespace);
	start->whitespace = next->whitespace;
	start->white_size = next->white_size;

	/* relink this to the rest of the list*/
	start->next = next->next;

	/* free next */
	next->whitespace = NULL;
	next->next = NULL;
	free(next->token);
	free(next);

	return TRUE;
}



