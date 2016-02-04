
/* Andrew M. Calhoun
calhouna@oregonstate.edu
Due Date: 2/7/2016
Project I 
chatserver.cpp

// Some Patterns are based off of Beej's guide: 
// http://beej.us/guide/bgnet/output/html/multipage/clientserver.html#simpleserver

Multithreading based off of: 
// http://www.cplusplus.com/forum/unices/116977/

Chat Server Defines and Includes */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>


using namespace std;

// Message Buffer Should Be 500 or less.
#define MSGBUFFER 500
#define USERNAME "calhouna-server"

//void *task1(void *);
//static int connFd;

void errorMsg(const char *msg)
{
	cout << "Something went wrong.\n" << endl;
	cout << msg << endl;
	exit(1);
}

int main(int argc, char *argv[])
{
	// Program variables:

	// pid: Holds the process id number
	// buffer: stores input string
	// message: stores client msg
	// outgoingMsg: the outgoing message back to the client.
	// messageLength: message Length
	// i, status: indices for loops.
	// n is for 
	// server_address, client_address -- for the server and client addresses


	// signal(SIGCHLD, sigintHandle); Not neccessary in C++.
	// pthread_t threadA[3];
	int sockfd, newsockfd, portNum;
	pid_t pid;	// PID, listens for Python
	socklen_t client_length; // stores the size of the address

	struct sockaddr_in server_address, client_address;

	char buffer[MSGBUFFER];
	char message[MSGBUFFER];
	char user_name[] = USERNAME;
	char outgoingMsg[MSGBUFFER];
	size_t messageLength;
	int i, n, status = 0;

	char *quit = (char*)malloc(4);

	if(argc < 2)
	{
		cout << (stderr, "No port provided.\nUsage: \"$ chatserve [port]\"") << endl;
		exit(1);
	}


	// Initialize the socket

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		errorMsg("Problem opening socket.");

	bzero((char *) &server_address, sizeof(server_address));

	portNum = atoi(argv[1]);
	if((portNum > 65535) || (portNum < 1024))
		errorMsg("Please use range 1024-65335");

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(portNum);

	cout << "Opening Server Address..." << endl;
	
	if(bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
		errorMsg("Cannot bind to socket.");
		//return 0;
	

	listen(sockfd, 5);
	client_length = sizeof(client_address);

	// Infinite loop for client requests
	while(1) {
		
		cout << "Listening on port " << portNum << endl;
		newsockfd = accept(sockfd, (struct sockaddr *) &client_address, &client_length);
		cout << "listening: " << newsockfd;


		if(newsockfd < 0)
			errorMsg("Cannot accept connection. Please try again.");
		
		// Give a new process so that we can have multiple processes and multi-threading
		pid = fork();

		cout << "Process ID: " << pid << endl;

		if(pid < 0)
			errorMsg("Error on fork.");

		if(pid == 0)
		{
			// Close the socket.
			close(sockfd);
			// Clear the messages out of memory.
			bzero(message, 500);

			// Get the initial message.
			n = recv(newsockfd, message, 7, 0);
			if(n < 0)
				errorMsg("Error reading from socket.");
			if(strcmp(message, "Port Number") == 0)
			{
				cout << "Connection successful. Begin transmission...\n" << endl;
			}

		while(status == 0)
		{
			bzero(message, 500);
			n = recv(newsockfd,message,500,0);
			if(n<0)
				errorMsg("Error reading from socket.");

			messageLength = strlen(message);
			for(i=0; i < messageLength; i++)
			{
				if(message[i] == '>')
					break;
			}
			i = i + 3;

			bzero(quit, 5);
			strncpy(quit, message + i, 5);

			if(strcmp(quit,"\\quit") == 0)
			{
				cout << "Client has quit, closing the connection." << endl;
				status = 1;
				break;
			}

		cout << message << endl;

		cout << user_name << " >> ";

		bzero(buffer, 500);
		fgets(buffer, 500, stdin);
		
		messageLength = strlen(buffer) - 1;
		
		if(buffer[messageLength] == '\n')
			buffer[messageLength] = '\0';

		bzero(outgoingMsg, 500);
		strcat(outgoingMsg, user_name);
		strcat(outgoingMsg, " ");
		strcat(outgoingMsg, buffer);

		n = send(newsockfd,outgoingMsg,500,0);
		if(n<0)
			errorMsg("Error writing to socket");
		
		bzero(quit,5);
		
		strncpy(quit, outgoingMsg + 10, 5);
		
		if(strcmp(quit,"\\quit") == 0)
		{
			cout << "Server sent a quit message. Closing the connection." << endl;
			status = 1;
			break;
		}
	}
		
	free(quit);
	exit(0);
	
	} else
			close(newsockfd);
	}

	close(sockfd);
	return 0;
 }
