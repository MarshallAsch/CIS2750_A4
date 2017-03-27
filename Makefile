BUILD_DIR = out
LIB_DIR = lib
INCLUDE_DIR =


# lists of .c files
MY_LIB_SRC = mystring.c input.c
STREAM_LIB_SRC = stream.c dbAccess.c

A1_SRC = C++lite.c tokenStruct.c list.c lists.c typeCheck.c
POST_SRC = post.c
ADD_AUTHOR_SRC = addauthor.c
DEL_AUTHOR_SRC = removeauthor.c
AUTHENTICATE_SRC = authenticateUser.c
GEN_HTML_SRC = genHTML.c fieldList.c
DATA_BASE_SRC = db.c
DB_ACCESS_SRC = dbAccess.c


# lists of .o files
A1_OBJ =  $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(A1_SRC)))
POST_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(POST_SRC)))
ADD_AUTHOR_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(ADD_AUTHOR_SRC)))
DEL_AUTHOR_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(DEL_AUTHOR_SRC)))
AUTHENTICATE_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(AUTHENTICATE_SRC)))
GEN_HTML_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(GEN_HTML_SRC)))
DATA_BASE_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(DATA_BASE_SRC)))
DB_ACCESS_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(DB_ACCESS_SRC)))
MY_LIB_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(MY_LIB_SRC)))
STREAM_LIB_OBJ = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(STREAM_LIB_SRC)))



# my list of libraries
LIBRARIES = $(LIB_DIR)/libmylib.a $(LIB_DIR)/libstream.a

# list of all the excecutables to build
EXECS = db post addauthor genHTML auth removeauthor


LIBS = -Llib -lmylib -lstream
CC = gcc
CFLAG =  -c -Wall -ansi -g  -I$(INCLUDE_DIR) -fPIC
LFLAG = -lc -lm -lmysqlclient $(LIBS)



all:  $(EXECS)
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

db: $(DB_ACCESS_OBJ) $(DATA_BASE_OBJ) $(LIBRARIES)
	$(CC) $(DB_ACCESS_OBJ) $(DATA_BASE_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR GENHTML
#
#**********************************

genHTML: $(GEN_HTML_OBJ)  $(LIBRARIES)
	$(CC) $(GEN_HTML_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR POST
#
#**********************************

post: $(POST_OBJ) $(LIBRARIES)
	$(CC) $(POST_OBJ) -o $@ $(LFLAG)

# to convert the C++ into the C using A1
post.c: a1 post.cc
	./a1 post.cc

#**********************************
#
#	BUILDING CODE FOR AUTH
#
#**********************************

auth: $(AUTHENTICATE_OBJ) $(LIBRARIES)
	$(CC) $(AUTHENTICATE_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR ADDATHTOR
#
#**********************************

addauthor: $(ADD_AUTHOR_OBJ) $(LIBRARIES)
	$(CC) $(ADD_AUTHOR_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR REMOVEATHTOR
#
#**********************************

removeauthor: $(DEL_AUTHOR_OBJ) $(LIBRARIES)
	$(CC) $(DEL_AUTHOR_OBJ) -o $@ $(LFLAG)


#**********************************
#
#	BUILDING CODE FOR A1
#
#**********************************

a1: $(A1_OBJ) $(LIBRARIES)
	$(CC) $(A1_OBJ) -o $@ $(LFLAG)


#**********************************
#
#
#	BUILDING LIBRARIES
#
#
#**********************************


$(LIB_DIR)/libmylib.a: $(MY_LIB_OBJ) | $(LIB_DIR)
	ar cr $@ $(MY_LIB_OBJ)



$(LIB_DIR)/libstream.a: $(STREAM_LIB_OBJ) | $(LIB_DIR)
	ar cr $@ $(STREAM_LIB_OBJ)



#**********************************
#
#
#
#		OTHER STUFF
#
#
#**********************************


#generic build a C file with a header file
$(BUILD_DIR)/%.o: %.c %.h  | $(BUILD_DIR)
	$(CC) $(CFLAG) $< -o $@

#generic build a C file with no header file
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAG) $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(LIB_DIR):
	mkdir $(LIB_DIR)

.PHONY: clean

clean:
	rm -f -r $(BUILD_DIR)
	rm -f -r $(LIB_DIR)
	rm -f $(EXECS)

	rm -f -r tmp
	rm -f post.c
	rm -f -r build
	rm -f *.so


.PHONY: destroy
destroy: 
	make db
	./db -reset
	make clean

tmp:
	mkdir tmp


#to package the code to be submitted
tar:
	make clean
	tar cf a4.tar *.h *.c *.cc *.php *.wpml *.js *.css *.py Makefile README.txt divClose.txt divOpen.txt

