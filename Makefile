
OUT_DIR = out/
LIB_DIR = lib/

LIBRARIES = $(LIB_DIR)libmystr.a $(LIB_DIR)libstream.a

A1_SRC =  C++lite.c tokenStruct.c list.c lists.c typeCheck.c
postSRC = post.c input.c
addAuthorSRC = addauthor.c input.c
delAuthorSRC = removeauthor.c input.c
authSRC = authenticateUser.c input.c
HTML_SRC = genHTML.c fieldList.c
db_SRC = db.c dbAccess.c







A1_OBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(A1_SRC)))
postOBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(postSRC)))
addAuthorOBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(addAuthorSRC)))
delAuthorOBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(delAuthorSRC)))
authOBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(authSRC)))
HTML_OBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(HTML_SRC)))
db_OBJ = $(addprefix $(OUT_DIR), $(patsubst %.c, %.o, $(db_SRC)))





LIBS = -L$(LIB_DIR) -lmystr -lstream
CC = gcc
CFLAG =  -c -Wall -ansi -g -fPIC
LFLAG = -lc -lm -lmysqlclient $(LIBS)


EXE = db post addauthor genHTML auth removeauthor


all: $(EXE)
	chmod +x setup.py
	./setup.py build
	cp build/lib.*/*.so .
	chmod +x view.py
	chmod +x loadStreams.py
	chmod +x markOne.py
	chmod +x markAll.py
	./db -init




#**********************************
#
#	BUILDING CODE FOR DB
#
#**********************************

db:  $(db_OBJ) $(LIBRARIES)
	$(CC) $(db_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR GENHTML
#
#**********************************

genHTML:  $(HTML_OBJ) $(LIBRARIES)
	$(CC) $(HTML_OBJ) -o $@ $(LFLAG)

#**********************************
#
#	BUILDING CODE FOR POST
#**********************************

post: $(postOBJ) $(LIBRARIES) | tmp
	$(CC) $(postOBJ) -o $@ $(LFLAG)


# to convert the C++ into the C using A1
post.c: a1 post.cc
	./a1 post.cc

#**********************************
#
#	BUILDING CODE FOR AUTH
#
#**********************************

auth:  $(authOBJ) $(LIBRARIES)
	$(CC) $(authOBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR ADDATHTOR
#
#**********************************

addauthor:  $(addAuthorOBJ) $(LIBRARIES)
	$(CC) $(addAuthorOBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR REMOVEATHTOR
#
#**********************************

removeauthor:  $(delAuthorOBJ) $(LIBRARIES)
	$(CC) $(delAuthorOBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR A1
#
#**********************************

a1: $(A1_OBJ) $(LIBRARIES)
	$(CC) $(A1_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING LIBRARIES
#
#**********************************


$(LIB_DIR)libmystr.a: $(OUT_DIR)mystring.o | $(LIB_DIR)
	ar cr $@ $<

$(LIB_DIR)libstream.a: $(OUT_DIR)stream.o $(OUT_DIR)dbAccess.o | $(LIB_DIR)
	ar cr $@ $^


#**********************************
#
#		OTHER STUFF
#
#**********************************

clean:
	rm -f -r $(OUT_DIR)
	rm -f -r $(LIB_DIR)
	rm -f -r tmp
	rm -f -r build
	rm -f post.c
	rm -f a1
	rm -f $(EXE)
	rm -f *.so


destroy:
	make db
	./db -rest
	make clean

# building the directories
$(OUT_DIR):
	mkdir $(OUT_DIR)

tmp:
	mkdir tmp

$(LIB_DIR):
	mkdir $(LIB_DIR)



#generic build a C file with no header file
$(OUT_DIR)%.o: %.c | $(OUT_DIR)
	$(CC) $(CFLAG) $< -o $@

#to package the code to be submitted
tar:
	make destroy
	tar cf a2.tar *.h *.c *.cc *.php *.wpml *.js *.css *.py Makefile README.txt divClose.txt divOpen.txt
