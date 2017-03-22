/****************************** C++lite.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: January 29, 2017                    	 Course Name: CIS*2750
Assignment: A1

holds the main control for converting a C++ into C

See README for usage and restrictions for this program

****************************************************************************/


#include "tokenStruct.h"
#include "mystring.h"
#include "list.h"
#include "lists.h"
#include "typeCheck.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * searchGlibal
 * Searches for global variables
 *
 * IN:	tok, where to start the search
 * OUT: a list of the global varibales on success
 *		NULL on failure
 * POST: joins the types together into 1 token
 * ERROR: it token does nto exist then return NULL
 */
List* searchGlobal(Keyword* tok);

/**
 * findStartofClass
 * Recursivly searches to the start of a class, signified by "class [name] {"
 *
 * IN:	head, where to start searching
 *		masterList, the list of lists (contains globals, functions, calss variables)
 *		root, the root of list of elements
 * OUT: the last keyword of the class
 *		NULL on failure
 * POST: calls other functions that do all of the class manipulation
 * ERROR: it token does nto exist then return NULL
 */
Keyword* findStartofClass(Keyword* head, Lists*  masterList, Keyword* root);

/**
 * createConstruct
 * Creates the constructor for the class adds a comment identifying it was auto generated
 * if there are no functions for the class then it creates a function that does nothing
 *
 * IN:	funcList, the list of functions that are in the class
 *		className, the name of the class
 * OUT: pointer to a string in mem that contains the constructor
 *		NULL on failure
 * POST: generateds the contructor for the class
 * ERROR: NULL if invalid
 */
char* createConstruct(List* funcList, char* className);

/**
 * searchClass
 * Searches the class for class variables, and functions
 *
 * IN:	startClass, token that contains the token after the class name
 *		className, the name of the class
 *		masterList, contains the list of lists (globals, class func, class var)
 *		root, the first token in the file
 * OUT: none
 * POST: searches the usage in the class
 * ERROR: returns if something is wrong
 */
void searchClass(Keyword* startClass, string className,  Lists*  masterList, Keyword* root);

/**
 * modFunc
 * Modifies the function in the class so that it has the correct name and
 * checks wether or not it needs to have a object passed in as an aditional
 * parameter and adds a function pointer to its stuct
 *
 * IN:	startFunc, the start of the function
 *		className, the namer of the class it was in
 *		putName, where to put the fucntion poitner in the class
 *		masterList, the list  that contains all the calss variables and other stuff
 * OUT: NULL on failure
 *		the end of the function on success
 * POST: modifies the function so it is valid C
 * ERROR: NULL if something goes worng
 */
Keyword* modFunc(Keyword* startfunc, string className, Keyword** putName, Lists* masterList);

/**
 * searchUsage
 * Recursivly search for the usage of the class in the rest of the program
 * on first call:
 *
 * tok 			   = right after the class is first defined
 * funcList 	  = the list of all the functions on the class
 * className 	  = the name of the class
 * parent_objects = NULL
 *
 * run untill tok == NULL (EOF)
 *
 * IN:	tok, the start of the class definition
 *		funcList, the list of all the functions in the class
 *		className, the name of the class
 *		parent_objects,	??????
 *		parent_var		??????
 *		isGlobal 		??????
 * OUT: the first token after the class on success
 *		NULL on failure
 * POST: modifies the function so it is valid C
 * ERROR: NULL if something goes worng
 */
Keyword* searchUsage(Keyword* tok, List* funcList, char* className, List* parent_objects, List* parent_var, bool isGlobal);

/**
 * findMain
 * Searches for the start main and returns the opening { of main
 *
 * IN: tok, where to start the search
 * OUT: a poitner to the { of main
 * POST: none
 * ERROR: NULL if it runns off the end of the list
 */
Keyword* findMain(Keyword* tok);

/**
 * loadFile
 * Loads the file into tokens for further parsing
 *
 * IN:	fileName, the name of the file that it is reading from
 * OUT: NULL on failure,
 *		the root of the list of tokens of the file
 * POST: generates a lsit of tokens from the file
 * ERROR: NULL if somehting goes wrong
 */
Keyword* loadFile(char* fileName);



/**
 * Main entry point to the program.
 * Takes in 1 argument of the C++ lite file name
 *
 */
int main(int argc, char const *argv[]) {

	char* fileName;
	char* outFileName;

	Keyword* root;
	Keyword* test;
	Lists* masterList;
	FILE* fileOut;

	if (argc != 2) {
		printf("usage: ./a1 fileName\n");
		return -1;
	}

	fileName = strduplicate(argv[1]);

	outFileName = genOutName(fileName);

	fileOut = fopen(outFileName, "w");

	/* create the master list so all functions can use it */
	masterList = newLists();

	root = loadFile(fileName);


	masterList->globalList = searchGlobal(root);


	do {
		test = findStartofClass(root, masterList, root);
	} while (test != NULL);

	printTokensAll(fileOut, root);

	fclose(fileOut);
	free(fileName);
	free(outFileName);
	freeToken(root);
	freeLists(masterList);

	return 0;
}


/**
 * searchGlibal
 * Searches for global variables
 *
 * IN:	tok, where to start the search
 * OUT: a list of the global varibales on success
 *		NULL on failure
 * POST: joins the types together into 1 token
 * ERROR: it token does nto exist then return NULL
 */
List* searchGlobal(Keyword* tok) {
	List* globals;
	Keyword* temp;

	/* check the parameter list */
	if (tok == NULL) {
		return NULL;
	}

	globals = NULL;

	/* loop through untill reached the last token */
	while (tok != NULL) {
		char* type;

		type = isType(tok);

		/* if the token is a type then it is ether: global varm, func, func ptoto type */
		if (type != NULL) {
			if (strcmp(tok->next->token, "{") == 0) {	/* then it is a struct*/
				free(type);
				break;
			} else if (strcmp(tok->next->next->token, "(") == 0) {
				/* then it is ether a function proto type or a function dec,*/
				/* if it is a declaration then stop,
					if it is a prototyoe do nothing and continue */

				temp = tok;
				/* skip all of the parameters in the (...)*/
				while (temp != NULL && strcmp(temp->token, ")") != 0) {
					temp = temp->next;
				}

				/* check if it is a function proto type */
				if (temp != NULL && temp->next != NULL && strcmp(temp->next->token, ";") == 0) {
					tok = temp;
					continue;
				} else {
					/* it is the start of a normal function and break;*/
					free(type);
					break;
				}
			} else if (tok != NULL && tok->next != NULL && tok->next->next != NULL &&
			           (strcmp(tok->next->next->token, ",") == 0 ||
			            strcmp(tok->next->next->token, ";") == 0 ||
			            strcmp(tok->next->next->token, "=") == 0 )) {
				/* it is a global variable declaration */
				char* varType;
				char* name;

				varType = strduplicate(tok->token);
				name = strduplicate(tok->next->token);

				globals = addToList(globals, name, varType);

				free(name);

				/* loop token until reached the end of declaration line */
				while (tok != NULL && strcmp(tok->token, ";") != 0) {
					/* if it is a list then the next token is a var name */
					if (strcmp(tok->token, ",") == 0) {
						name = strduplicate(tok->next->token);

						globals = addToList(globals, name, varType);
						free(name);
					}
					tok = tok->next;
				}

				free(varType);
				/* avoid a segfault*/
				if (tok == NULL) {
					free(type);
					continue;
				}
			}
		}
		free(type);
		tok = tok->next;
	}

	return globals;
}

/**
 * findStartofClass
 * Recursivly searches to the start of a class, signified by "class [name] {"
 *
 * IN:	head, where to start searching
 *		masterList, the list of lists (contains globals, functions, calss variables)
 *		root, the root of list of elements
 * OUT: the last keyword of the class
 *		NULL on failure
 * POST: calls other functions that do all of the class manipulation
 * ERROR: it token does nto exist then return NULL
 */
Keyword* findStartofClass(Keyword* head, Lists*  masterList, Keyword* root) {
	string className;

	/* make sure that perameters are valid */
	if (head == NULL) {
		return NULL;
	}

	/* find the start of the class */
	if (strcmp(head->token, "class") != 0) {
		return findStartofClass(head->next, masterList, root);
	}

	/* check that the third variable is a open backet */
	if (head->next->next->token == NULL) {
		return NULL;
	}

	if (strcmp(head->next->next->token, "{") != 0) {
		/* continue looking for the start of a class*/
		return findStartofClass(head->next, masterList, root);
	}

	/* it is a class definition */
	/* make this a copy instead */
	className = strduplicate(head->next->token);

	/* replace the "class" token with "struct" */
	changeToken(head, "struct ");

	/* deal with stuff in class*/
	searchClass(head->next->next, className, masterList, root);

	free(className);
	return head;
}

/**
 * createConstruct
 * Creates the constructor for the class adds a comment identifying it was auto generated
 * if there are no functions for the class then it creates a function that does nothing
 *
 * IN:	funcList, the list of functions that are in the class
 *		className, the name of the class
 * OUT: pointer to a string in mem that contains the constructor
 *		NULL on failure
 * POST: generateds the contructor for the class
 * ERROR: NULL if invalid
 */
char* createConstruct(List* funcList, char* className) {
	char* constructor;
	List* func;

	/* check the parameters */
	if ( className == NULL) {
		return NULL;
	}

	func = funcList;

	/* void constructor[className] (struct [className] * myOBJ) { \n */
	constructor = strduplicate("\n/* Generated constructor for the class */\nvoid constructor");
	constructor = join(constructor, className);
	constructor = join(constructor, " (struct ");
	constructor = join(constructor, className);
	constructor = join(constructor, " * myOBJ) {\n");

	/* add the function pointers if there are functions */
	if (funcList != NULL) {
		/* go through each function in the list */
		while (func != NULL) {
			constructor = join(constructor, "\tmyOBJ->");
			constructor = join(constructor, func->newName);
			constructor = join(constructor, " = &");
			constructor = join(constructor, func->newName);
			constructor = join(constructor, ";\n");

			func = func->next;
		}
	} else {
		constructor = join(constructor, "\treturn;\t\t\t/* constructor that does nothing */\n");
	}

	constructor = join(constructor, "}\n\n");

	return constructor;
}

/**
 * searchClass
 * Searches the class for class variables, and functions
 *
 * IN:	startClass, token that contains the token after the class name
 *		className, the name of the class
 *		masterList, contains the list of lists (globals, class func, class var)
 *		root, the first token in the file
 * OUT: none
 * POST: searches the usage in the class
 * ERROR: returns if something is wrong
 */
void searchClass(Keyword* startClass, string className, Lists*  masterList, Keyword* root) {
	int numBrackets;

	Keyword* tok;
	Keyword* lastVar;
	Keyword* new;
	Keyword* temp;

	char* name;
	char* next;
	char* constructor;
	string typeList;

	/* init variables */
	typeList = NULL;
	tok = startClass;
	lastVar = startClass;
	numBrackets = 0;

	/* make sure peram is valid */
	if (startClass == NULL || className == NULL) {
		return;
	}

	/* while the token exists */
	while (tok != NULL  && tok->token != NULL) {
		if (strcmp(tok->token, "{") == 0) {
			numBrackets ++;
		} else if (strcmp(tok->token, "}") == 0) {
			numBrackets --;
		}

		/*  this is the end of the class */
		if (numBrackets == 0) {
			constructor = createConstruct(masterList->funcList, className);
			changeToken(tok, constructor);

			/* remove the trailing ; after the class declaration */
			if (tok->next != NULL) {
				temp = tok->next;

				if (strcmp(temp->token, ";") == 0) {
					tok->next = temp->next;
					temp->next = NULL;
					freeToken(temp);
					temp = NULL;
				}
			}
			free(constructor);

			/* do useage search here for usage of the class in the file */
			break;
		}

		typeList = isType(tok);

		/* check if it is a datatype and concate all type tokens*/
		if (typeList != NULL) {
			if (tok->next != NULL  && tok->next->next != NULL) {
				name = tok->next->token;

				next = tok->next->next->token;

				if (next != NULL && name != NULL) {
					if (strcmp(next, "(") == 0) {
						/* parse the function, return to the first token after the function*/
						tok = modFunc(tok, className, &lastVar, masterList);
					} else {
						/* tok points to type */
						char* varType;

						varType = strduplicate(tok->token);

						/* class var list, var name , var type list*/
						masterList->classVar = addToList(masterList->classVar, name, varType);


						/* loop token until reached the end of declaration line */
						while (tok != NULL && strcmp(tok->token, ";") != 0) {
							/* if it is a list then the next token is a obj name */
							if (strcmp(tok->token, ",") == 0) {
								name = tok->next->token;

								masterList->classVar = addToList(masterList->classVar, name, varType);
							}
							tok = tok->next;
						}

						/* avoid a segfault*/
						if (tok == NULL) {
							free(typeList);
							continue;
						}
						/* tok now points to the ";" */

						lastVar = tok;		/* for replacing the end of the struct */

						free(varType);
					}
				}
			}
		}
		free(typeList);
		tok = tok->next;
	}

	/* do usage check here */
	searchUsage(root, masterList->funcList, className, NULL, masterList->globalList, TRUE);

	new = newToken();

	changeToken(new, "\n}; /* new end of class */\n\n");
	insertToken(lastVar, new);

	/* clear the list if functions and class variables from theis class */
	freeList(masterList->classVar);
	freeList(masterList->funcList);

	masterList->classVar = NULL;
	masterList->funcList = NULL;
}

/**
 * modFunc
 * Modifies the function in the class so that it has the correct name and
 * checks wether or not it needs to have a object passed in as an aditional
 * parameter and adds a function pointer to its stuct
 *
 * IN:	startFunc, the start of the function
 *		className, the namer of the class it was in
 *		putName, where to put the fucntion poitner in the class
 *		masterList, the list  that contains all the calss variables and other stuff
 * OUT: NULL on failure
 *		the end of the function on success
 * POST: modifies the function so it is valid C
 * ERROR: NULL if something goes worng
 */
Keyword* modFunc(Keyword* startfunc, string className, Keyword** putName, Lists* masterList) {
	Keyword* new;
	Keyword* temp;

	bool hasClass;				/* TRUE if it uses any variables from the class (needs obj) */
	bool hasParam;

	char* funcType;				/* free 		at end of function after func pinter added */
	char* peramType;			/* free 		malloc in loop freed in corosponding loop*/
	char* newName;				/* free 		added to token freed there*/
	char* name;					/* free 		used in loop freed in loops*/
	char* funcName;				/* free 		freed at end of func */
	char* funcTypeList;

	List* peramList;			/* free 		at the end of the func*/
	int numBrackets;

	/* add peram checks here */
	if (startfunc == NULL) {
		return NULL;
	}

	/* copy the return type of the function (used for the func ptr)*/
	funcType = strduplicate(startfunc->token);

	startfunc = startfunc->next;
	funcName = strduplicate(startfunc->token);

	newName = strduplicate(className);
	newName = join(newName, funcName);

	peramList = NULL;
	funcTypeList = NULL;

	temp = startfunc;

	hasClass = FALSE;
	hasParam = FALSE;
	numBrackets = 0;

	/* bring forward to point at start of param list */
	while (temp != NULL && strcmp(temp->token, "(") != 0) {
		temp = temp->next;
	}

	/* function peram list*/
	numBrackets = 0;
	while (temp != NULL) {
		if (strcmp(temp->token, "(") == 0) {
			numBrackets++;
		}

		if (strcmp(temp->token, ")") == 0) {
			numBrackets--;
		}

		/* check perameter list for local variables */
		peramType = isType(temp);
		if (peramType != NULL) {
			if (temp->next != NULL) {
				name = temp->next->token;

				peramList = addToList(peramList, name, temp->token);

				funcTypeList = join(funcTypeList, temp->token);
				funcTypeList = join(funcTypeList, ", ");


				hasParam = TRUE;
			}
			/* add the type to the name */
			newName = join(newName, peramType);
		}
		free(peramType); /* malloc in loop free in loop */

		temp = temp->next;

		/* it has reached the end of the param list */
		if (numBrackets == 0) {
			break;
		}
	}

	/* function body */
	numBrackets = 0;
	while (temp != NULL) {
		if (strcmp(temp->token, "{") == 0) {
			numBrackets++;
		}

		if (strcmp(temp->token, "}") == 0) {
			numBrackets--;
		}

		/* check for variable declarations, add to local var list*/
		peramType = isType(temp);
		if (peramType != NULL) {
			/* temp points to type*/
			if (temp->next != NULL) {
				char* varType;

				name = temp->next->token;

				varType = strduplicate(temp->token);
				peramList = addToList(peramList, name, varType);

				/* loop token until reached the end of declaration line */
				while (temp != NULL && strcmp(temp->token, ";") != 0) {
					/* if it is a list then the next token is a obj name */
					if (strcmp(temp->token, ",") == 0) {
						name = temp->next->token;
						peramList = addToList(peramList, name, varType);
					}
					temp = temp->next;
				}
				free(varType);
			}
		}
		free(peramType); /* malloc in loop free in loop */

		/* search for local variable first*/
		if (search(peramList,  temp->token) != NULL)
		{ }
		/* then search for class variables */
		else if (search(masterList->classVar,  temp->token) != NULL) {
			/* change the useage of the variable to struct->var */
			name = strduplicate("myOBJ->");
			name = join(name, temp->token);

			changeToken(temp, name);
			free(name);						/* malloc in loop free in loop */

			hasClass = TRUE;
		}
		/* otherwise it is a global var */

		/* it has reached the end of the block */
		if (numBrackets == 0) {
			break;
		}

		temp = temp->next;
	}

	/* add an new variable for the struct to the peram list */
	if (hasClass == TRUE) {
		new = newToken();

		name = strduplicate("struct ");
		name = join(name, className);

		/* if there are other perameters then use a comma after inserting the object */
		if (hasParam == TRUE) {
			name = join(name, "* myOBJ, ");
		} else {
			name = join(name, "* myOBJ");
		}

		changeToken(new, name);
		insertToken(startfunc->next, new);

		free(name);
		name = NULL;
		newName = joinC(newName, 'C');
		newName = joinC(newName, className[0]);
	}

	/* make the function pointer in the class body  */
	new = newToken();

	char* funcPtr;

	funcPtr = createFuncPtr(newName, funcTypeList, className, funcType, hasClass);

	changeToken(new, funcPtr);
	insertToken(*putName, new);
	*putName = new;

	/* the new function name */
	changeToken(startfunc, newName);

	/* add the function to the list with all its info */
	masterList->funcList = addToListAll(masterList->funcList, funcName, newName, funcTypeList, hasClass);

	free(funcTypeList);
	free(funcPtr);
	free(newName);
	freeList(peramList);
	free(funcType);
	free(funcName);

	return temp;
}

/**
 * searchUsage
 * Recursivly search for the usage of the class in the rest of the program
 * on first call:
 *
 * tok 			   = right after the class is first defined
 * funcList 	  = the list of all the functions on the class
 * className 	  = the name of the class
 * parent_objects = NULL
 *
 * run untill tok == NULL (EOF)
 *
 * IN:	tok, the start of the class definition
 *		funcList, the list of all the functions in the class
 *		className, the name of the class
 *		parent_objects,	??????
 *		parent_var		??????
 *		isGlobal 		??????
 * OUT: the first token after the class on success
 *		NULL on failure
 * POST: modifies the function so it is valid C
 * ERROR: NULL if something goes worng
 */
Keyword* searchUsage(Keyword* tok, List* funcList, char* className, List* parent_objects, List* parent_var, bool isGlobal) {
	List* objects;		/* free at the end of function
							pass down recursive chain (do not add to)
							check for lowest scope first (block -> parent -> ... -> file)
						*/

	List* scopeVar;		/* free at the end of function
							pass down recursive chain (do not add to)
							check for lowest scope first (block -> parent -> ... -> file)
						*/

	Keyword* func;
	Keyword* new;

	bool addParam;

	char* name;			/* do not free */
	char* class;		/* do not free */
	char* searchRes;	/* need to free in loop if used */
	char* classType;

	classType = strduplicate("struct ");
	classType = join(classType, className);
	classType = join(classType, " *, ");

	/* the token and the class can not be NULL but the list of functions can be */
	if (tok == NULL || className == NULL) {
		return NULL;
	}

	objects = parent_objects;
	scopeVar = parent_var;

	while (tok != NULL) {
		/* if it is a new object declaration*/
		if (strcmp(tok->token, "class") == 0) { /* then it is a obj*/
			char* objName;
			char* constructorCall;
			List* tempListPtr_end;
			List* tempListPtr_start;

			class = tok->next->token; /* store the name of the objects class type */

			if (strcmp(class, className) != 0) {
				/* this this object is not of the class we are looking at continue */
				tok = tok->next;
				continue;
			}

			changeToken(tok, "struct ");

			if (tok->next != NULL && tok->next->next != NULL) {
				tok = tok->next->next;	/* point to objName*/
			} else {
				break; /* error */
			}
			/* tok points to obj name*/

			tempListPtr_end = objects;

			objName = strduplicate(tok->token);

			/* add the object to the head of the list (for scope reasons) */
			objects = addToHead(objects, objName, classType);

			free(objName);

			/* loop untill reached the end of the declaration line */
			while (tok != NULL && strcmp(tok->token,  ";") != 0) {
				/* check if there are more then 1 obj being declared on the line */
				if (strcmp(tok->token, ",") == 0) {
					objName = strduplicate(tok->next->token);

					/* add the object to the head of the list (for scope reasons) */
					objects = addToHead(objects, objName, classType);

					free(objName);
				}
				tok = tok->next;
			}

			/* now need to put the constructors in after tok (points to ; )*/
			tempListPtr_start = objects;

			/* generate the constructor call  for the objects */
			while (tempListPtr_start != NULL && tempListPtr_start != tempListPtr_end) {
				constructorCall = strduplicate("constructor");
				constructorCall = join(constructorCall, className);
				constructorCall = join(constructorCall, " (&");
				constructorCall = join(constructorCall, tempListPtr_start->name);
				constructorCall = join(constructorCall, ");\t\t/* make call to the constructor for the class */\n");

				new = newToken();
				changeToken(new, constructorCall);

				/* if it is in global scope then put the constructor into main */
				if (isGlobal == TRUE) {
					Keyword* startMain;
					startMain = findMain(tok);

					insertToken(startMain, new);
				} else {
					insertToken(tok, new);
				}
				free(constructorCall);

				tempListPtr_start = tempListPtr_start->next;
			}

			/* make sure not used again */
			tempListPtr_start = NULL;
			tempListPtr_end = NULL;
		} else if ((searchRes = search(objects, tok->token)) != NULL) {		/* if its an obj*/
			name = NULL;

			/* check for [objName].[field] */
			if (tok->next != NULL && strcmp(tok->next->token, ".") == 0) {
				func = tok->next->next;
				char* objName  = tok->token;

				/* make sure it is a function */
				if (func->next != NULL && strcmp(func->next->token, "(") == 0) {
					/* search for var name (smallest scope to largest scope) to get its type*/
					char* peramTypeList = NULL;
					char* result = NULL;
					char* funcName;

					funcName = strduplicate(func->token);

					tok = func->next->next;

					/* loop through all the parameters in the list*/
					while (tok != NULL && strcmp(tok->token, ")") != 0) {
						/* get the variable type from the list*/
						result = getVarType(scopeVar, tok->token);

						/* join the type string*/
						if (result != NULL) {
							peramTypeList = join(peramTypeList, result);
							peramTypeList = join(peramTypeList, ", ");
						}
						tok = tok->next;
					}

					/* result == new name (do not free) */
					result = searchFunc(funcList, funcName, peramTypeList);

					if (result == NULL) {
						/* error no function with that set of types found */
						printf("not found error!!!\n");
						printf("\t\t%s (%s)\n", funcName, peramTypeList);
					} else {
						/* change the function name */
						changeToken(func, result);

						/* check to see if it needs to be given the object */
						addParam = getNeedsOBJ(funcList, result);

						/* insert the new &objName as first param to func*/
						if (addParam == TRUE) {
							name = strduplicate("&");
							name = join(name, objName);

							/* figure out if there is only 1 param or more*/
							if (strcmp(func->next->next->token, ")") != 0) {
								name = join(name, ", ");
							}

							new = newToken();
							changeToken(new, name);
							insertToken(func->next, new);

							free(name);
						}
					}
					free(peramTypeList);
					free(funcName);
				}
			}
		}

		/* if it is the start of a new block  then recursivly branch into block */
		else if (strcmp(tok->token, "{") == 0) {
			tok = searchUsage(tok->next, funcList, className, objects, scopeVar, FALSE);
			continue;
		}
		/* if reached the end of the block then exit this recurssive loop*/
		else if (strcmp(tok->token, "}") == 0 || strcmp(tok->token, "};") == 0) {
			tok = tok->next;
			break;
		}
		/* else check for normal varaible declarations*/
		else {
			char* paramType = isType(tok);

			/* then it is a normal variable declaration*/
			if (paramType != NULL) {
				if (strstr(tok->token, "struct") != NULL && strcmp(tok->next->token, "{") == 0) {
					tok = searchUsage(tok->next->next, funcList, className, objects, scopeVar, FALSE);
					free(paramType);
					continue;
				}
				/* then it is a var not a func */
				if (tok->next != NULL && tok->next->next != NULL && strcmp(tok->next->next->token, "(") != 0) {
					char* varType;
					char* varName;

					varType = strduplicate(tok->token);
					varName = strduplicate(tok->next->token);

					/* add the variable to the head of the list (for scope reasons) */
					scopeVar = addToHead(scopeVar, varName, varType);
					free(varName);

					/* loop untill reached the end of the declaration line */
					while (tok != NULL && strcmp(tok->token, ";") != 0) {
						/* check if there are more then 1 obj being declared on the line */
						if (strcmp(tok->token, ",") == 0) {
							varName = strduplicate(tok->next->token);

							/* add the variable to the head of the list (for scope reasons) */
							scopeVar = addToHead(scopeVar, varName, varType);

							free(varName);
						}
						tok = tok->next;
					}
					free(varType);
				}

				/* if it is a dunction definition*/
				else {
					while (tok != NULL && strcmp(tok->token, ")") != 0) {
						tok = tok->next;
					}
					free(paramType);
					continue;
				}
			}
			free(paramType);
		}
		tok = tok->next;
	}

	free(classType);

	/* free the list of variables used in this block */
	freeLimit(objects, parent_objects);
	freeLimit(scopeVar, parent_var);

	return tok;
}

/**
 * findMain
 * Searches for the start main and returns the opening { of main
 *
 * IN: tok, where to start the search
 * OUT: a poitner to the { of main
 * POST: none
 * ERROR: NULL if it runns off the end of the list
 */
Keyword* findMain(Keyword* tok) {
	/* check the parameters */
	if (tok == NULL) {
		return NULL;
	}

	/* search for the start of main */
	while (tok != NULL && strcmp(tok->token, "main") != 0) {
		tok = tok->next;
	}

	/* will happen on an error or if the search was startd after the begining of main */
	if (tok == NULL) {
		return NULL;
	}

	/* search for the start of main (.. ) { */
	while (tok != NULL && strcmp(tok->token, "{") != 0) {
		tok = tok->next;
	}

	/* tok should point to the opening { after the start of main on success ot NULL on error */
	return tok;
}

/**
 * loadFile
 * Loads the file into tokens for further parsing
 *
 * IN:	fileName, the name of the file that it is reading from
 * OUT: NULL on failure,
 *		the root of the list of tokens of the file
 * POST: generates a lsit of tokens from the file
 * ERROR: NULL if somehting goes wrong
 */
Keyword* loadFile(char* fileName) {
	/* make sure that filename exists */
	if (fileName == NULL) {
		printf("The File does not exist.\n");
		return NULL;
	}

	/* open the file */
	FILE* file;

	char temp;
	char next;

	bool space = FALSE;
	bool string = FALSE;
	bool blockComment = FALSE;
	bool lineComment = FALSE;

	Keyword* root = NULL;
	Keyword* token = NULL;
	Keyword* tmp_token = NULL;


	/* try opening the file */
	file = fopen(fileName, "r");
	if (file == NULL) {
		printf("The file could not be opened.\n");
		return NULL;
	}

	/* create a empty token to be the root */
	token = newToken();
	if (token == NULL) {
		fclose(file);
		return NULL;
	}

	root = token;

	/* read the file untill it reaches the end */
	while ((temp = fgetc(file))) {
		/* reached teh end of the file */
		if (temp == EOF) {
			break;
		}

		/* check what character it is */
		switch (temp) {
		case '\n':
		case '\r':
			if (lineComment) {
				printWhite_char(token, temp);
				lineComment = FALSE;
				break;
			}
		case ' ':
		case '\t':
		case '\f':
		case '\v':
			if (string) {
				printToken_char(token, temp);
			} else if (lineComment == FALSE && blockComment == FALSE) {
				printWhite_char(token, temp);
				space = TRUE;
			} else {
				printWhite_char(token, temp);
			}
			break;
		case '{':
		case '}':
		case '[':
		case ']':
		case '(':
		case ')':
		case ',':
		case '.':
		case '-':
		case '+':
		case '=':
		case ';':
			if (lineComment || blockComment) {
				printWhite_char(token, temp);
			} else if (string) {
				printToken_char(token, temp);
			} else {
				tmp_token = newToken();
				if (tmp_token == NULL) {
					/* error */
					freeToken(root);
					return NULL;
				} else {
					token = addToken(token, tmp_token);
					tmp_token = NULL;
					string = FALSE;
					space = TRUE;
					printToken_char(token, temp);
				}
			}
			break;
		case '\"':
			if (string) {
				printToken_char(token, temp);
				string = FALSE;
			} else if (!string && !(lineComment && blockComment)) {
				tmp_token = newToken();
				if (tmp_token == NULL) {
					/* error */
					freeToken(root);
					return NULL;
				} else {
					token = addToken(token, tmp_token);
					tmp_token = NULL;
					string = TRUE;
					printToken_char(token, temp);
				}
			} else if (lineComment || blockComment) {
				printWhite_char(token, temp);
			}
			break;
		case '\\':
			printToken_char(token, temp);
			next = fgetc(file);
			printToken_char(token, next);
			break;
		case '*':
			if (string) {
				printToken_char(token, temp);
			} else if (lineComment) {
				printWhite_char(token, temp);
			} else if (blockComment) {
				next = fgetc(file);

				if (next == '/') {
					printWhite_char(token, temp);
					printWhite_char(token, next);
					blockComment = FALSE;
				} else {
					printWhite_char(token, temp);
					ungetc(next, file);
				}
			} else {
				tmp_token = newToken();
				if (tmp_token == NULL) {
					/* error */
					freeToken(root);
					return NULL;
				} else {
					token = addToken(token, tmp_token);
					tmp_token = NULL;
					string = FALSE;
					space = TRUE;
					printToken_char(token, temp);
				}
			}
			break;
		case '/':
			if (lineComment || blockComment) {
				printWhite_char(token, temp);
			} else if (string) {
				printToken_char(token, temp);
			} else {
				next = fgetc(file);

				if (next == '*') {
					blockComment = TRUE;
					space = FALSE;
					printWhite_char(token, temp);
					printWhite_char(token, next);
				} else if (next == '/') {
					lineComment = TRUE;
					space = TRUE;
					printWhite_char(token, temp);
					printWhite_char(token, next);
				} else {
					tmp_token = newToken();
					if (tmp_token == NULL) {
						/* error */
						freeToken(root);
						return NULL;
					} else {
						token = addToken(token, tmp_token);
						tmp_token = NULL;
						space = TRUE;
						printToken_char(token, temp);
						ungetc(next, file);
					}
				}
			}
			break;
		default:
			if (lineComment || blockComment) {
				printWhite_char(token, temp);
			} else if (string) {
				printToken_char(token, temp);
			} else if (space) {
				tmp_token = newToken();

				if (tmp_token == NULL) {
					/* error */
					freeToken(root);
					return NULL;
				} else {
					token = addToken(token, tmp_token);
					tmp_token = NULL;
					space = FALSE;
					printToken_char(token, temp);
				}
			} else {
				printToken_char(token, temp);
			}
		}
	}

	fclose(file);
	return root;
}
