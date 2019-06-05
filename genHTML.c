/****************************** genHTML.c ***********************************
Student Name: Marshall Aaron Asch            Student Number:  0928357
Date: March 14, 2017                    	 Course Name: CIS*2750
Assignment: A3

Parses the config file (.wpml) into HTML with imbeded PHP

NOTE: button, text form, radio form , exe generate some PHP that needs to be
executed (only lines that begin with ?> )

The button tag requires a form to exist on the page that is called
BUTTONFORM and a javascript function link(dest) to submit the form

the text form and the radio form will have 2 extra hidden fields
for the userID and the stream name. If those PHP variables fo not exist
then the values will be left blank.

 ****************************************************************************/
#include "genHTML.h"



int main(int argc, char const *argv[]) {
	bool isValid;

	/* make sure that a filename was given */
	if (argc != 2) {
		printf("<h1>Error. A filename must be given to display.</h1>\n");
		return 1;
	}

	isValid = checkFiletype(argv[1]);

	/* make sure that the file is the correct type */
	if (isValid == false) {
		printf("<h1>Error. File is the wrong type.</h1>\n");
		return 1;
	}

	parseFile(argv[1]);

	return 0;
}

/**********************

		PARSER

 ***********************/


/**
 * checkFiletype
 * Takes in a filename and checks if its extension is .wpml
 *
 * IN:	file, the name of the file to check
 * OUT: true, if the extension is .wpml
 *		false, otherwise or on error
 * POST: none
 * ERROR: false if the filename does not exist or does not contain an extension
 */
bool checkFiletype(const char* file) {
	char* fileName;
	char* extension;

	int startExtension;

	fileName = strduplicate(file);

	/* make sure that the file is given */
	if (fileName == NULL) {
		return false;
	}

	startExtension = lastIndex(fileName, '.');
	extension = substring(fileName, startExtension + 1, strlen(fileName) - 1);

	/* check if it is a .wpml extension */
	if (extension == NULL || strcmp(extension, "wpml") != 0) {
		free(fileName);
		free(extension);
		return false;
	} else {
		free(fileName);
		free(extension);
		return true;
	}
}

/**
 * parseFile
 * Parses the .wpml file into a HTML body
 *
 * IN:	fileName, the name of the file to parse
 * OUT: none
 * POST: prints the HTML page to the stdout
 * ERROR: prints error message is there is an error
 */
void parseFile(const char * fileName) {
	FILE* file;
	char temp;
	char temp2;

	/* make sure that the file name is given */
	if (fileName == NULL) {
		return;
	}

	/* open the file */
	file = fopen(fileName, "r");
	if (file == NULL) {
		return;
	}

	while ((temp = fgetc(file))) {
		/* check if it reached the end of the file */
		if (temp == EOF) {
			break;
		}

		if (temp != '.') {
			printf("%c", temp);
		} else {
			temp2 = fgetc(file);

			/* if this is the end the print the last character and end */
			if (temp2 == EOF) {
				printf("%c", temp);
				break;
			}

			/* check what tag it is */
			switch (temp2) {
			case 'b': /* button (name="name",link="page link") */
			case 'd': /* horozontal line  there are no arguments  is it .d() or .d */
			case 'e': /* excecutable (exe=file) <file ether in current dir or ./bin  or system /bin search in that order >*/
			case 'h': /* heading (size=<1-6>,text="...") <default size = 3 (optional) , text = "HEADING"> */
			case 'i': /* form (action="action file", text="prompt", name="name", value="default value" [required] ... ) <add a submit button, use POST args apear as sets of 3 > */
			case 'l': /* link (text="...",link=<URL>) <default text = "link", link = "#" > */
			case 'p': /* picture (image="...", size=<width>x<height>) <default height = 100 width = 100 > */
			case 'r': /* radio button (action="action file", name="name", value="<value>", [value="<value>"...]) <first value is the default value> */
			case 't': /* text (filename="file) or (text="...") <default text = "Default text", if file then ready the file into the tag, text might contain other HTML tags > */
			case 's': /* select stream () takes no arguments */
			case 'n': /* line break there are no arguments  is it .n() */
			case 'f': /* create a new post form () takes no arguments */
				parseTag(file, temp2);
				break;
			default:
				printf("%c", temp);
				ungetc(temp2, file);
			}
		}
	}

	fclose(file);
}

/**
 * parseTag
 * Parses from the opning ( to the ) and parses the key, value pairs
 * that is in the order of key = value, or key = "value" must be in
 * pairs.
 *
 * IN:	file, the file that is being read
 *		type, the type of tag it is parsing
 * OUT: none
 * POST: prints the HTML tag (or PHP tags) to the stdout
 * ERROR: ends on error
 */
void parseTag(FILE* file, char type) {
	char temp;
	char temp2;

	char* text;
	char* link;
	char* image;
	char* size;
	char* action;
	char* defaultValue;
	char* name;
	char* fileName;
	char* prompt;
	char* other;
	char* onclick;
	char* class;
	char* string;
	char* keyStr;
	char* args;

	bool required;
	bool end_file;
	bool isStr;

	FieldList* fields;
	FieldList* newFieldElement;

	Param* param;
	ParamList* paramList;

	int i;



	isStr = false;
	end_file = false;

	string = NULL;
	keyStr = NULL;
	param = NULL;
	paramList = NULL;

	fields = NULL;
	other = NULL;

	/* make sure that the parameter is valid */
	if (file == NULL) {
		return;
	}

	fgetc(file);

	/* move up to first non whitespace */
	while ((temp = fgetc(file))) {
		if (temp == EOF || temp != ' ') {
			break;
		}
	}

	/* make sure it did not reach the end of the file */
	if (temp == EOF) {
		end_file = true;
	} else {
		ungetc(temp, file);

		/* read from the file */
		while ((temp = fgetc(file))) {
			/* check for premature EOF */
			if (temp == EOF) {
				end_file = true;
				break;
			}

			/* check if it reached the end of the tag*/
			if (temp == ')' && isStr == false) {
				/* add this string to the list */
				param = newParam(keyStr, string);
				paramList = addToParamList(paramList, param);
				free(keyStr);
				free(string);

				keyStr = NULL;
				string = NULL;
				break;
			}

			/* the end of a token  */
			if ((temp == ',' || temp == '=' ) && isStr == false) {

				/* set the old string to the key and the next to the value */
				if (temp == '=') {
					keyStr = string;
					string = NULL;
				}

				if (temp == ',') {
					param = newParam(keyStr, string);
					paramList = addToParamList(paramList, param);
					free(keyStr);
					free(string);

					keyStr = NULL;
					string = NULL;
				}

				continue;
			}

			/* then it has reached the end of the value */
			if (temp == '\"') {
				string = joinC(string, temp);

				TOGGLE(isStr);
				continue;
			}


			/* check to see if there is an escaped " */
			if (temp == '\\') {
				temp2 = fgetc(file);

				if (temp2 == '\"') {
					string = join(string, "\\\"");
				} else {
					string = joinC(string, temp);
					ungetc(temp2, file);
				}
				continue;
			}

			/* skip any whitespcae that is not in a string */
			if (temp == ' ' && isStr == false) {
				continue;
			}

			/* add the character to the string */
			string = joinC(string, temp);
		}
	}

	/* check if the file ended unexcpectedly */
	if (end_file == true) {
		printf("Error unexcpected end of file.\n");
		return;
	}

	/* all the args should now be in list in the order of name -> data -> name ->data ...*/

	/* parse the key, values into there corospinding arguments */
	switch (type) {
	case 'b': /* button (name="name",link="page link") */

		text = NULL;
		link = NULL;
		onclick = NULL;
		class = NULL;
		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "name") == 0) {
					text = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "link") == 0) {
					link = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "onclick") == 0) {
					onclick = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "class") == 0) {
					class = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		createButton(text, link, onclick, class, other);
		free(other);

		break;
	case 'd': /* horozontal line  there are no arguments  is it .d() */

		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		horozontalLine(other);
		free(other);
		break;
	case 'e': /* excecutable (exe=file) <file ether in current dir or ./bin  or system /bin search in that order >*/
		fileName = NULL;
		args = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (strcmp(paramList->list[i]->key, "exe") == 0) {
					fileName = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "args") == 0) {
					args = paramList->list[i]->value;
				}
			}
		}

		createExe(fileName, args);
		break;
	case 'h': /* heading (size=<1-6>,text="...") <default size = 3 (optional) , text = "HEADING"> */
		text = NULL;
		size = NULL;
		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "text") == 0) {
					text = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "size") == 0) {
					size = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		createHeading(text, size, other);
		free(other);
		break;
	case 'i': /* form (action="action file", text="prompt", name="name", value="default value" [required=true] ... ) <add a submit button, use POST args apear as sets of 3 > */

		action = NULL;
		text = NULL;
		name = NULL;
		defaultValue = NULL;
		required = false;
		newFieldElement = NULL;
		fields = NULL;
		other = NULL;


		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "action") == 0) {
					action = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "name") == 0) {
					name = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "value") == 0) {
					defaultValue = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "name") == 0) {
					name = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "required") == 0) {
					if (strcmp_nocase(paramList->list[i]->value, "true") == 0 ||
					        strcmp_nocase(paramList->list[i]->value, "\"true\"") == 0) {
						required = true;
					} else {
						required = false;
					}
				} else if (strcmp(paramList->list[i]->key, "text") == 0) {

					/* add the element */
					if (text != NULL) {
						newFieldElement = newField(name, text, defaultValue, required, other);
						fields = addToList(fields, newFieldElement);

						/* clear tuple */
						text = NULL;
						name = NULL;
						defaultValue = NULL;
						newFieldElement = NULL;

						free(other);
						other = NULL;
						required = false;
					}

					text = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		/* Add the last input field to the list */
		if (text != NULL) {
			newFieldElement = newField(name, text, defaultValue, required, other);
			free(other);
			other = NULL;
			fields = addToList(fields, newFieldElement);

			/* clear tuple */
			text = NULL;
			name = NULL;
			defaultValue = NULL;
			newFieldElement = NULL;
		}

		createFormText(action, fields);
		free(other);
		break;
	case 'l': /* link (text="...",link=<URL>) <default text = "link", link = "#" > */

		text = NULL;
		link = NULL;
		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "text") == 0) {
					text = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "link") == 0) {
					link = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		createLink(text, link, other);
		free(other);
		break;
	case 'p': /* picture (image="...", size=<width>x<height>) <default height = 100 width = 100 > */

		image = NULL;
		size = NULL;

		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "image") == 0) {
					image = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "size") == 0) {
					size = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		createPicture(image, size, other);
		free(other);
		break;
	case 'r': /* radio button (action="action file", [prompt="text"], name="name", value="<value>", [value="<value>"...]) <first value is the default value> */

		action = NULL;
		name = NULL;
		defaultValue = NULL;
		newFieldElement = NULL;
		prompt = NULL;
		fields = NULL;
		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "action") == 0) {
					action = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "name") == 0) {
					name = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "value") == 0) {
					defaultValue = paramList->list[i]->value;

					newFieldElement = newField(NULL, NULL, defaultValue, false, other);
					fields = addToList(fields, newFieldElement);

					free(other);
					other = NULL;
				} else if (strcmp(paramList->list[i]->key, "name") == 0) {
					name = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "prompt") == 0) {
					prompt = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		createFormRadio(action, name, prompt, fields);
		free(other);
		break;
	case 't': /* text (filename="file) or (text="...") <default text = "Default text", if file then ready the file into the tag, text might contain other HTML tags > */

		text = NULL;
		fileName = NULL;
		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
				if (strcmp(paramList->list[i]->key, "text") == 0) {
					text = paramList->list[i]->value;
				} else if (strcmp(paramList->list[i]->key, "filename") == 0) {
					fileName = paramList->list[i]->value;
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		createtext(text, fileName, other);
		free(other);
		break;
	case 'n': /* line break there are no arguments  is it .n() */

		other = NULL;

		/* make sure parameters are given */
		if (paramList != NULL) {
			/* get the arguments */
			for (i = 0; i < paramList->length; i++) {
				if (paramList->list[i]->key == NULL) {
					/* join other*/
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				} else {
					/* join other*/
					other = join(other, paramList->list[i]->key);
					other = joinC(other, '=');
					other = join(other, paramList->list[i]->value);
					other = joinC(other, ' ');
				}
			}
		}

		lineBreak(other);
		free(other);
		break;
	case 's': /* select stream () takes no arguments */
		createStreamSelect();
		break;
	case 'f': /* create a new post form () takes no arguments */
		createPost();
		break;
	}

	freeParamList(paramList);
	destroyFieldList(fields);
}


/**********************

	CREATE TAGS

***********************/


/**
 * createExe
 * Creates the tag to excecute a file on the server using PHP
 *
 * .e(exe="file", args="stuff")
 *
 * checks for the file in
 * 1. fileName
 * 2. bin/fileName
 * 3. /bin/fileName
 *
 * IN:	fileNameIn, the name of the file to excecute
 * 		argsIn, any optional commandline arguments
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: prints "<h4> File does not exist.</H4>"" on error
 */
void createExe(char* fileNameIn, char* argsIn) {
	FILE* file;
	char* name;
	char* fileName;
	char* args;

	/* make sure that the file to excecute exists */
	if (fileNameIn == NULL) {
		printf("<h4> File does not exist.</H4>\n");
		return;
	}

	/* make sure there are not quotes arround the fileName */
	fileName = unWrapQuote(fileNameIn);
	args = unWrapQuote(argsIn);

	/* check if in local dir */
	file = fopen(fileName, "r");
	if (file != NULL) {
		printf("<?php exec(\"%s %s\"); ?>", fileName, args);
		fclose(file);
		free(fileName);
		free(args);
		return;
	}

	name = strduplicate("./bin/");
	name = join(name, fileName);

	/* check if in local bin file */
	file = fopen(name, "r");
	if (file != NULL) {
		printf("<?php exec(\"%s %s\"); ?>", name, args);
		fclose(file);
		free(name);
		free(fileName);
		free(args);
		return;
	}

	free(name);
	name = strduplicate("/bin/");
	name = join(name, fileName);

	/* check if in system bin file */
	file = fopen(name, "r");
	if (file != NULL) {
		printf("<?php exec(\"%s %s\"); ?>", name, args);
		fclose(file);
		free(name);
		free(fileName);
		free(args);
		return;
	}

	free(args);
	free(name);
	free(fileName);

	printf("<h4> File does not exist.</H4>\n");
	return;
}

/**
 * createtext
 * Creates the tag that contains the given text
 * or the text loaded from the file
 *
 * puts all the text into a <p> </p> tags only if aditional
 * fields are given
 *
 * .t(filename="file") or .t(text="...") or .t()
 *
 * default text = "Default text"
 *
 * If the file option is given the full filepath must be given
 *
 * IN:	text, the text to put into the tag
 *		fileName, the name of the file to containing the text
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createtext(char* textIn, char* fileNameIn, char* otherIn) {
	FILE* file;
	char temp;

	char* other;
	char* text;
	char* fileName;

	/* make sure there are not quotes arround the string */
	fileName = unWrapQuote(fileNameIn);
	other = unWrapQuote(otherIn);
	text = unWrapQuote(textIn);

	/* add the additional info if it is given */
	if (otherIn != NULL) {
		printf("<div %s>\n", other);
	}


	/* if the file option is given */
	if (textIn == NULL && fileNameIn != NULL) {
		/* then read from file */
		file = fopen(fileName, "r");

		if (file == NULL) {
			printf("Failed to open the file.\n");
		} else {
			/* read the file 1 char at a time */
			while ((temp = fgetc(file))) {
				if (temp == EOF) {
					break;
				} else {
					printf("%c", temp);
				}
			}
			fclose(file);
		}
	} else if (textIn != NULL && fileNameIn == NULL) {
		/* then print the text */
		printf("%s", text);
	} else {
		/* invalid param print error */
		printf("Default text\n");
	}

	/* add the additional info if it is given */
	if (otherIn != NULL) {
		printf("</div %s>\n", other);
	}

	free(other);
	free(text);
	free(fileName);
}

/**
 * createButton
 * Creates the tag for the button elements.
 * Ff the link option is given then it will sibmit
 * a form to that page that has the ID BUTTONFORM
 * If the onclick option is given then it will do
 * that action when the button is clicked.
 *
 * .b(name="text in button", link="page link")
 * .b(name="text in button", onclick="script()")
 *
 * 2 options are required
 *
 * IN:	text, the name of the button
 *		link, the link to another page
 *		onclick, the script to be called on click
 *		class, the style classes for the button
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createButton(char* textIn, char* linkIn, char* onclickIn, char* classIn, char* otherIn) {
	char* text;
	char* link;
	char* click;
	char* class;
	char* other;

	static int numButtons = 0;

	/* make sure there are not quotes arround the string */
	text = unWrapQuote(textIn);
	link = unWrapQuote(linkIn);
	click = wrapQuote(onclickIn);
	class = wrapQuote(classIn);
	other = unWrapQuote(otherIn);


	if (textIn == NULL) {
		free(text);
		text = strduplicate("default");
	}

	/* print the correct version for link or onclick */
	if (linkIn != NULL) {
		printf("<button class=%s type=button onclick=link(\'%s\') %s>%s</button>", class, link, other, text);

		/* print the form if it is neccisary */
		if (numButtons == 0) {
			genButtonForm();
		}

		numButtons++;
	} else if (onclickIn != NULL) {
		printf("<button class=%s type=button onclick=%s %s>%s</button>", class, click, other, text);
	} else {
		printf("<button class=%s type=button %s>%s</button>", class, other, text);
	}

	free(text);
	free(link);
	free(click);
	free(class);
	free(other);
}

/**
 * createFormText
 * Creates the tag for the text form
 * submits the form using POST.
 *
 * The text, name, value and required (optional) must apear as multiples of
 * ether 3 or 4 , name value and required can apear in any order byt text
 * must be first
 * .i(action="action file", text="prompt", name="name", value="default value" [required = anything] ... )
 *
 *
 *
 * IN:	action, the file to submit the form to
 *		fields, linked list that contains a stuct with all the field info
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createFormText(char* actionIn, FieldList* fields) {
	FieldList* tempField;

	char* action;

	/* make sure there are not quotes arround the string */
	action = wrapQuote(actionIn);

	printf("<form action=%s method=\"post\">\n", action);

	/* to hold the userID and the stream that gets submitted */
	printf("<input type=\"hidden\" name=\"__USERID_TF\" id=\"__USERID_TF\" value=\"<?php echo $userID; ?>\">\n");
	printf("<input type=\"hidden\" name=\"__STREAM_TF\" id=\"__STREAM_TF\" value=\"<?php echo $streamName; ?>\">\n");

	printf("<p><span class=\"required\">*</span> = required field </p>\n");

	/* go through all the fields*/
	tempField = fields;
	while (tempField != NULL) {
		if (tempField->required == true) {
			printf("<label for=%s>%s <span class=\"required\">*</span></label>", tempField->name, tempField->prompt);
			printf("<input type=\"text\" name=%s id=%s value=%s required>\n", tempField->name, tempField->name, tempField->defaultValue);
		} else {
			printf("<label for=%s>%s</label>", tempField->name, tempField->prompt);
			printf("<input type=\"text\" name=%s id=%s value=%s>\n", tempField->name, tempField->name, tempField->defaultValue);
		}

		printf("<br>\n");
		tempField = tempField->next;
	}

	printf("<input type=\"submit\" value=\"Submit\"/>\n");
	printf("</form>\n");

	free(action);
}

/**
 * createFormRadio
 * Creates the tag for the radio button form
 * submits the form using POST.
 *
 * prompt is optional, if it is not given the the name is used.
 * The first value in the list starts off selected
 *
 * .r(action="action file", [prompt="text"], name="name", value="<value>", [value="<value>"...])
 *
 * IN:	action, the file to submit the form to
 *		name, the name of the field
 *		prompt, the prompt for the input
 *		fields, linked list that contains a stuct with all the field info
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createFormRadio(char* actionIn, char* nameIn, char* promptIn, FieldList* fields) {
	FieldList* tempField;

	bool first;

	char* action;
	char* name;
	char* prompt;

	/* make sure there are not quotes arround the string */
	action = wrapQuote(actionIn);
	name = wrapQuote(nameIn);
	prompt = unWrapQuote(promptIn);


	first = true;

	if (nameIn == NULL) {
		free(name);
		name = strduplicate("\"radioButton\"");
	}

	if (promptIn == NULL) {
		free(prompt);
		prompt = unWrapQuote(name);
	}

	printf("<form action=%s method=\"post\">\n", action);

	/* insert the userID and the streamName */
	printf("<input type=\"hidden\" name=\"__USERID_RF\" id=\"__USERID_RF\" value=\"<?php echo $userID; ?>\">\n");
	printf("<input type=\"hidden\" name=\"__STREAM_RF\" id=\"__STREAM_RF\" value=\"<?php echo $streamName; ?>\">\n");

	printf("<label for=%s>%s</label>\n", name, prompt);

	/* go through all the fields*/
	tempField = fields;
	while (tempField != NULL) {
		if (first == true) {
			printf("<input type=\"radio\" name=%s id=%s value=%s checked>%s<br>\n", name, name, tempField->defaultValue, tempField->defaultValue);

		} else {
			printf("<input type=\"radio\" name=%s id=%s value=%s>%s<br>\n", name, name, tempField->defaultValue, tempField->defaultValue);
		}
		tempField = tempField->next;
	}

	printf("<input type=\"submit\" value=\"Submit\"/>\n");
	printf("</form>\n");

	free(action);
	free(name);
	free(prompt);
}

/**
 * createLink
 * Creates the tag for the link
 *
 * .l(text="...",link="<URL>")
 *
 * If the text is not given then it is set to "link"
 * If the link is not given it is set to "#" (its self)
 *
 * IN:	text, the text that is part of the link
 *		link, the URL to go to
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createLink(char* textIn, char* linkIn, char* otherIn) {
	char* other;
	char* text;
	char* link;

	/* make sure there are not quotes arround the string */
	text = unWrapQuote(textIn);
	link = unWrapQuote(linkIn);
	other = unWrapQuote(otherIn);

	if (textIn == NULL) {
		free(text);
		text = strduplicate("link");
	}

	if (linkIn == NULL) {
		free(link);
		link = strduplicate("\"#\"");
	}

	/* print the link tag */
	printf("<a href=%s %s>%s</a>\n", link, other, text);

	free(other);
	free(text);
	free(link);
}

/**
 * createPicture
 * Creates the tag for a picture
 *
 * .p(image="URL", size=<width>x<height>)
 *
 * If the image is not given then a placeholder is used
 * if size is not given then height and width are set to 100
 *
 * IN:	image, the URL of the picture that is being inserted
 *		size, the height and width of the picture
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createPicture(char* imageIn, char* sizeIn, char* otherIn) {
	char* height;
	char* width;

	char* other;
	char* image;
	char* size;

	/* make sure there are not quotes arround the string */
	image = unWrapQuote(imageIn);
	size = unWrapQuote(sizeIn);
	other = unWrapQuote(otherIn);

	if (sizeIn != NULL) {
		/* try to get the size of the picture */
		width = substring(size, 0, firstIndex(size, 'x') - 1);
		height = substring(size, firstIndex(size, 'x') + 1, strlen(size) - 1);
	} else {
		width = NULL;
		height = NULL;
	}

	/* check if the defaults need to be used */
	if (image == NULL) {
		free(image);
		image = strduplicate("\"http://placehold.it/500/?text=image+not+found\"");
	}

	if (width == NULL) {
		width = strduplicate("\"100\"");
	}

	if (height == NULL) {
		height = strduplicate("\"100\"");
	}

	/* print the image tag */
	printf("<img src=%s height=%s width=%s alt=\"Image not found\" %s>\n", image, height, width, other);

	free(height);
	free(width);
	free(size);
	free(image);
	free(other);
}

/**
 * createHeading
 * Creates the tag for a heading
 *
 * .h(size=<1-6>,text="...")
 *
 * If the text is not given then "HEADING" is used
 * if size is not given then size is set to 3
 *
 * IN:	text, body of the <H> tag
 *		size, the size of the heading
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createHeading(char* textIn, char* sizeIn, char* otherIn) {
	char* other;
	char* text;
	char* size;

	/* make sure there are not quotes arround the string */
	text = unWrapQuote(textIn);
	size = unWrapQuote(sizeIn);
	other = unWrapQuote(otherIn);


	if (sizeIn == NULL) {
		free(size);
		size = strduplicate("3");
	}

	if (textIn == NULL) {
		free(text);
		text = strduplicate("HEADING");
	}

	/* make sure that size is valid if not set to default  */
	if (size == NULL || strlen(size) > 1 || size[0] < '1' || size[0] > '6') {
		size = "3";
	}

	/* print the heading tag */
	printf("<H%s %s>%s</H%s>\n", size, other, text, size);

	free(text);
	free(other);
	free(size);
}

/**
 * horozontalLine
 * Creates the tag for a hotozontal line
 *
 * .d()
 *
 * IN:	other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void horozontalLine(char* otherIn) {
	char* other;

	/* make sure there are not quotes arround the string */
	other = unWrapQuote(otherIn);

	printf("<HR %s>\n", other);

	free(other);
}

/**
 * horozontalLine
 * Creates the tag for a line break
 *
 * .n()
 *
 * IN:	other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void lineBreak(char* otherIn) {
	char* other;

	/* make sure there are not quotes arround the string */
	other = unWrapQuote(otherIn);

	printf("<BR %s>\n", other);

	free(other);
}

/**
 * genButtonForm
 * Prints the form needed for buttons to to submit to abother page
 *
 * IN:	none
 * OUT: none
 * POST: prints the form to the stdout
 * ERROR: none
 */
void genButtonForm() {
	printf("<!-- Form that holds the userID and the stream for the links -->\n");
	printf("<form action=\"#\" method=\"post\" id=\"BUTTONFORM\">\n");
	printf("<input type=\"hidden\" name=\"__USERID_B\" id=\"__USERID_B\" value=\"<?php echo $userID; ?>\">\n");
	printf("<input type=\"hidden\" name=\"__STREAM_B\" id=\"__STREAM_B\" value=\"<?php echo $streamName; ?>\">\n");
	printf("</form>\n");


	/* generate the script needed */
	printf("<script>\n");
	printf("function link(dest) {\n");
	printf("var form = document.getElementById(\"BUTTONFORM\");\n");
	printf("form.action = dest;\n");
	printf("form.submit();\n");
	printf("}\n");
	printf("</script>\n");
}

/**
 * createStreamSelect
 * Generated the PHP code that will generate
 * the form to select a stream
 *
 * IN:	none
 * OUT: none
 * POST: prints the form to the stdout
 * ERROR: none
 */
void createStreamSelect() {
	printf("<?php if (sizeof($streamList) === 0)");
	printf("{");
	printf("echo \"<h3>Invalid Login</H3>\";");
	printf("}");
	printf("else");
	printf("{");
	printf("echo \"<form action=\\\"viewposts.php\\\" method=\\\"post\\\">\";");
	printf("echo \"<input type=\\\"hidden\\\" name=\\\"__USERID_B\\\" id=\\\"__USERID_B\\\" value=\\\"$userID\\\">\";");

	printf("echo \"<input type=\\\"radio\\\" name=\\\"__STREAM_B\\\" id=\\\"__STREAM_B\\\" value=\\\"all\\\" checked>all <br>\";");
	printf("foreach ($streamList as $stream)");
	printf("{");
	printf("echo \"<input type=\\\"radio\\\" name=\\\"__STREAM_B\\\" id=\\\"__STREAM_B\\\" value=\\\"$stream\\\">$stream <br>\";");
	printf("}");

	printf("echo \"<input type=\\\"submit\\\" value=\\\"Select\\\"/>\";");
	printf("echo \"</form>\";");
	printf("} ?>");
}

/**
 * createPost
 * Generated the PHP code that will generate
 * the form to select submit a post
 *
 * IN:	none
 * OUT: none
 * POST: prints the form to the stdout
 * ERROR: none
 */
void createPost() {
	/* print error mssage if they dont have permission */
	printf("<?php if (sizeof($streamList) === 0)");
	printf("{");
	printf("echo \"<h3>Invalid Login</H3>\";");
	printf("}");
	printf("else");
	printf("{");
	printf("echo \"<form action=\\\"post.php\\\" method=\\\"post\\\">\";");
	printf("echo \"<input type=\\\"hidden\\\" name=\\\"userID\\\" id=\\\"userID\\\" value=\\\"$userID\\\">\";");
	printf("echo \"<input type=\\\"hidden\\\" name=\\\"oldstream\\\" id=\\\"oldstream\\\" value=\\\"$streamName\\\">\";");


	printf("echo \"<p><span class=\\\"required\\\">*</span> = required field </p>\";");

	printf("echo \"<label for=\\\"stream\\\">Stream: <span class=\\\"required\\\">*</span></label>\";");
	printf("echo \"<select name=\\\"stream\\\" id=\\\"stream\\\" required>\";");

	/* generate the stream options */
	printf("foreach ($streamList as $stream)");
	printf("{");
	printf("echo \"<option value=\\\"$stream\\\">$stream</option>\";");
	printf("}");

	printf("echo \"</select><br>\";");

	printf("echo \"<label for=\\\"post\\\">Message: <span class=\\\"required\\\">*</span></label><br>\";");
	printf("echo \"<textarea name=\\\"post\\\" id=\\\"post\\\" rows=\\\"8\\\" col=\\\"80\\\" required></textarea>\";");

	printf("echo \"<br><input type=\\\"submit\\\" value=\\\"Submit\\\"/>\";");
	printf("echo \"</form>\";");
	printf("} ?>");
}


/**
 * newParamList
 * Create a new ParamList struct
 * with the a list of the given length
 *
 * IN:	num, the number of elements in the list
 * OUT: pointer to the struct in mem on success
 *		NULL on failure
 * POST: create memeoy
 * ERROR: NULL if mem could not be allocated
 */
ParamList* newParamList(int num) {
	ParamList* list;

	/* try alocating memory */
	list = malloc(sizeof(ParamList));
	if (list == NULL) {
		return NULL;
	}

	/* initilize the data */
	list->length = 0;
	list->list = malloc(sizeof(Param*)*num + 1);

	if (list->list != NULL) {
		list->length = num;
		list->list[0] = NULL;
	}

	return list;
}

/**
 * addToParamList
 * Adds the item to the list. Makes the list bigger
 *
 * IN:	list, the list to be added to
 *		item, the item to be added
 * OUT: pointer to the list object
 *		NULL on failure
 * POST: makes the list bugger
 * ERROR: NULL if mem could not be allocated
 */
ParamList* addToParamList(ParamList* list, Param* item) {
	Param** temp;
	int i;

	/* check that a item is being added */
	if (item == NULL) {
		return list;
	}

	if (list == NULL) {
		/* create the list*/
		list = newParamList(1);
		if (list == NULL) {
			return NULL;
		}
		list->list[0] = item;
		return list;
	}

	/* allocate new memory */
	temp = malloc(sizeof(Param*) * (list->length + 2));
	if (temp == NULL) {
		return list;
	}

	/* copy list over */
	for (i = 0; i < list->length; i++) {
		temp[i] = list->list[i];
	}

	/* add the new one */
	temp[list->length] = item;
	temp[list->length + 1] = NULL;

	list->length += 1;
	free(list->list);
	list->list = temp;

	return list;
}

/**
 * newParam
 * Create a new Param struct
 * with the given data
 * the stings are copied and need to be
 * freed by the caller
 *
 * IN:	key, the key of the value
 *		value, the value of the item
 * OUT: pointer to the struct in mem on success
 *		NULL on failure
 * POST: copys the strings into memory
 * ERROR: NULL if mem could not be allocated
 */
Param* newParam(char* key, char* value) {
	Param* item;

	/* allocate memory  */
	item = malloc(sizeof(Param));
	if (item == NULL) {
		return NULL;
	}

	item->key = strduplicate(key);
	item->value = strduplicate(value);

	return item;
}


/**
 * freeParam
 * Destroys all the memory being used by the
 * struct
 *
 * IN:	param, the list to be freed
 * OUT: none
 * POST: destroys all the memory
 * ERROR: ends
 */
void freeParam(Param* param) {
	/* make sure argument is valid */
	if (param == NULL) {
		return;
	}

	free(param->key);
	free(param->value);
	free(param);
}

/**
 * freeParamList
 * Destroys all the memory being used by the list
 *
 * IN:	list, the list to be freed
 * OUT: none
 * POST: destroys all the memory
 * ERROR: ends
 */
void freeParamList(ParamList* list) {
	int i;

	/* check the parameters */
	if (list == NULL) {
		return;
	}

	/* free the list */
	for (i = 0; i < list->length; i++) {
		freeParam(list->list[i]);
	}

	free(list->list);
	free(list);
}

