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
import sys
import textwrap


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


	#check if they are marking all as ready
	if (stream == "all"):
		allStreams = getUsersStreams(userID)

		# if they dont have permission end
		if (allStreams == []):
			print("{\"data\": {\"status\": \"false\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\", \"msg\": \"User has no permission in streams\"} }")

			exit(1)

		# mark each stream as read
		for stream in allStreams:
			markAll(userID, stream)

		print("{\"data\": {\"status\": \"true\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\"} }")

		exit(0)

	# mark a single stream as read
	markAll(userID, stream)
	print("{\"data\": {\"status\": \"true\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\"} }")
	exit(0)


# run main
#
main()






