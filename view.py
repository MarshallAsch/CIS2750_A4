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
import dbwrapper

# class that holds all the data for each post
#
class Post:
	"""Object used to hold all of the info assosiated with a
	given post """

	# Creates a new instance of the object.
	# The time is stored as a tuple time struct
	# counts the number of lines in the post and saves that too
	def __init__(self, author, date, stream, message, viewed = 0):

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
		self.date = time.strptime(date, "%Y-%m-%d %H:%M:%S")
		self.sortDate = self.getDateNum()
		self.stream = stream
		self.message = message

		if (viewed == 1):
			self.viewed = True
		else:
			self.viewed = False



	# create a string that has all of the data of the post
	def __repr__(self):
		return 'author=%s\nstream=%s\ndate=%s\nnumLines=%d\nstartPos=%d\nviewed=%s\nmessage=%s\n' % (self.author, self.stream, time.asctime(self.date), self.numLines, self.startPos, self.viewed, self.message)

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


def displayPosts(posts):

	if (len(posts) > 0):
		print("{\"data\": [", end="")
		for i in range(len(posts)-1):
			print(posts[i].JSON(), end=", ")
		print(posts[len(posts)-1].JSON(), end="")
		print("]}", end="")
	else:
		print("{\"data\": [{\"stream\": \"NONE\", \"author\": \"NONE\", \"date\": \"2017-03-08:12:01:01\",  \"viewed\": \"false\", \"text\": \"NONE\"}]}")


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
	stream = argv[2]

	postList = []
	posts = []

	#check if they are marking all as ready
	if (stream == "all"):
		allStreams = dbwrapper.getUsersStreams(userID)

		# if they dont have permission end
		if (allStreams == []):
			print("{\"data\": {\"status\": \"false\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\", \"msg\": \"User has no permission in streams\"} }")

			exit(1)

		# mark each stream as read
		for stream in allStreams:
			tempPosts = dbwrapper.getStreamPosts(userID, stream)
			postList.append(tempPosts);

	else:
		#get the posts from teh one stream
		tempPosts = dbwrapper.getStreamPosts(userID, stream)
		postList.append(tempPosts)



	for tempObj1 in postList:

		#get the list of streams from the object
		tempPostList = tempObj1[2]

		for post in tempPostList:
			post = Post(post[2], post[3], post[1], post[4], post[5])
			posts.append(post)


	# if they dont have permission end
	if (posts == []):
		print("{\"data\": [{\"stream\": \"NONE\", \"author\": \"NONE\", \"date\": \"2017-03-08:12:01:01\", \"viewed\": \"False\", \"text\": \"NONE\"}]}")
		exit(1)

	# load and sort the posts by date
	posts = sort(posts)

	displayPosts(posts)



# run main
#
main()






