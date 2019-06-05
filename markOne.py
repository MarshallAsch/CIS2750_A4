#!/usr/local/bin/python3

################################ markOne.py ################################
# Student Name: Marshall Aaron Asch            Student Number:  0928357
# Date: March 14, 2017                    	   Course Name: CIS*2750
# Assignment: A3
#
# Will mark one post for the given userID and stream as read.
# Checks to make sure the the user has permission in the stream
# Checks that it does not mark more as read then are in the file
#
##########################################################################


# neccisary includes
import sys
import dbwrapper



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


	# get the stream to view
	status = dbwrapper.markOne(userID, stream)

	if (status != 0):
		print("{\"data\": {\"status\": \"false\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\", \"msg\": \"permission in that stream denied\"} }")

		exit(1)

	print("{\"data\": {\"status\": \"true\", \"userID\": \""+userID+"\", \"stream\": \""+stream+"\"} }")



# run main
#
main()






