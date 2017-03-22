#!/usr/local/bin/python3
# change use on the server
#!/usr/bin/python3

################################ loadStreams.py ################################
# Student Name: Marshall Aaron Asch            Student Number:  0928357
# Date: March 14, 2017                    	   Course Name: CIS*2750
# Assignment: A3
#
# Will load all of the streams that the user has permission in and
# print them to the standard out 1 per line
#
##########################################################################


# neccisary includes
import os
import sys

# get the name of all the streams in the messages dirrectory
# if there are no streams returns an empty list
def getStreams():
	fileList = os.listdir("messages")
	streamList = []

	#go through all the files and get just the streamNames
	for file in fileList:

		#ignore the users and data files
		if (file.endswith("Stream")):

			#cut of the Stream part of the name
			endPos = file.find("Stream", 0)
			if (endPos != -1):
				file = file[0:endPos]
			#add to new list
			streamList.append(file)
	return streamList

# get the list of streams that the user
# has permission to read
# 
def getUserStreams(userID):

	#all the streams
	streams = getStreams()

	#the list of streams the user is in
	streamsUsersIn = []

	#check every stream user file for the userName
	for stream in streams:
		streamUser = "messages/" + stream + "StreamUsers"
		file = open(streamUser, "r", 1)

		#search the entire file
		while True:
			line = file.readline()

			#reached the end of the file
			if line == "":
				break

			# find the seperater bwtween the userID and the number
			splitInd = line.rfind(" ")
			if(splitInd == -1):
				streamsUsersIn = []
				break

			#make sure the user has permission
			if (userID == line[0:splitInd].strip()):
				streamsUsersIn.append(stream)
				break

		file.close()

	return streamsUsersIn


# main entry point for the program
#
def main():

	# get the command line arguments 
	argc = len(sys.argv)
	argv = sys.argv

	# make sure that the the user gave a userName
	# exit if name is not given
	if (argc == 1):
		print("usage: ./loadStreams.py userID")
		exit(1)

	# get the name of the user into  1 string
	argv = argv[1:argc]
	userID = ' '.join(argv)

	# get all the streams the user has permission in
	streams = getUserStreams(userID)

	# if they dont have permission end
	if (streams == []):
		exit(1)

	for stream in streams:
		print(stream)

# run main
#
main()

		




