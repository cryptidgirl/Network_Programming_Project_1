#include <stdio.h>      	/* for printf() and fprintf() */
#include <stdlib.h>     	/* for atoi() and exit() */
#ifdef _WIN32                   /* IF ON A WINDOWS PLATFORM YOU WILL HAVE TO CHECK THIS */
#include <winsock2.h>           /* IF ON A WINDOWS PLATFORM YOU WILL HAVE TO CHECK THIS */
#include <ws2tcpip.h>           /* IF ON A WINDOWS PLATFORM YOU WILL HAVE TO CHECK THIS */
#else
#include <arpa/inet.h>  	/* for sockaddr_in and inet_ntoa() */
#include <sys/socket.h> 	/* for socket(), bind(), and connect() */
#include <string.h>     	/* for memset() */
#include <unistd.h>     	/* for close() */
#include <time.h>
#endif

#define MAXPENDING 5    	/* Maximum outstanding connection requests */

void DieWithError(char *errorMessage);  /* Error handling helper function */
void HandleClientTCP(int clntSocket);   /* TCP client handling function   */
void ProcessMain(int servSock);		/* Fork main function definition  */


int main(void)
{
    int TCPservSock_d;
    int UDPservSock_d;/* Socket descriptor for server */
    pid_t fork_ProcessID;		/* Fork Process ID from fork()  */
    unsigned int childProcessCount = 0; /* Number of child processes    */
    unsigned int processLimit;          /* Number of child processes to create */
    struct sockaddr_in echoServAddr;    /* Local address */
    unsigned short echoServPort;        /* Server port */

    echoServPort = atoi("3000");  	/* First arg:  should be local port */
    processLimit = atoi("5");  	/* Second arg:  should be limit for # of children */

    /* ------Step 1 create the socket ------- */
    /* Create socket for incoming connections */
    if ((TCPservSock_d = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");
    
      
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
    echoServAddr.sin_family = AF_INET;                /* Internet address family */
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    echoServAddr.sin_port = htons(echoServPort);      /* Local port */

    /* ------Step 2 bind the connection ip:port ------- */
    /* Bind to the local address */
    if (bind(TCPservSock_d, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");


    /* ------Step 3 listen to the socket for a connection ------- */
    /* Establish the socket to listen for incoming connections */
    if (listen(TCPservSock_d, MAXPENDING) < 0)
        DieWithError("listen() failed");

    for (childProcessCount=0; childProcessCount < processLimit; childProcessCount++) 
    {
        /* Fork child process and report any errors */
        if ((fork_ProcessID = fork()) < 0)
            DieWithError("fork() failed");
        else if (fork_ProcessID == 0)  /* If this is the child process */
            ProcessMain(TCPservSock_d);
    }
    exit(0);  /* The children will carry on */
    
    if ((UDPservSock_d = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError("socket() failed");
    
    if(bind(UDPservSock_d, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");
    
    struct sockaddr_in clientAddr;
    unsigned int clientAddrLen = sizeof(clientAddr);
    
    char buffer[500];
    
    int len = recvfrom(UDPservSock_d, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if(len < 0){
        DieWithError("recvfrom() failed");
    }
    buffer[len] = '\0';
    
    int hour;
    char am_or_pm;
    time_t current = time(NULL);
    
    if (strncmp("GMT", buffer, 3)) {
        struct tm *ptr;
        ptr = gmtime(&current);
        hour = ptr -> tm_hour;
        if (hour < 11)
            am_or_pm = 'a';
        else {
            hour -= 12;
            am_or_pm = 'p';
        }
        if (hour == 0)
            hour = 12;
        snprintf(buffer, sizeof(buffer),"%.2d-", ptr->tm_mon +1);
        snprintf(buffer, sizeof(buffer),"%.2d-", ptr ->tm_mday);
        snprintf(buffer, sizeof(buffer),"%d", ptr->tm_year +1900);
        snprintf(buffer, sizeof(buffer)," %2d:", hour);
        snprintf(buffer, sizeof(buffer),"%2d", ptr->tm_min);
        snprintf(buffer, sizeof(buffer),"%c\n", am_or_pm);
        sendto(UDPservSock_d, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }
    else if (strncmp("Local", buffer, 5)){
        struct tm *ptr;
        hour = ptr -> tm_hour;
        if (hour < 11)
            am_or_pm = 'a';
        else {
            hour -= 12;
            am_or_pm = 'p';
        }
        if (hour == 0){
            hour = 12;
        }
        
        snprintf(buffer, sizeof(buffer),"%.2d-", ptr->tm_mon +1);
        snprintf(buffer, sizeof(buffer),"%.2d-", ptr ->tm_mday);
        snprintf(buffer, sizeof(buffer),"%d", ptr->tm_year +1900);
        snprintf(buffer, sizeof(buffer)," %2d:", hour);
        snprintf(buffer, sizeof(buffer),"%2d", ptr->tm_min);
        snprintf(buffer, sizeof(buffer),"%c\n", am_or_pm);
        sendto(UDPservSock_d, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, clientAddrLen);
    }
    
    return 0;
}

void ProcessMain(int servSock)
{
    int clntSock;                  /* Socket descriptor for client connection */
    struct sockaddr_in echoClntAddr;    /* Client address */
    unsigned int clntLen;               /* Length of client address data structure */

    for (;;)  /* Run forever */
    {
    	/* Set the size of the in-out parameter */
    	clntLen = sizeof(echoClntAddr);
    
    	/* Wait for a client to connect */
    	if ((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, 
        	   &clntLen)) < 0)
        	DieWithError("accept() failed");
    
    	/* clntSock is connected to a client! */
    
    	printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        printf("With child process: %d\n", (unsigned int) getpid());
        //HandleClientTCP(clntSock);
    }
}

