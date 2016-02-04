#!/usr/bin/python

#	Name: Andrew Michael Calhoun
#	Class: CS372 Introduction to Networks
#	Project 1
#	Filename: chatclient.py

# Dependencies
from struct import *  # https://docs.python.org/2/library/struct.html
import os
import getopt
import signal
import sys
import socket


# FUNCTIONS

# takes a hostname and port number
# opens a socket and connects to host on the port
# socket file descriptor

def client_connect(host, port):
	if port < 1024 or port > 65535:
		print "Ports must be in range 1024-65535" # Ports less than 1024 may conflict, and greater than 65535 do not work because 16-bit overflow.
		sys.exit(0)

	# open and connect to a socket
	clientSocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	
	try:
		clientSocket.client_connect((host, port))
	
	except: 
		print "Error connecting to " + host + " : " + str(port)
		sys.exit(0)

	return clientSocket

	# Based off Socket Programming Primer Lecture

	# sentence = raw_input('Input lowercase sentence:')
	# clientSocket.send(sentence)
	# modifiedSentence = clientSocket.recv(1024)

	# print 'From Server:', modifiedSentence
	# clientSocket.close()

# From https://docs.python.org/2/library/struct.html

def sendMessage(sockfd, outMsg):
	size = len(outgoing) 	# length of the message
	data = pack('I', size)	# http: https://docs.python.org/2/library/struct.html
							# 's' is string size
	sockfd.send(data)		# size of the message
	sockfd.send(outgoing)	# send the message itself


def receiveMsg(sockfd):
	data = sockfd.recv(4)
	recvsize = unpack('I', data)
	incoming = sockfd.recv(recvsize[0])
	return incoming

def getUserName():
	validUserName = False
	while validUserName == False:
		validUserName = True
		print "Enter your user name, one word, up to 16 characters.\nPlease avoid using spaces or special characters."
		handle = raw_input()
		for characters in handle:
			if character == ' ':
			# if character ==  ' ' || '!' || '@' || '#' || '$' || '%' || '^' || '&' || '*' || '(' || ')' || '-' || '=' || '_' || '+' || '[' || ']' || '|' || '\\' || '{' || '}' || ';' || ':' || '\'' || '"' || '<' || '>' || '/' || ',' || '.' || '/' || '?':
				validUserName = False
				print("Invalid User Name: May not contain spaces or special characters.")
			if len(handle) > 16:
				validUserNAme = False
				print("Invalid User Name: Must be 16 characters or less.")
		return userName



# MAIN

# cmd line args
if len(sys.argv) != 3:
	print "\nUsage:\n $ python chatclient.py [hostname] [port number]\n\n" 
else:
	# connect to the server -- no server... no worky.
	serverConnect = client_connect(sys.argv[1], int(sys.argv[2]))

	# get the user name
	userName = getUseName()

	quit = False
	while quit == False:
		print (userName + "$"),
		message = raw_input()
		if message == "\quit":
			quit = True

	if quit == True:
		outMsg = "\quit"
	else:
		message = (userName + " > " + message) # append to username.

		sendMessage(serverConnect, message)

	if quit == False:
		incMsg = receiveMsg(clientSocket);
		if incMsg == "quit":
			printf("Connection reset by server; exiting.\n")
			clientSocket.close()
			quit = True
		else:
			print incMsg

	else:
		clientSocket.close()


#if __name__ == '__main__':
#	client_connect(host, port)



