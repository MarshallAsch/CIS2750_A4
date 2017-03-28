****************************************************
Marshall Asch       0928357
CIS2750             Assignment 3
March 3, 2017
****************************************************


*****************
Compilation
*****************

make

or
make single

To restrict userID’s to single words

*****************
Usage
*****************

I have chosen to implement the .t() into a <div> </div> HTML tag. when there are any additional arguments that are passed in the tag. otherwise it just gets printed.


every line of output from the genHTML program has a ?> appended to the beginning and it will try to be run as PHP code. 

The .e() tags can not accept 1 additional argument names args=“” which contains any command line arguments to be passes to the file when it is run.


The user can currently only view 1 stream at a time or all of them (because it can only hold 1 stream in the buttons form) 

The stream and user names are limited by the length of the command line argument list.

Additional options in the tag list

All of the tags accept additional arguments unless otherwise stated.


.e(exe=“”, args=“”)
	- args is optional
	- if nether is given the error message is displayed
	-accepts no additional arguments

.b(name="text in button", link="page link")
.b(name="text in button", onclick="script()")

	-if text is not given then it is set to “default”
	-if a link and onclick are not given then no button action is given

.i(action="action file", text="prompt", name="name", value="default value" [required = true|false] ... )
	-the text, name , value, and required can appear in any oder as long as text is the first element in the list.
	-If required is set to true then a red * will be added next to the prompt to inform the user that it is required.


.r(action="action file", [prompt="text"], name="name", value="<value>", [value="<value>"...])

	-The prompt option was added as the text that is displayed as the question, if it is not given then the name is used as the prompt as well. 
	-If the name is not given then “radioButton” is used


.p(image="URL", size=<width>x<height>)
	-If the image is not given the a default image from placehold.it is used.


.n()
	-inserts a <br> tag


.s()
	-prints the form for selecting a stream
	-accepts no additional arguments
.f()
	-prints the form to accept a new post
	-accepts no additional arguments



*****************
Known Limitations
*****************

All file and links that are given must use ether relative or absolute file paths.
File names are not parsed.

Any parameter that is given in side of “” must not contain any “ characters.
Any parameter names must not contain any ‘,’ ‘=‘ or ‘)’ character2.


all of the tags can accept additional arguments, but if they have not been implemented then they will be ignored. Every argument must be in the form of:
paramName = data or 
paramName = “data” 
or ,data, 

if there are no “” then whitespace is removed.
All the paramNames and tags are case sensitive.


Any character that is given into the program must be an ASCII character (not unicode)

files can not be modified at the same time that they are being read
	NOTE: I believe this is not actually an issue since OS will not allow it

the files must be in the correct format.


stream files must be in the format

userID
YYY-MM-DD:HH:mm:ss
text
...
...
...


where:

YYYY (1900 - 9999)
MM   (01 - 12)
DD   (01 - 31)
HH   (00 - 23)
mm   (00 - 59)
ss   (00 - 59)


*****************
Testing
*****************

tested all 3 programs with users that did not exist.
tested all 3 programs with streams that had 2 words

post was tested with extra whitespace to check that it would not be left out
(note that the final new line for ctrl-D is not included)

Post was tested with a line that contains more then 80 characters to make sure that line wrapping worked


*****************
Future improvements 
*****************


changes the forms to accept multiple input types.

keep track of the stream name using JSON packets


*****************
References
*****************
 
https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm
was used nohow to use the time functions

https://www.tutorialspoint.com/python/
https://docs.python.org/3/reference
were used on how to use python and curses

w3schools.com
Was used on PHP, CSS, javascript and HTML tag reference.

The entire input file is from a previous assignment I did for CIS2520 that I wrote.
that buffers and will read a line of input from STDIN.


The PHP template code is based off of previous code I have written for my job and personal web page.



image place holder is from placehold.it
