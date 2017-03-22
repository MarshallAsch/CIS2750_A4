A1_OBJ =  out/C++lite.o out/tokenStruct.o out/list.o out/lists.o out/typeCheck.o 
LIBRARIES = lib/libmystr.a lib/libstream.a
postOBJ = out/post.o out/input.o 
addAuthorOBJ = out/addauthor.o out/input.o 
delAuthorOBJ = out/removeauthor.o out/input.o 
authOBJ = out/authenticateUser.o out/input.o 
HTML_OBJ = out/genHTML.o out/fieldList.o
db_OBJ = out/db.o out/dbAccess.o

LIBS = -Llib -lmystr -lstream
CC = gcc
CFLAG =  -c -Wall -ansi -g
LFLAG = -lc -lm $(LIBS)



all: db post addauthor genHTML auth removeauthor
	chmod +x setup.py
	./setup.py install
	chmod +x view.py
	chmod +x loadStreams.py
	chmod +x markOne.py
	chmod +x markAll.py




#**********************************
#
#
#	BUILDING CODE FOR DB
#
#
#**********************************

db: lib out $(db_OBJ) $(LIBRARIES)
	$(CC) $(db_OBJ) -o $@ $(LFLAG)



out/db.o: db.c
	$(CC) $(CFLAG) $< -o $@

out/dbAccess.o: dbAccess.c
	$(CC) $(CFLAG) $< -o $@






#**********************************
#
#
#	BUILDING CODE FOR GENHTML
#
#
#**********************************

genHTML: lib out $(HTML_OBJ) $(LIBRARIES)
	$(CC) $(HTML_OBJ) -o $@ $(LFLAG)



out/genHTML.o: genHTML.c
	$(CC) $(CFLAG) $< -o $@

out/fieldList.o: fieldList.c
	$(CC) $(CFLAG) $< -o $@


#**********************************
#
#
#	BUILDING CODE FOR POST
#
#
#**********************************

post: tmp messages lib out $(postOBJ) $(LIBRARIES)
	$(CC) $(postOBJ) -o $@ $(LFLAG)



out/post.o: post.c
	$(CC) $(CFLAG) $< -o $@

out/input.o: input.c
	$(CC) $(CFLAG) $< -o $@

# to convert the C++ into the C using A1
post.c: a1 post.cc
	./a1 post.cc

#**********************************
#
#
#	BUILDING CODE FOR AUTH
#
#
#**********************************

auth: lib out $(authOBJ) $(LIBRARIES)
	$(CC) $(authOBJ) -o $@ $(LFLAG)


out/authenticateUser.o: authenticateUser.c
	$(CC) $(CFLAG) $< -o $@



#**********************************
#
#
#	BUILDING CODE FOR ADDATHTOR
#
#
#**********************************

addauthor: lib out $(addAuthorOBJ) $(LIBRARIES)
	$(CC) $(addAuthorOBJ) -o $@ $(LFLAG)


out/addauthor.o: addauthor.c
	$(CC) $(CFLAG) $< -o $@

#**********************************
#
#
#	BUILDING CODE FOR REMOVEATHTOR
#
#
#**********************************

removeauthor: lib out $(delAuthorOBJ) $(LIBRARIES)
	$(CC) $(delAuthorOBJ) -o $@ $(LFLAG)


out/removeauthor.o: removeauthor.c
	$(CC) $(CFLAG) $< -o $@


#**********************************
#
#
#	BUILDING CODE FOR A1
#
#
#**********************************

a1: lib out $(A1_OBJ) $(LIBRARIES)
	$(CC) $(A1_OBJ) -o $@ $(LFLAG)


out/C++lite.o: C++lite.c
	$(CC) $(CFLAG) $< -o $@

out/tokenStruct.o: tokenStruct.c
	$(CC) $(CFLAG) $< -o $@

out/list.o: list.c
	$(CC) $(CFLAG) $< -o $@

out/lists.o: lists.c
	$(CC) $(CFLAG) $< -o $@

out/typeCheck.o: typeCheck.c
	$(CC) $(CFLAG) $< -o $@




#**********************************
#
#
#	BUILDING LIBRARIES
#
#
#**********************************


lib/libmystr.a: out/mystring.o
	ar cr $@ $<

lib/libstream.a: out/stream.o
	ar cr $@ $<


out/stream.o: stream.c
	$(CC) $(CFLAG) $< -o $@


out/mystring.o: mystring.c
	$(CC) $(CFLAG) $< -o $@


#**********************************
#
#
#
#		OTHER STUFF 
#
#
#**********************************

clean:
	rm -f -r out
	rm -f -r lib
	rm -f -r tmp
	rm post.c
	rm a1
	rm post
	rm addauthor
	rm genHTML
	rm auth
	rm removeauthor


destroy:
	make clean
	rm -f -r messages

# building the directories 
out:
	mkdir out

messages: 
	mkdir messages
	chmod 777 messages
tmp:
	mkdir tmp

lib:
	mkdir lib


#to package the code to be submitted
tar:
	make destroy
	tar cf a2.tar *.h *.c *.cc *.php *.wpml *.js *.css *.py Makefile README.txt divClose.txt divOpen.txt
	
