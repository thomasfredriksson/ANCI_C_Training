
#include "winsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


void StartMain()
{
	SOCKADDR_IN internetAddr;
	int nPortId = 5150;
	WSADATA wsaData;
	int Ret;
	//Initialize Winsock version 2.2
	if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
	{
        printf("WSAStartup failed with error %d\n", Ret);
	    return;
	}
	//Setup Winsock communication code here
    internetAddr.sin_family = AF_INET;
    //Convert the proposed dotted Internet address 136.149.3.29
	//to a four-byte integer, and assign it to sin_addr
    internetAddr.sin_addr.s_addr = inet_addr("136.149.3.29");
    internetAddr.sin_port = htons(nPortId);
	//When your application is finished call WSACleanup
	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WSAStartup failed with error %d\n", WSAGetLastError());
	}
}

void BindSocket()
{
	SOCKET       s;
    SOCKADDR_IN  tcpaddr;
	int          port = 5150;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    tcpaddr.sin_family = AF_INET;
    tcpaddr.sin_port = htons(port);
    tcpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(s, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
}

// A sample of the select() return value
int recvTimeOutTCP(SOCKET socket, long sec, long usec)
{
    // Setup timeval variable
    struct timeval timeout;
    struct fd_set fds;
 
     // assign the second and microsecond variables
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    // Setup fd_set structure
    FD_ZERO(&fds);
    FD_SET(socket, &fds);
    // Possible return values:
    // -1: error occurred
    // 0: timed out
    // > 0: data ready to be read
    return select(0, &fds, 0, 0, &timeout);
}

int TcpServer()
{
	WSADATA        wsaData;
	SOCKET         ListeningSocket;
	SOCKET         NewConnection;
	SOCKADDR_IN    ServerAddr, SenderInfo;
    SOCKADDR_IN    ClientAddr;
    int            ClientAddrLen;
	int            Port = 5150;
	char           recvbuff[1024];
	int            ret;
	int            nLeft, nlen;
	int            idx, i;
	int            ByteReceived, SelectTiming;

    // Initialize Winsock version 2.2
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
        // The WSAGetLastError() function is one of the only functions
        // in the Winsock 2.2 DLL that can be called in the case of a WSAStartup failure
        printf("Server: WSAStartup failed with error %ld.\n", WSAGetLastError());
        // Exit with error
        return 1;
	}
	else
	{
        printf("Server: The Winsock DLL found!\n");
        printf("Server: The current status is %s.\n", wsaData.szSystemStatus);
	}

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
    {
        //Tell the user that we could not find a usable WinSock DLL
        printf("Server: The dll do not support the Winsock version %u.%u!\n",
                    LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));
        // Do the clean up
        WSACleanup();
        // and exit with error
        return 1;
    }
    else
    {
        printf("Server: The dll supports the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),
                    HIBYTE(wsaData.wVersion));
        printf("Server: The highest version this dll can support is %u.%u\n",
                    LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
    }

	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (ListeningSocket == INVALID_SOCKET)
    {
        printf("Server: Error at socket(), error code: %ld.\n", WSAGetLastError());
        // Clean up
        WSACleanup();
        // and exit with error
        return 1;
    }
    else
        printf("Server: socket() is OK!\n");

	// Set up a SOCKADDR_IN structure 
	// Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using port 7171.
    // The IPv4 family
    ServerAddr.sin_family = AF_INET;
    // host-to-network byte order
    ServerAddr.sin_port = htons(Port);
    // Listen on all interface, host-to-network byte order
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associate the address information with the socket using bind.
	// Associate the address information with the socket using bind.
    // Call the bind function, passing the created socket and the sockaddr_in
    // structure as parameters. Check for general errors.
	if (bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
        printf("Server: bind() failed! Error code: %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(ListeningSocket);
        // Do the clean up
        WSACleanup();
        // and exit with error
        return 1;
	}
	else
        printf("Server: bind() is OK!\n");

	// Listen for client connections. We used a backlog of 5, which
	// is normal for many applications.
	// Listen for client connections with a backlog of 5
	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
        printf("Server: listen(): Error listening on socket %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(ListeningSocket);
        // Do the clean up
        WSACleanup();
        // Exit with error
        return 1;
	}
    else
        printf("Server: listen() is OK, I'm listening for connections...\n");

    // Set 10 seconds 10 useconds timeout
    SelectTiming = recvTimeOutTCP(ListeningSocket, 10000, 10000);
 
    switch (SelectTiming)
    {
        case 0:
           // Timed out, do whatever you want to handle this situation
           printf("\nServer: Timeout lor while waiting you retard client!...\n");
           break;
        case -1:
           // Error occurred, more tweaking here and the recvTimeOutTCP()...
           printf("\nServer: Some error encountered with code number: %ld\n", WSAGetLastError());
           break;
        default:
        {
           // Accept a new connection when available. 'while' always true
           while(1)
           {
			   // Reset the NewConnection socket to SOCKET_ERROR
               // Take note that the NewConnection socket in not listening
               NewConnection = SOCKET_ERROR;
			   // While the NewConnection socket equal to SOCKET_ERROR
               // which is always true in this case...
			   while (NewConnection == SOCKET_ERROR)
               {
                  // Accept a new connection when one arrives
				  // Accept connection on the ListeningSocket socket and assign
                  // it to the NewConnection socket, let the ListeningSocket
                  // do the listening for more connection
	              NewConnection = accept(ListeningSocket, NULL, NULL);
                  printf("\nServer: accept() is OK...\n");
                  printf("Server: New client got connected, ready to receive and send data...\n");
                  
	              // At this point you can do two things with these sockets
                  // 1. Wait for more connections by calling accept again
                  //    on ListeningSocket (loop)
                  // 2. Start sending or receiving data on NewConnection.

				  
			      memset(&recvbuff, 0, sizeof(recvbuff));
                  ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);
			    
                  if (ByteReceived > 0)
				  {
                      printf("Server: recv() looks fine....\n");

                      // Some info on the receiver side...
                      getsockname(ListeningSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
                      printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
                      printf("Server: Receiving port used: %d\n", htons(ServerAddr.sin_port));
 
                      // Some info on the sender side
                      // Allocate the required resources
                      memset(&SenderInfo, 0, sizeof(SenderInfo));
                      nlen = sizeof(SenderInfo);
 
                      getpeername(NewConnection, (SOCKADDR *)&SenderInfo, &nlen);
                      printf("Server: Sending IP used: %s\n", inet_ntoa(SenderInfo.sin_addr));
                      printf("Server: Sending port used: %d\n", htons(SenderInfo.sin_port));
 
                      // Print the received bytes. Take note that this is the total
                      // byte received, it is not the size of the declared buffer
                      printf("Server: Bytes received: %d\n", ByteReceived);
                      // Print what those bytes represent
                      printf("Server: Those bytes are: \"");
                      // Print the string only, discard other
                      // remaining 'rubbish' in the 1024 buffer size
                      for(i=0;i < ByteReceived;i++)
                         printf("%c", recvbuff[i]);
                      printf("\"");
				  }
                  // No data
				  else if (ByteReceived == 0)
                      printf("Server: Connection closed!\n");
                  // Others
				  else
                      printf("Server: recv() failed with error code: %d\n", WSAGetLastError());
			   }
               
               // Clean up all the send/recv communication, get ready for new one
               if (shutdown(NewConnection, SD_SEND) != 0)
                   printf("\nServer: Well, there is something wrong with the shutdown(). The error code: %ld\n", WSAGetLastError());
               else
                   printf("\nServer: shutdown() looks OK...\n");
 
               // Well, if there is no more connection in 15 seconds,
               // just exit this listening loop...
               if (recvTimeOutTCP(ListeningSocket, 10000, 10000) == 0)
                   break;
		   }
		}
	}

	printf("\nServer: The listening socket is timeout...\n");
    // When all the data communication and listening finished, close the socket
    if(closesocket(ListeningSocket) != 0)
        printf("Server: Cannot close \"ListeningSocket\" socket. Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: Closing \"ListeningSocket\" socket...\n");
 
    // Finally and optionally, clean up all those WSA setup
    if(WSACleanup() != 0)
        printf("Server: WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: WSACleanup() is OK...\n");
 
    return 0;
}