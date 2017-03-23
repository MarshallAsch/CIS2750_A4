#!/usr/local/bin/python3
# change use on the server
#!/usr/bin/python3

################################ markAll.py ################################
# Student Name: Marshall Aaron Asch            Student Number:  0928357
# Date: March 14, 2017                    	   Course Name: CIS*2750
# Assignment: A3
#
# Will mark all post for the given userID and stream as read.
# Checks to make sure the the user has permission in the stream
#
#
##########################################################################


# neccisary includes
import os
import sys
import time
import textwrap



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

# returns a list of all the projects in the given list
# of streams. The list is not sorted
#
def loadPosts(streams, userID):

	posts = []

	# load all the posts that are in the files in the list
	for stream in streams:

		# generate the file names
		streamUsers = "messages/" + stream + "StreamUsers"
		streamData = "messages/" + stream + "StreamData"
		streamPosts = "messages/" + stream + "Stream"

		numRead = 0

		#get the position that the user is currently at
		file = open(streamUsers, "r", 1)

		#search the entire file for the userID
		while True:
			line = file.readline()

			#reached the end of the file
			if line == "":
				break

			# find the seperater bwtween the userID and the number
			splitInd = line.rfind(" ")
			if(splitInd == -1):
				break

			# check for the users name in the file
			if (userID == line[0:splitInd].strip()):
				numRead =  int(line[splitInd:].strip())
				break

		file.close()

		startBytes = []

		# open the file that contains the start byte position for each post
		file = open(streamData, "r", 1)

		# read the entire file and get the start postions of each post
		while True:
			line = file.readline()

			# reached the end of the file
			if (line == ""):
				break

			# add the start postion to the list of positions
			startBytes.append(int(line.strip()))

		file.close()

		# open the file that contains all the posts
		file = open(streamPosts, "r", 1)

		for i in range(len(startBytes)):
			file.seek(startBytes[i])

			# read from the start of the start postion to the start of next (or EOF)
			if (i + 1 < len(startBytes)):
				line = file.read(startBytes[i + 1] - startBytes[i])
			else:
				line = file.read()

			# split it into 3 tokens
			data = line.split("\n", 2)

			# create the post
			if (i < numRead):
				post = Post(data[0], data[1], stream, data[2], startBytes[i], True)
			else:
				post = Post(data[0], data[1], stream, data[2], startBytes[i], False)

			# add the post to the end of the list
			posts.append(post)

		file.close()

	#return the list of post objects
	return posts

# checks wether or not the user is allowed to view that stream
#
def checkPermission(stream, allStreams):

	for name in allStreams:
		name = name.strip()

		if (name == stream):
			return True


	return False


# Marks all the posts in the given list of streams as read
#
def markAll(stream, userID):
	# generate the file names
	streamUsers = "messages/" + stream + "StreamUsers"
	streamData = "messages/" + stream + "StreamData"

	numRead = 0

	# get the position that the user is currently at
	file = open(streamData, "r", 1)

	# count the number of posts in the stream
	while True:
		line = file.readline()

		#reached the end of the file
		if line == "":
			break

		numRead += 1
	file.close()

	lineBuff = []

	# open the file that contains all the users
	file = open(streamUsers, "r", 1)

	#read the enrtire file in
	while True:
		line = file.readline()

		#reached the end of the file
		if (line == ""):
			break

		#write the line to the into the buffer
		lineBuff.append(line)
	file.close()

	file = open(streamUsers, "w", 1)

	#change the users count and write it all back to the file
	for line in lineBuff:

		# find the seperater bwtween the userID and the number
		splitInd = line.rfind(" ")
		if(splitInd == -1):
			print ("error")
			break

		# check for the users name in the file
		if (userID == line[0:splitInd].strip()):
			line = str(userID) + " " + str(numRead) + "\n"
		file.write(line)

	file.close()


# main entry point for the program
#
def main():

	# get the command line arguments
	argc = len(sys.argv)
	argv = sys.argv

	# make sure that the the user gave a userName
	# exit if name is not given
	if (argc != 3):
		print("usage: ./markOne.py \"userID\" \"stream\"")
		exit(1)

	# get the name of the user into  1 string
	userID = argv[1].strip()
	stream = argv[2].strip()

	# get all the streams the user has permission in
	allStreams = getUserStreams(userID)

	# if they dont have permission end
	if (allStreams == []):
		print("{\"data\": {\"status\": \"false\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\", \"msg\": \"User has no permission in streams\"} }")

		exit(1)

	# get the stream to view
	status = checkPermission(stream, allStreams)

	if (status == False):
		print("{\"data\": {\"status\": \"false\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\", \"msg\": \"permission in that stream denied\"} }")

		exit(1)

	markAll(stream, userID)
	print("{\"data\": {\"status\": \"true\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\"} }")



# run main
#
main()






