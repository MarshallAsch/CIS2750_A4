#!/usr/local/bin/python3
# change use on the server
#!/usr/bin/python3

################################ view.py ################################
# Student Name: Marshall Aaron Asch            Student Number:  0928357
# Date: March 14, 2017                  	   Course Name: CIS*2750
# Assignment: A3
#
# Loads all of the posts in the given streams and prints a JSON 
# package with that info on success
#
# {data: [{stream: "..", author: "..", date: "..", viewed: "..", text: ".."}, ... ]}
#
# or
# {data: [{stream: "NONE", author: "NONE", date: "2017-03-08:12:01:01", viewed: "false", text: "NONE"}] }
#
#
##########################################################################


# neccisary includes
import os
import sys
import time
import textwrap

# class that holds all the data for each post 
#
class Post:
	"""Object used to hold all of the info assosiated with a
	given post """

	# Creates a new instance of the object.
	# The time is stored as a tuple time struct
	# counts the number of lines in the post and saves that too
	def __init__(self, author, date, stream, message, position, viewed = False):

		#break the message into lines that are less then 80 long
		text = []
		lines = message.splitlines()
		for line in lines:
			line = textwrap.fill(line, 79)
			text.append(line)

		# put the messages back into 1 string
		message = "<br>".join(text)

		# set the object fields
		self.author = author
		self.datestr = date
		self.date = time.strptime(date, "%Y-%m-%d:%H:%M:%S")
		self.sortDate = self.getDateNum()
		self.stream = stream
		self.message = message
		self.position = position
		self.viewed = viewed


	# create a string that has all of the data of the post
	def __repr__(self):
		return 'author=%s\nstream=%s\ndate=%s\nposition=%d\nnumLines=%d\nstartPos=%d\nviewed=%s\nmessage=%s\n' % (self.author, self.stream, time.asctime(self.date), self.position, self.numLines, self.startPos, self.viewed, self.message)

	# create a string representation of the object used for printing
	def __str__(self):
		return 'Stream: %s\nSender: %s\ndate: %s\n%s-----------------------------------------' % (self.stream, self.author, time.asctime(self.date), self.message)

	def JSON(self):
		return '{"stream": "%s", "author": "%s", "date": "%s", "viewed": "%s", "text": "%s"}' % (self.stream, self.author, self.datestr, self.viewed, self.message)

	# gets the time of the post in integer form
	# used for soting by date
	# YYYYMMDDHHmmss
	def getDateNum(self):
		time = self.date.tm_sec + self.date.tm_min * 100 + self.date.tm_hour * 10000
		time += self.date.tm_mday * 1000000 + self.date.tm_mon * 100000000 + self.date.tm_year * 10000000000
		return time

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

# FOR TESTING
# REMOVE ME
def displayPosts(posts):

	if (len(posts) > 0):
		print("{\"data\": [", end="")
		for i in range(len(posts)-1):
			print(posts[i].JSON(), end=", ")
		print(posts[len(posts)-1].JSON(), end="")
		print("]}", end="")
	else:
		print("{\"data\": [{\"stream\": \"NONE\", \"author\": \"NONE\", \"date\": \"2017-03-08:12:01:01\",  \"viewed\": \"false\", \"text\": \"NONE\"}]}")


# Takes in a list of all the streams that the user has permission in
# and asks the user what stream they want to look at
# loops untill they make a valid choice
#
def getStreamsToShow(streams, allStreams):

	streams = streams.strip()

	streamList = []
	listIn = streams.split(",")

	isValid = False

	for name in listIn:
		name = name.strip()

		isValid = False

		if (name == "all"):
			return allStreams

		#check to see which stream the user wasnts to see
		for stream in allStreams:
			if (name == stream):
				streamList.append(stream)
				isValid = True
				break

		# make sure theat the name is valid
		if (isValid ==  False):
			break

	return streamList

# sorts the list if posts
# ether by date if        option == 0
#    or by author if      option == 1
#    or by stream name if option == 2
def sort(posts, option = 0):

	# sort baised on the option given 
	if (option == 0):
		return sorted(posts, key=lambda post: post.sortDate)
	elif (option == 1):
		return sorted(posts, key=lambda post: post.author)
	elif (option == 2):
		return sorted(posts, key=lambda post: post.stream)


# main entry point for the program
#
def main():

	# get the command line arguments 
	argc = len(sys.argv)
	argv = sys.argv

	# make sure that the the user gave a userName
	# exit if name is not given
	if (argc != 3):
		print("usage: ./view.py \"userID\" \"streamList\"")
		exit(1)

	# get the name of the user into  1 string
	userID = argv[1]
	streams = argv[2]

	# get all the streams the user has permission in
	allStreams = getUserStreams(userID)

	# if they dont have permission end
	if (allStreams == []):
		print("{\"data\": [{\"stream\": \"NONE\", \"author\": \"NONE\", \"date\": \"2017-03-08:12:01:01\", \"viewed\": \"false\", \"text\": \"NONE\"}]}")
		exit(1)

	# get the stream to view
	streamsToShow = getStreamsToShow(streams, allStreams)

	# load and sort the posts by date
	posts = loadPosts(streamsToShow, userID)
	posts = sort(posts)

	displayPosts(posts)



# run main
#
main()

		




