
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h> 

#define MSGBUFFER 500
#define HANDLE 64

// This function is called when a system call fails 
void errorMsg(const char *msg)
{
    printf("Something went wrong.\n");
    printf("%s\n", msg);
    exit(1);
}

int main(int argc, char *argv[])
{

    // Program variables:
    // Set up some variables for the program
    // hostName     : to store the hostname from command line
    // buffer: to store the user enter for a message
    // outgoingMsg : to store the final constructed message (userName + outgoingMsg) 
    // userName      : to store the user name and set it a prompt
    // quit  :stores /quit for quitting
    // MessageLength: to store the messages lenghts
    // status       : varibale to set flag to continue a session between client and server
    // i            : index for loop


	//Initialize the socket struct
    int sockfd, portNum;

  //  const char *host = "localhost";

    struct sockaddr_in server_address;
    struct hostent *server;
    const char *hostname = "localhost";

    char hostName[HANDLE];
    char outgoingMsg[MSGBUFFER];
    char buffer[MSGBUFFER];
    char userName[HANDLE];
    size_t messageLength;
    int i, n, status = 0;
   
    char *quit = (char*) malloc(4);

    if (argc < 3) {
       fprintf(stderr, "Please provide host or port.\nUsage: $ %s Hostname port\n", argv[0]);
       exit(1);
    }

    // Get the host name and the port from the command line
    sprintf(hostName,"%s",argv[1]);
    portNum = atoi(argv[2]);

    // Get the user name
	printf("Please enter your name (no spaces): ");
	fgets(userName, HANDLE, stdin);
	size_t length = strlen(userName) - 1;
	if (userName[length] == '\n')
			userName[length] = '\0';

	strcat(userName, " >>");

	// Creatig the socket  
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("Error opening socket");

    // Get the address of the host
    server = gethostbyname(hostName);
    if (server == NULL) {
        fprintf(stderr,"Error: no such host\n");
        exit(1);
    }

    // Initlize the server address
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(portNum);

    // Connect to the server
    if (connect(sockfd,(struct sockaddr *) &server_address,sizeof(server_address)) < 0) 
        errorMsg("Error: connecting to server.");

    // Send the start message to the server
    n = write(sockfd,"PORTNUM",7);
    if (n < 0) 
         errorMsg("Error: writing to socket");

    // While there is a connection keep sending and receiving messages
    while(status == 0) {

    	// display userName and get the user message
    	printf("%s ", userName);
    	bzero(buffer,500);
	    fgets(buffer, 500, stdin);

	    messageLength = strlen(buffer) - 1;
		if (buffer[messageLength] == '\n')
    			buffer[messageLength] = '\0';

    	// Construct the message + userName to be sent to server
    	bzero(outgoingMsg,500);
    	strcat(outgoingMsg, userName);
    	strcat(outgoingMsg, " ");
    	strcat(outgoingMsg, buffer);

    	// send the message to the server
    	n = write(sockfd,outgoingMsg,500);
    	if (n < 0) 
	         errorMsg("ERROR writing to socket");

	    // Check if the user sent a "quit" message to exit connection
	    messageLength = strlen(outgoingMsg);
		for (i=0; i< messageLength ; i++)
		{
			if (outgoingMsg[i] == '>')
				break;
		}
		i = i + 3;
		bzero(quit,5);
		strncpy(quit, outgoingMsg + i , 5);
		if (strcmp(quit,"\\quit") == 0)
		{
			printf ("Client sent a quit message! will quit now!\n");
			status = 1;
			break;
		}

		// If the message is not "quit" then continue the connection
	    bzero(buffer,500);
	    bzero(quit,5);
	    n = read(sockfd,buffer,500);
	    if (n < 0)
			errorMsg("Error reading from socket");

		// Check if the server sent a "quit" message to end connection
		strncpy(quit, buffer + 10 , 5);

		if (strcmp(quit,"\\quit") == 0)
		{
			printf ("Server Sent a quit message! will quit now!\n");
			status = 1;
			break;
		}
		// if not display the message
    	printf ("%s\n",buffer);
    }
    
   	// freeing the memory from quit allocation
    free(quit);
	   
    // Close the socket / end of the request
    close(sockfd);
    
    return 0;
}