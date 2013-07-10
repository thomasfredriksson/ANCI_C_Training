
#include "winsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")

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

int recvfromTimeOutUDP(SOCKET socket, long sec, long usec)
{
    // Setup timeval variable
    struct timeval timeout;
    struct fd_set fds;

    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    // Setup fd_set structure
    FD_ZERO(&fds);
    FD_SET(socket, &fds);
    // Return value:
    // -1: error occurred
    // 0: timed out
    // > 0: data ready to be read
    return select(0, &fds, 0, 0, &timeout);
}

void UdpReceiver()
{
    WSADATA            wsaData;
    SOCKET             ReceivingSocket;
    SOCKADDR_IN        ReceiverAddr;
    int                Port = 5150;
    char               ReceiveBuf[1024];
    int                BufLength = 1024;
    SOCKADDR_IN        SenderAddr;
    int                SenderAddrSize = sizeof(SenderAddr);
    int                ByteReceived = 5, SelectTiming, ErrorCode;
    char               ch = 'Y';
 
    // Initialize Winsock version 2.2
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
       printf("Server: WSAStartup failed with error %ld\n", WSAGetLastError());
       return -1;
    }
    else
       printf("Server: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);   

	// Create a new socket to receive datagrams on.
    ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 
    if (ReceivingSocket == INVALID_SOCKET)
    {
       printf("Server: Error at socket(): %ld\n", WSAGetLastError());
       // Clean up
       WSACleanup();
       // Exit with error
       return -1;
    }
    else
       printf("Server: socket() is OK!\n");
    
    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to receive datagrams from all interfaces using port 5150.
 
    // The IPv4 family
    ReceiverAddr.sin_family = AF_INET;
    // Port no. 5150
    ReceiverAddr.sin_port = htons(Port);
    // From all interface (0.0.0.0)
    ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
    // Associate the address information with the socket using bind.
    // At this point you can receive datagrams on your bound socket.
    if (bind(ReceivingSocket, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
    {
        printf("Server: bind() failed! Error: %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(ReceivingSocket);
        // Do the clean up
        WSACleanup();
        // and exit with error
        return -1;
    }
    else
        printf("Server: bind() is OK!\n");   
    
    // Some info on the receiver side...
    getsockname(ReceivingSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));
 
    printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
    printf("Server: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));
    printf("Server: I\'m ready to receive a datagram...\n");
 
    SelectTiming = recvfromTimeOutUDP(ReceivingSocket, 1000, 1000);

	switch (SelectTiming)
    {
       case 0:
         // Timed out, do whatever you want to handle this situation
         printf("Server: Timeout lor while waiting you bastard client!...\n");
         break;
       case -1:
         // Error occurred, maybe we should display an error message?
         // Need more tweaking here and the recvfromTimeOutUDP()...
         printf("Server: Some error encountered with code number: %ld\n", WSAGetLastError());
         break;
       default:
       {
         while (1)
         {
            // Call recvfrom() to get it then display the received data...
            ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, BufLength, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
            if (ByteReceived > 0)
            {
                printf("Server: Total Bytes received: %d\n", ByteReceived);
                printf("Server: The data is \"%s\"\n", ReceiveBuf);
            }
            else if ( ByteReceived <= 0 )
                printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
            else
                printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());

            // Some info on the sender side
            getpeername(ReceivingSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
            printf("Server: Sending IP used: %s\n", inet_ntoa(SenderAddr.sin_addr));
            printf("Server: Sending port used: %d\n", htons(SenderAddr.sin_port));
         }
       }
    }

    // When your application is finished receiving datagrams close the socket.
    printf("Server: Finished receiving. Closing the listening socket...\n");
    if (closesocket(ReceivingSocket) != 0)
        printf("Server: closesocket() failed! Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: closesocket() is OK...\n");
 
    // When your application is finished call WSACleanup.
    printf("Server: Cleaning up...\n");
    if(WSACleanup() != 0)
        printf("Server: WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: WSACleanup() is OK\n");
    // Back to the system
    return 0;
}

// Print out the contents of a WSAPROTOCOL_INFO structure in
// a readable format.
void PrintBufContent(WSAPROTOCOL_INFO *pProtocolBuf)
{
      unsigned char *pszUuid;
      int i;
     
      printf("\nProtocol <%s>\n", pProtocolBuf->szProtocol);
      // A guid is the same as a uuid.
      UuidToString(&pProtocolBuf->ProviderId, (RPC_WSTR *)&pszUuid);
      printf("  ProviderId {%s}\n", pszUuid);
      RpcStringFree((RPC_WSTR *)&pszUuid);
     
      if (!pProtocolBuf->dwServiceFlags1)
            printf("  dwServiceFlags1: 0\n");
      else
            printf("  dwServiceFlags1: 0x%08X\n",pProtocolBuf->dwServiceFlags1);
     
      // Check which bit flags are set.
      printf("\nChecking the set bit flags...\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_CONNECTIONLESS)
            printf("    XP1_CONNECTIONLESS\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_GUARANTEED_DELIVERY)
            printf("    XP1_GUARANTEED_DELIVERY\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_GUARANTEED_ORDER)
            printf("    XP1_GUARANTEED_ORDER\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_MESSAGE_ORIENTED)
            printf("    XP1_MESSAGE_ORIENTED\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_PSEUDO_STREAM)
            printf("    XP1_PSEUDO_STREAM\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_GRACEFUL_CLOSE)
            printf("    XP1_GRACEFUL_CLOSE\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_EXPEDITED_DATA)
            printf("    XP1_EXPEDITED_DATA\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_CONNECT_DATA)
            printf("    XP1_CONNECT_DATA\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_DISCONNECT_DATA)
            printf("    XP1_DISCONNECT_DATA\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_SUPPORT_BROADCAST)
            printf("    XP1_SUPPORT_BROADCAST\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_SUPPORT_MULTIPOINT)
            printf("    XP1_SUPPORT_MULTIPOINT\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_MULTIPOINT_CONTROL_PLANE)
            printf("    XP1_MULTIPOINT_CONTROL_PLANE\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_MULTIPOINT_DATA_PLANE)
            printf("    XP1_MULTIPOINT_DATA_PLANE\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_QOS_SUPPORTED)
            printf("    XP1_QOS_SUPPORTED\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_INTERRUPT)
            printf("    XP1_INTERRUPT\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_UNI_SEND)
            printf("    XP1_UNI_SEND\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_UNI_RECV)
            printf("    XP1_UNI_RECV\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_IFS_HANDLES)
            printf("    XP1_IFS_HANDLES\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_PARTIAL_MESSAGE)
            printf("    XP1_PARTIAL_MESSAGE\n");
 
      printf("  dwServiceFlags2: reserved\n");
      printf("  dwServiceFlags3: reserved\n");
      printf("  dwServiceFlags4: reserved\n");
 
      printf("\nChecking the Provider flags...\n");
      printf("  dwProviderFlags:\n");
      if (pProtocolBuf->dwProviderFlags & PFL_MULTIPLE_PROTO_ENTRIES)
            printf("    PFL_MULTIPLE_PROTO_ENTRIES\n");
      if (pProtocolBuf->dwProviderFlags & PFL_RECOMMENDED_PROTO_ENTRY)
            printf("    PFL_RECOMMENDED_PROTO_ENTRY\n");
      if (pProtocolBuf->dwProviderFlags & PFL_HIDDEN)
            printf("    PFL_HIDDEN\n");
      if (pProtocolBuf->dwProviderFlags & PFL_MATCHES_PROTOCOL_ZERO)
            printf("    PFL_MATCHES_PROTOCOL_ZERO\n");
 
      printf("\nMore digging...\n");
      printf("  dwCatalogEntryId = %u\n", pProtocolBuf->dwCatalogEntryId);
 
      printf("  ProtocolChain.ChainLen = %d ",  pProtocolBuf->ProtocolChain.ChainLen);
      if (pProtocolBuf->ProtocolChain.ChainLen == 1)
            printf("  (This is a base service provider)\n");
      else if (pProtocolBuf->ProtocolChain.ChainLen > 1)
      {
            printf("  (ProtocolChain layered to base protocol)\n");
            for (i=0; i<pProtocolBuf->ProtocolChain.ChainLen; i++)
            {
                  printf("    Chain Catalog Entry Id = %u\n", pProtocolBuf->ProtocolChain.ChainEntries[i]);
            }
      }
      else if (pProtocolBuf->ProtocolChain.ChainLen == 0)
            printf("  (This is a layered service provider)\n");
      else
            printf("  Invalid\n");
 
      printf("  iVersion = %d\n", pProtocolBuf->iVersion);
      printf("  iAddressFamily = %d\n", pProtocolBuf->iAddressFamily);
      printf("  iMaxSockAddr = %d\n", pProtocolBuf->iMaxSockAddr);
      printf("  iMinSockAddr = %d\n", pProtocolBuf->iMinSockAddr);
 
      // iProtocols returns a negative number for Microsoft NetBIOS
      // service providers corresponding to the lana number * -1 (for
      // example, -2 implies lana 2), except for lana 0 which is equal to
      // 0x80000000 because protocol 0 is reserved for special use.
      printf("  iProtocol = %d\n", pProtocolBuf->iProtocol);
      printf("  iProtocolMaxOffset = %d\n", pProtocolBuf->iProtocolMaxOffset);
      printf("  iNetworkByteOrder = %s\n", ((pProtocolBuf->iNetworkByteOrder == LITTLEENDIAN) ?
            "LittleEndian" : "BigEndian"));
      printf("  iSecurityScheme = %d\n", pProtocolBuf->iSecurityScheme);
      printf("  dwMessageSize = %u\n", pProtocolBuf->dwMessageSize);
      printf("  dwProviderReserved = reserved\n");
      return;
}

void EnumInstalledProtocols()
{
      WSADATA WSAData;
      int i, nRet;
      DWORD dwErr;
      WSAPROTOCOL_INFO *lpProtocolBuf = NULL;
      DWORD dwBufLen = 0;
 
      // The WSAStartup() function directly returns the extended error code
      // in the return value for this function. A call to the WSAGetLastError
      // function is not needed and should not be used
      if (WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
            printf("WSAStartup() error with code %d\n", WSAGetLastError());
      else
      {
            printf("WSAStartup() is OK!\n");
            // First, have WSAEnumProtocols tell you how big a buffer you need.
			/* The WSAEnumProtocols function retrieves information 
			 * about available transport protocols. */
            nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
           
            if (nRet != SOCKET_ERROR)
                  printf("WSAEnumProtocols() looks OK!\n");
            else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
                  // WSAEnumProtocols() failed for some reason not relating to buffer size
                  printf("WSAEnumProtocols() failed with code %d\n", WSAGetLastError());
            else
            {
                  // WSAEnumProtocols failed for the "expected" reason.
                  // Now you need to allocate a buffer that is the right size.
                  lpProtocolBuf = (WSAPROTOCOL_INFO *)malloc(dwBufLen);
                  if (lpProtocolBuf)
                  {
                        // Now you can call WSAEnumProtocols again with the
                        // expectation that it will succeed
                        // because you have allocated a big enough buffer.
                        nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
 
                        if (nRet == SOCKET_ERROR)
                              printf("WSAEnumProtocols() failed with code %d\n", WSAGetLastError());
                        else
                        {
                              // Enumerate the protocols.
                              printf("Enumerating the protocols...");
                              for (i=0; i<nRet; i++)
                              {
                                    PrintBufContent(&lpProtocolBuf[i]);
                                    printf("\nPress <Enter> for more info, if any...\n");
                                    _fgetchar();
                              }
                        }
                        // Free up the allocated resource
                        free(lpProtocolBuf);
                  }
            }
      }
      if(WSACleanup() != 0)
            printf("WSACleanup() failed!...\n");
      else
            printf("WSACleanup() is OK...\n");
      return 0;
}

void CreateWSASocket()
{
      WSADATA WSAData;
      int i, nRet;
      WSAPROTOCOL_INFO *lpProtocolBuf = NULL;
      DWORD dwBufLen = 0;
      SOCKET SocDescriptor;
      DWORD dwErr;
      BOOL bProtocolFound;
 
      if (WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
            printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
      else
      {
            printf("WSAStartup() seems OK!\n");
 
            // First, have WSAEnumProtocols() tell you how big a buffer you need
            nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
 
            printf("The buffer size is %d bytes...\n", dwBufLen);
 
            // If failed, returns SOCKET_ERROR
            if (nRet != SOCKET_ERROR)
                  printf("WSAEnumProtocols() does not return SOCKET_ERROR!\n");
            // WSAENOBUFS - The buffer length was too small
            else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
                  // WSAEnumProtocols() failed for some reason not relating to buffer size
                  printf("WSAEnumProtocols() does not return WSAENOBUFS,\n"
                  "    however failed with error code %ld\n", WSAGetLastError());
            else
            {
                  // WSAEnumProtocols() failed for the "expected" reason. Therefore,
                  // you need to allocate a buffer of the appropriate size.
                  printf("Allocating the buffer of the appropriate size...\n");
                  lpProtocolBuf = (WSAPROTOCOL_INFO *)malloc(dwBufLen);
                  printf("Recheck, the buffer size is %d bytes...\n", dwBufLen);
                  if (lpProtocolBuf)
                  {
                        // Now you can call WSAEnumProtocols() again with the expectation
                        // that it will succeed because you have allocated a big enough buffer.
                        nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
                        if (nRet == SOCKET_ERROR)
                              printf("WSAEnumProtocols() failed with error code %d\n", WSAGetLastError());
                        else
                        {
                              printf("WSAEnumProtocols() looks pretty fine!\n");
                              // Loop through protocols, looking for the first service
                              // provider that meets the matching criteria.
                              // Originally FALSE, if found change to TRUE
                              printf("Matching the specified service provider...\n");
                              printf("    IPPROTO_TCP and XP1_QOS_SUPPORTED...\n");
                              bProtocolFound = FALSE;
                              for (i=0; i<nRet; i++)
                              {
                                    if ((lpProtocolBuf[i].iProtocol == IPPROTO_TCP) &&
                                          ((XP1_QOS_SUPPORTED & lpProtocolBuf[i].dwServiceFlags1) ==
                                                    XP1_QOS_SUPPORTED))
                                    {
                                          bProtocolFound = TRUE;
                                          // first service provider matched, exit the loop
                                          break;
                                    }
                              }
                              printf("The specified service provider found/matched! buffer index, i is %d...\n", i);
 
                              // Once you have chosen a service provider, the following sample code
                              // demonstrates how to actually create a socket using a supplied
                              // WSAPROTOCOL_INFO structure
                              // from the service provider you have chosen
 
							  /* The WSASocket function creates a socket 
							   * that is bound to a specific transport-service provider. 
							   *
                               * The  following  may  be used in place of the address family, socket type, or
                               * protocol  in  a  call  to WSASocket to indicate that the corresponding value
                               * should  be taken from the supplied WSAPROTOCOL_INFO structure instead of the
                               * parameter itself.
                               */
                              SocDescriptor = WSASocket(
                                    FROM_PROTOCOL_INFO, // manifest constant
								    FROM_PROTOCOL_INFO, // manifest constant
                                    FROM_PROTOCOL_INFO, // manifest constant
                                    &lpProtocolBuf[i], // from WSAPROTOCOL_INFO
                                    0, // reserved
                                    WSA_FLAG_OVERLAPPED);
 
                              printf("Reconfirm the buffer index, i is %d\n", i);
                              if (SocDescriptor == INVALID_SOCKET)
                              {
                                    printf("WSASocket() failed with error code %d\n", WSAGetLastError());
                                    WSACleanup();
                              }
                              else
                                    printf("WSASocket() looks fine, continue on next tasks!\n");
                        }
                  }
            }
      }
      // If not needed anymore, deallocate memory to the system's heap
      free(lpProtocolBuf);
      // WSA cleanup
      if(WSACleanup() != 0)
            printf("WSACleanup() failed!...\n");
      else
            printf("WSACleanup() is OK...\n");
      return 0;
}

void TestNameResolutionFunctions()
{
      WSADATA                   wsaData;
	  SOCKET                    s;
	  int                       rc, iResult, RetCode;
      struct addrinfo*          result = NULL;
      struct addrinfo           hints;

	  // Initialize Winsock
	  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	  if (iResult != 0)
	  {
		  printf("Client: WSAStartup() failed with error code %d\n", iResult);
		  return 1;
	  }
	  else
		  printf("Client: WSAStartup() is OK...\n");

	  // Setup the hints address info structure
	  // which is passed to the getaddrinfo() function
	  ZeroMemory( &hints, sizeof(hints) );

	  hints.ai_flags = AI_CANONNAME;
	  hints.ai_family = AF_INET;
	  hints.ai_socktype = SOCK_STREAM;
	  hints.ai_protocol = IPPROTO_TCP;

	  // rc = getaddrinfo("206.190.60.37", "7171", &hints, &result);
	  rc = getaddrinfo("www.ipv6tf.org", "80", &hints, &result);

	  if (rc != 0)
	  {
		  printf("Client: getaddrinfo() failed with error code %ld\n", WSAGetLastError());
		  WSACleanup();
		  return 1;
	  }

	  printf("Client: getaddrinfo() looks OK!\n");

	  // Create a new socket to make a client connection.
	  s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	  if (s == INVALID_SOCKET)
	  {
		  printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
		  // Do the clean up
		  WSACleanup();
		  // Exit with error
		  return 1;
	  }
	  else
		  printf("Client: socket() is OK!\n");

	  // Make a connection to the server with socket SendingSocket.
	  RetCode = connect(s, result->ai_addr, result->ai_addrlen);
	  if(RetCode != 0)
	  {
	      printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
		  // Close the socket
		  closesocket(s);
		  // Do the clean up
		  WSACleanup();
		  // Exit with error
		  return 1;
      }
	  else
	  {
		  printf("Client: connect() is OK, got connected...\n");
		  printf("Client: Ready for sending and/or receiving data...\n");
	  }

      // Return the allocated resource to the heap
	  freeaddrinfo(result);
	  // When your application is finished handling the connection, call WSACleanup.
	  if(WSACleanup() != 0)
		  printf("Client: WSACleanup() failed!...\n");
	  else
		  printf("Client: WSACleanup() is OK...\n");

      return 0;      
}

int AI_NUMERICHOST_test(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    DWORD dwRetval;
    int i = 1;   
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
 
    // Validate the parameters
    if (argc != 2) {
        printf("Usage: %s <IP Address String>\n", argv[0]);
        printf("  getaddrinfo() determines the IP binary network address\n");
        printf("Example: %s 206.190.60.37\n", argv[0]);
        return 1;
    }
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", iResult);
        return 1;
    }
 
    printf("WSAStartup() looks OK!\n");
    // Setup the hints address info structure which is passed to the getaddrinfo() function
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_flags = AI_NUMERICHOST;
    hints.ai_family = AF_UNSPEC;
 
    // Call getaddrinfo(). If the call succeeds,
    // the result variable will hold a linked list
    // of addrinfo structures containing response information
    dwRetval = getaddrinfo(argv[1], NULL, &hints, &result);
    if ( dwRetval != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", dwRetval);
        WSACleanup();
        return 1;
    }
 
    printf("getaddrinfo() returned success!\n");
    // Retrieve each address and print out the hex bytes
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
    {
        printf("getaddrinfo() response %d\n", i++);
        printf("\tFlags: 0x%x\n", ptr->ai_flags);
 
        printf("\tFamily: ");
        switch (ptr->ai_family)
        {
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n");
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_family);
                break;
        }
 
        printf("\tSocket type: ");
        switch (ptr->ai_socktype)
        {
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol)
        {
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);
    }
    // Release the allocated resource
    freeaddrinfo(result);
    // WSA clean up
    WSACleanup();
    return 0;
}


int AI_NUMERICHOST_test1(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
 
    DWORD dwRetval;
 
    struct sockaddr_in saGNI;
    char hostname[NI_MAXHOST]; // Set to the max value
    char servInfo[NI_MAXSERV]; // Set to the max value
    // Assume we plan to use TCP port 7777
    u_short port = 7777;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <IPv4 address>\n", argv[0]);
        printf("  This program return hostname\n");
        printf("Example: %s 127.0.0.1\n", argv[0]);
        return 1;
    }
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", iResult);
        return 1;
    }
 
    printf("WSAStartup() looks OK...\n");
    // Set up sockaddr_in structure which is passed to the getnameinfo function
    saGNI.sin_family = AF_INET; // AF_INET6
    saGNI.sin_addr.s_addr = inet_addr(argv[1]);
    saGNI.sin_port = htons(port);
 
    // Call getnameinfo()
    dwRetval = getnameinfo((struct sockaddr *) &saGNI,
                           sizeof (struct sockaddr),
                           hostname,
                           NI_MAXHOST, servInfo,
                           NI_MAXSERV, NI_NUMERICSERV);
 
    if (dwRetval != 0)
    {
        printf("getnameinfo() failed with error code %ld\n", WSAGetLastError());   
    }
    else
    {
        printf("getnameinfo() returned hostname \"%s\"\n", hostname);
    }
 
    // Terminate the use of Winsock
    WSACleanup();
    if (dwRetval != 0)
        return 1;
    else
        return 0; 
}

int AI_NUMERICHOST_test2(int argc, char **argv)
{
    WSADATA                wsaData;
    int                    iResult, RetVal, RetVal2;
    // SOCKADDR_IN  MyAddr;
    SOCKADDR_IN6           MyAddrValue;
    // Storage buffer, using LPWSTR pointer failed lor!
    // WCHAR  MyAddrString[256] = L"For IPv4...";
    WCHAR  MyAddrString[256] = L"Some dummy initializer";
    // Take note that if the address is in FQDN/Name/hostname
    // we need to convert it to a dotted IP address. For example:
    // WCHAR IPAddr[256] = L"localhost"; // this should fail lor!
    // WCHAR IPAddr[256] = L"209.131.36.158";
    WCHAR IPAddr[256] = L"2a01:48:1:0:2e0:81ff:fe05:4658";
 
    // These buffer sizes must be allocated properly else
    // the WSAStringToAddress() and WSAAddressToString() will
    // fail miserably!
    // int MyAddrSize = sizeof(struct sockaddr_storage);
    int MyAddrSize = sizeof(struct sockaddr_storage);
    // DWORD MyAddrSize2 = sizeof(struct sockaddr_storage);
    DWORD MyAddrSize2 = sizeof(struct sockaddr_storage);
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        // Exit with error
        return 1;
    }
    else
        printf("WSAStartup() is OK...\n");
 
    // Initialize the address family
    // MyAddrValue.sin_family = AF_INET;
    MyAddrValue.sin6_family = AF_INET6;
 
    // This part should convert the server/hostname/FQDN to a standard dotted IP address
    // string first (if any) using other helper functions such as getaddressinfo()
 
    //  ==============Convert address string to value using WSAStringToAddress()==============
    printf("\nWSAStringToAddress() - string to address value\n");
    RetVal = WSAStringToAddress(
                        IPAddr,                        // Pointer to address string to be converted
                        AF_INET6 /* AF_INET*/,         // address family
                        NULL,                          // Protocol info structure, not useable here
                        (LPSOCKADDR)&MyAddrValue,      // Socket address string buffer [out]
                        &MyAddrSize                    // Length of socket structure
                        );
 
    // Something wrong
    if (RetVal != 0)
    {
        printf("WSAStringToAddress() failed with error code %ld\n", WSAGetLastError());
        // Clean-up
        WSACleanup();
        // Exit with error
        return 1;
    }
    else
        printf("WSAStringToAddress() looks fine! Return value is %ld\n", RetVal);

    // printf("The IP Address part of the buffer filled with %ul\n", MyAddr.sin_addr);
    printf("The IP address value is %ul\n", MyAddrValue.sin6_addr);
    // and other info
    printf("The address family value is %ul\n", MyAddrValue.sin6_family);
    printf("The port value is %ul\n", MyAddrValue.sin6_port);
    printf("The scopeID is %ul\n", MyAddrValue.sin6_scope_id);

    // =========Do the re-conversion using WSAAddressToString()===============
    printf("\nWSAAddressToString() - address value to string\n");
    RetVal2 = WSAAddressToString(
                (LPSOCKADDR)&MyAddrValue,  // Pointer to address value to be converted
                MyAddrSize,              // Length of pointer to address value, in bytes
                NULL,                         // A pointer to the WSAPROTOCOL_INFO structure for a particular provider
                MyAddrString,           // A pointer to the buffer that receives the human-readable address string
                &MyAddrSize2          // length of the buffer pointed to by the MyAddrString parameter
                );

    // Something wrong
    if( RetVal2 != 0)
    {
           printf("WSAAddressToString() failed with error code %ld\n", WSAGetLastError());
           // Clean-up
           WSACleanup();
           // Exit with error
           return 1;
    }
    else
           printf("WSAAddressToString() looks fine! Return value is %ld\n", RetVal2);

    // printf("The IP Address is %ul\n", MyAddr.sin_addr);
    printf("The address string is %S\n", MyAddrString);
    // and other info
    printf("The address family is %ul\n", MyAddrValue.sin6_family);
    printf("The port value is %ul\n", MyAddrValue.sin6_port);
    printf("The scopeID is %ul\n", MyAddrValue.sin6_scope_id);

    // Call WSACleanup for WSA* cleanup
    if(WSACleanup() != 0)
        printf("\nWSACleanup() failed!...\n");
    else
        printf("\nWSACleanup() is OK...\n");

    return 0;   
}

int AI_NUMERICHOST_test3(int argc, char **argv)
{
    WSADATA          wsaData;
    SOCKADDR_IN      addr;
    char             AddrValue[256] = "216.239.61.104";
    char             AddrString[256]  = "Some dummy value";
    DWORD            dwSizeOfStr = sizeof(AddrValue);
    int              nSizeOfInput = sizeof(AddrString), iResult;
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        // Exit with error
        return 1;
    }
    else
        printf("WSAStartup() is OK...\n");
        // Initialize address family
        addr.sin_family = AF_INET;
 
        if(WSAStringToAddressA(AddrValue, AF_INET, NULL, (LPSOCKADDR)&addr, &nSizeOfInput) != 0)
        {
             printf("\nWSAStringToAddressA failed with error num %ld\n", WSAGetLastError());
             WSACleanup();
             return 1;
        }
        else
        {
             printf("\nAddress in value = %ul\n", addr.sin_addr);
        }

        if(WSAAddressToStringA((LPSOCKADDR)&addr, sizeof(addr), NULL, AddrString, &dwSizeOfStr) != 0)
        {
             printf("WSAAddressToStringA() failed miserably with error code %ld\n", WSAGetLastError());
             WSACleanup();
             return 1;
        }
        else
             printf("Address string = %s\n", AddrString);

        // Do the WSACleanup...
        if(WSACleanup() != 0)
             printf("\nWSACleanup() failed!...\n");
        else
             printf("\nWSACleanup() is OK...\n");
        return 0;
}

int AI_NUMERICHOST_test4(int argc, char **argv)
{
    WSADATA wsaData;
 
    struct addrinfo hints, *res, *p;
    int status;
    // Provide big enough buffer, ipv6 should be the biggest
    char ipstr[INET6_ADDRSTRLEN];
    char ipstr2[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        printf("Usage: %s <hostname>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        return 1;
    }

    // Initialization
    if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0)
    {
        printf("WSAStartup() failed miserably! With error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
         printf("getaddrinfo() failed lor! with error code %ld\n", WSAGetLastError());
         WSACleanup();
         return 1;
    }

    printf("The IP addresses for %s:\n", argv[1]);

    for(p = res;p != NULL; p = p->ai_next)
    {
        void *addr;
        char *ipver;

        // Get the pointer to the address itself, different fields in IPv4 and IPv6
        if (p->ai_family == AF_INET)
        {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convert the IP to a string and print it
        printf("String format: ");
        // InetNtop(address_family, IP_address_in_network_byte_to_convert_to_a_string,
        //         buffer_to_store_the_IP_address_string, the_IP_string_length_in_character);
		/* The InetNtop function converts an IPv4 or IPv6 
		 * Internet network address into a string in Internet standard format. 
		 * The ANSI version of this function is inet_ntop. */
        inet_ntop(p->ai_family, addr, (PSTR)ipstr, sizeof(ipstr));
        printf(" %s: %s\n", ipver, ipstr);

        printf("Value format: ");
        // InetPton(address_family, string_to_be_converted, buffer_to_store_the_converted_string);
        //inet_pton(p->ai_family, (PCSTR)ipstr, ipstr2);
        printf(" %s: %ul\n", ipver, ipstr2);
    }

    // Deallocate the resource...
    freeaddrinfo(res);
    // Cleanup the WSA...
    WSACleanup();
    return 0;  
}

int AI_NUMERICHOST_test5(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    unsigned long ulAddr = INADDR_NONE;
    ULONG TempBuff = 0;
    struct sockaddr_in saSample;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <IPv4 address>\n", argv[0]);
        printf("  inet_addr() converts a string containing an\n");
        printf("  IPv4 address in one of the supported formats\n");
        printf("  to a unsigned long representing an IN_ADDR\n");
                        printf("Example: %s 209.131.36.158\n", argv[0]);
        return 1;
    }
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
 
    // Just for demonstration, call inet_addr(). If the call
    // succeeds, the result variable will hold a IN_ADDR structure
    ulAddr = inet_addr(argv[1]);
 
    if (ulAddr == INADDR_NONE)
    {
        printf("inet_addr() failed and returned INADDR_NONE\n");
        WSACleanup();
        return 1;
    }
 
    if (ulAddr == INADDR_ANY) 
	{
        printf("inet_addr() failed and returned INADDR_ANY\n");
        WSACleanup();
        return 1; 
    }
 
    printf("inet_addr() returned success!\n");
    // Retrieve address and print out the hex/dec/octal bytes
    printf("  Hex:\t\t0X%0X\n", ulAddr);
    printf("  Decimal:\t%ul\n", ulAddr);
    printf("  Octal:\t0%o\n", ulAddr);

    // Then, ready to be used
    saSample.sin_family = AF_INET;
    // In typical app. we can be directly use:
    // usesaSample.sin_addr.s_addr = inet_addr(argv[1]);
    saSample.sin_addr.s_addr = ulAddr;
    saSample.sin_port = htons(1234);

    // More codes here, create socket, bind, connect etc.

    // Revert the process using inet_ntoa() just for the address part

	/* The inet_ntoa function converts an (Ipv4) 
	 * Internet network address into an ASCII string 
	 * in Internet standard dotted-decimal format.
	 */
    if(inet_ntoa(saSample.sin_addr) != NULL)
    {
        printf("inet_ntoa() return success!\n");
        printf("The IP address is %s\n", inet_ntoa(saSample.sin_addr));
    }
    else
        printf("inet_ntoa() failed with error code %ld\n", WSAGetLastError());
 
    WSACleanup();
    return 0;
}

int AI_NUMERICHOST_test6(int argc, char **argv)
{
     // Declare and initialize variables
    WSADATA wsaData;
    DWORD dwError;
    int i = 0;
    struct hostent *remoteHost;
    char *host_name;
    struct in_addr addr;
    char **pAlias;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <ipv4address> or <hostname>\n", argv[0]);
        printf("Example: %s 127.0.0.1\n", argv[0]);
        printf("  to return the host\n");
        printf("Example: %s www.palerbutuh.com\n", argv[0]);
        printf("  to return the IP addresses for a host\n");
        return 1;
    }
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");
 
    host_name = argv[1];
    // If the user input is an alpha name for the host, use gethostbyname()
    // If not, gethostbyaddr()
    // We are assuming the IPv4 here
    if (isalpha(host_name[0]))    // host address is a name instead og IP
    {
        printf("Calling gethostbyname() with %s\n", host_name);
        remoteHost = gethostbyname(host_name);
    }
    else
    {
        // host address is an IP
        printf("Calling gethostbyaddr() with %s\n", host_name);
        addr.s_addr = inet_addr(host_name);
        if (addr.s_addr == INADDR_NONE)
        {
            printf("The IPv4 address entered must be a legal address!\n");
            WSACleanup();
            return 1;
        }
        else
        {
            // gethostbyaddr() is OK
            // Should provide both the Ipv6 and IPv4. Just IPv4 shown here
            remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
        }
    }
 
    if (remoteHost == NULL)
    {
        dwError = WSAGetLastError();
        if (dwError != 0)
        {
            if (dwError == WSAHOST_NOT_FOUND)
            {
                        printf("Host not found!\n");
                        WSACleanup();
                        return 1;
            }
            else if (dwError == WSANO_DATA)
            {
                        printf("No data record found!\n");
                        WSACleanup();
                        return 1;
            }
            else
            {
                    printf("Function failed with error code %ld\n", dwError);
                    WSACleanup();
                    return 1;
            }
         }
    }
    else
    {
        printf("Function returned successfully!\n");
        printf("\tOfficial name: %s\n", remoteHost->h_name);
        // Extract all the cname/aliases if any
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++)
        {
            printf("\tAlternate name (if any) #%d: %s\n", ++i, *pAlias);
        }
 
        // Check the address family type
        printf("\tAddress type: ");
        switch (remoteHost->h_addrtype)
        {
        case AF_INET:
                    printf("AF_INET family\n");
                    break;
        case AF_INET6:
                    printf("AF_INET6 family\n");
                    break;
        case AF_NETBIOS:
                    printf("AF_NETBIOS - Windows NetBIOS\n");
                    break;
        default:
                    printf(" %d\n", remoteHost->h_addrtype);
                    break;
        }
        // Check the address family type
        printf("\tAddress length: %d\n", remoteHost->h_length);
        // Traverse the hostent address list and print the IP address(s)
        i = 0; // Reset counter
        while (remoteHost->h_addr_list[i] != 0)
        {
           addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
           printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
        }
    }
    WSACleanup();
    return 0;      
}

int AI_NUMERICHOST_test7(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    char node_name[100] = "";
    struct hostent *hostptr;
    struct servent *servptr;
    // http://www.iana.org/assignments/port-numbers
    char services[12][10] = {"http", "ftp", "ssh", "telnet", "imap4", "smtp", "gopher", "finger", "pop3", "tftp", "mysql"};
    int i;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <host_name>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        // Just return with error
        return 1;
    }
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");
 
    strcpy_s(node_name, sizeof(node_name), argv[1]);
    // Here, name of server computer is expected, better to add
    // the server IP address functionality...
    hostptr = gethostbyname(node_name);

    if (hostptr == NULL)
    {
        printf("gethostbyname() failed with error code %ld\n", WSAGetLastError());
    }
    else
        printf("gethostbyname() is OK.\n Host name is %s\n", hostptr->h_name);

	for(i=0; i <= 10;i++)
	{
		// The proto pointer is NULL, the getservbyname() will returns
		// the first service entry where name matches the s_name member
		// of the servent struct or the s_aliases member of the servent struct.

		/* The getservbyname function retrieves service 
		 * information corresponding to a service name and protocol. */
		servptr = getservbyname(services[i], NULL);
       
		if (servptr == NULL)
			printf ("\ngetservbyname() - %s failed with error code %ld\n", services[i], WSAGetLastError());
		else
		{
			// Print the related info
			printf("\ngetservbyname() is OK - %s\n", services[i]);
			printf(" Service name is %s\n", servptr->s_name);
			// Need to cast to host-byte order
			printf(" Port number is %u\n", ntohs(servptr->s_port));
			printf(" Protocol is %s\n", servptr->s_proto);
		}
	}

	WSACleanup();
    return 0;  
}

int AI_NUMERICHOST_test8(int argc, char **argv)
{ 
    // Declare and initialize variables
    WSADATA wsaData;
    char node_name[100] = "";
    struct hostent *hostptr;
    struct servent *servptr;
    // http://www.iana.org/assignments/port-numbers
    int services[12] = {23, 80, 25, 21, 53, 69, 110, 143, 70, 7, 13};
    int i;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <host_name>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        // Just return with error
        return 1;
    }
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");
 
    strcpy_s(node_name, sizeof(node_name), argv[1]);
    // Here, name of server computer is expected, it is better
    // to add the server IP address functionality...
    hostptr = gethostbyname(node_name);
 
    if (hostptr == NULL)
    {
        printf("gethostbyname() failed with error code %ld\n", WSAGetLastError());
    }
    else
        printf("gethostbyname() is OK.\n Host name is %s\n", hostptr->h_name);
 
    for(i=0; i <= 10;i++)
    {
        // If this is null, getservbyport returns the first service
        // entry for which the port matches the s_port of the servent structure.
		/* The getservbyport function retrieves service 
		 * information corresponding to a port and protocol. */
        servptr = getservbyport(ntohs(services[i]), NULL);

        if (servptr == NULL)
            printf ("\ngetservbyport() - %d failed with error code %ld\n", services[i], WSAGetLastError());
        else
        {
            // Print the related info
            printf("\ngetservbyport() is OK - %d\n", services[i]);
            printf(" Service name is %s\n", servptr->s_name);
            // Need to cast to host-byte order
            printf(" Port number is %u\n", ntohs(servptr->s_port));
            printf(" Protocol is %s\n", servptr->s_proto);
        }
    }
    WSACleanup();
    return 0;  
}

void UniversalServer()
{
	// Declare and initialize variables
    WSADATA            wsaData;
    SOCKET             slisten[16], NewConnection;
    char*              szPort = "7777";
    struct addrinfo    hints, *res=NULL, *ptr=NULL;
    int                count=0, rc;
    char               recvbuff[1024];
    int                ByteReceived, i;
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Server: WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("Server: WSAStartup() looks fine!\n");
 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    rc = getaddrinfo(NULL, szPort, &hints, &res);
    if (rc != 0)
    {
        printf("Server: getaddrinfo() failed with error code %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else
        printf("Server: getaddrinfo() is OK...\n");

    ptr = res;

    while (ptr)
    {
        printf("\nServer: count value = %d\n", count);
        // Use the res struct info for listening...
        slisten[count] = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (slisten[count] == INVALID_SOCKET)
        {
            printf("Server: socket() failed with error code %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        else
            printf("Server: socket() is OK...\n");

        // The res struct info
        printf("\n The address family: %d\n", res->ai_family);
        printf(" The socket type: %d\n", res->ai_socktype);
        printf(" The protocol: %d\n\n", res->ai_protocol);

        // Then bind
        rc = bind(slisten[count], ptr->ai_addr, ptr->ai_addrlen);

        if (rc == SOCKET_ERROR)
        {
            printf("Server: bind() failed with error code %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        else
            printf("Server: bind() is OK...\n");

        // Next, listen
        rc = listen(slisten[count], 10);

        if (rc == SOCKET_ERROR)
        {
            printf("Server: listen() failed with error code %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        else
        {
            printf("Server: listen() is OK...\n");
            NewConnection = SOCKET_ERROR;
            // While the NewConnection socket equal to SOCKET_ERROR
            // which is always true in this case...
            while(NewConnection == SOCKET_ERROR)
            {
                // Accept connection on the slisten[count] socket and assign
                // it to the NewConnection socket, let the slisten[count]
                // do the listening for more connection
                NewConnection = accept(slisten[count], NULL, NULL);
                printf("Server: accept() is OK...\n");
                printf("Server: New client got connected, ready to receive and send data...\n");

                // Wait for more connections by calling accept again on ListeningSocket (loop)
                // or start sending or receiving data on NewConnection.
                ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);

                // When there is problem
                if ( ByteReceived == SOCKET_ERROR )
                {
                    printf("Server: recv() failed with error code %ld\n", WSAGetLastError());
                    WSACleanup();
                    break;
                }
                else
                {
                    printf("Server: recv() is OK....\n");
                    // Print the received bytes. Take note that this is the total
                    // byte received, it is not the size of the declared buffer
                    printf("Server: Bytes received: %d\n", ByteReceived);
                    // Print what those bytes represent
                    printf("Server: Those bytes are: \"");
                    // Print the string only, discard other
                    // remaining 'rubbish' in the 1024 buffer size
                    for(i=0;i < ByteReceived;i++)
                        printf("%c", recvbuff[i]);
                    printf("\"\n");
                }
            }
        }

        if(res->ai_protocol == 6)
        {
            printf("Doing the TCP shutdown on the receiving part...\n");
            shutdown(slisten[count], SD_RECEIVE);
        }

        closesocket(slisten[count]);
        count++;
        ptr = ptr->ai_next;
    }

    freeaddrinfo(res);
    WSACleanup();
    return 0;
}