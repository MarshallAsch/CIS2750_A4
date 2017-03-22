/****************************** genHTML.h ***********************************
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

#ifndef __GEN_HTML_H__
#define __GEN_HTML_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mystring.h"
#include "myboolean.h"
#include "fieldList.h"


typedef enum Type {
	key,
	value
} Type;


typedef struct Param {
	char* key;
	char* value;
} Param;

typedef struct ParamList {
	Param** list;
	int length;
} ParamList;



/**********************

		PARSER

***********************/

/**
 * checkFiletype
 * Takes in a filename and checks if its extension is .wpml
 *
 * IN:	file, the name of the file to check
 * OUT: TRUE, if the extension is .wpml
 *		FALSE, otherwise or on error
 * POST: none
 * ERROR: FALSE if the filename does not exist or does not contain an extension
 */
bool checkFiletype(const char* file);

/**
 * parseFile
 * Parses the .wpml file into a HTML body
 *
 * IN:	fileName, the name of the file to parse
 * OUT: none
 * POST: prints the HTML page to the stdout
 * ERROR: prints error message is there is an error
 */
void parseFile(const char * fileName);

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
void parseTag(FILE* file, char type);

/**********************

	FIELDLIST OBJECT

***********************/


/**********************

	CREATE TAGS

***********************/

/**
 * createExe
 * Creates the tag to excecute a file on the server using PHP
 *
 * .e(exe="file")
 *
 * checks for the file in
 * 1. fileName
 * 2. bin/fileName
 * 3. /bin/fileName
 *
 * IN:	fileNameIn, the name of the file to excecute
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: prints "<h4> File does not exist.</H4>"" on error
 */
void createExe(char* fileNameIn, char* argsIn);

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
 * IN:	textIn, the text to put into the tag
 *		fileName, the name of the file to containing the text
 *		other, any other tags that get inserted
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createtext(char* textIn, char* fileNameIn, char* otherIn);

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
 * If the file option is given the full filepath must be given
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
void createButton(char* textIn, char* linkIn, char* onclickIn, char* classIn, char* otherIn);

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
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createFormText(char* actionIn, FieldList* fields);

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
 * OUT: none
 * POST: prints the tag to the stdout
 * ERROR: none
 */
void createFormRadio(char* actionIn, char* nameIn, char* promptIn, FieldList* fields);

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
void createLink(char* textIn, char* linkIn, char* otherIn);

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
void createPicture(char* imageIn, char* sizeIn, char* otherIn);

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
void createHeading(char* textIn, char* sizeIn, char* otherIn);

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
void horozontalLine(char* otherIn);

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
void lineBreak(char* otherIn);

/**
 * genButtonForm
 * Prints the form needed for buttons to to submit to abother page
 *
 * IN:	none
 * OUT: none
 * POST: prints the form to the stdout
 * ERROR: none
 */
void genButtonForm();

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
void createStreamSelect();

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
void createPost();

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
ParamList* newParamList(int num);

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
ParamList* addToParamList(ParamList* list, Param* item);

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
Param* newParam(char* key, char* value);

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
void freeParam(Param* param);

/**
 * freeParamList
 * Destroys all the memory being used by the list
 *
 * IN:	list, the list to be freed
 * OUT: none
 * POST: destroys all the memory
 * ERROR: ends
 */
void freeParamList(ParamList* list);


#endif