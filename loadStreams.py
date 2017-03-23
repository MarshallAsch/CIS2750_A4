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
import dbwrapper

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
	streams = dbwrapper.getUsersStreams(userID)[2]

	# if they dont have permission end
	if (streams == []):
		exit(1)

	for stream in streams:
		print(stream[2])

# run main
#
main()






