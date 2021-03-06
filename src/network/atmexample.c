// Description:
//    This is a simple Winsock ATM client/server echo program.
//    The server side requires a local interface to bind to.
//    This can be accomplished with the -l:interface option.
//    For the client side, simply specify the sever address
//    to connect to. The server handles client connections in
//    the same thread as the listening socket. This sample
//    is meant to illustrate how to use the ATM address family
//    and is not how an actual client/server should be implemented
//
// Command Line Parameters/Options
//    -a          Print out local ATM address
//    -s          Act as the server
//    -l <if >    Interface to bind to. Either the index of the
//                interface or an NSAP ATM address
//    -r <addr>   Specify the server's NSAP ATM address (another version is ATM E.164 address)
//    -p <port>   The 2 digit selector byte (hexadecimal port number)
//
#include  <winsock2.h>
#include  <Ws2atm.h>
#include "atmexample.h"
 
#define MAX_ATM_ADDR_LEN        64
#define MAX_BUFFER              1024
#define NSAP_ADDR_LEN           38
#define SELECTOR_BYTE_LEN       2
 
WSAPROTOCOL_INFO  *lpSocketProtocol=NULL;
 
char szServerAddr[39],        // Server's ATM NSAP address
           szPort[3];         // Port number
BOOL bServer = FALSE;
DWORD dwInterface=-1;         // Which interface to bind to?

// Function: BtoH
// Description:
//    This function returns the equivalent binary value for
//    an individual character specified in the ascii format
UCHAR BtoH_atm(CHAR ch)
{
    if (ch >= '0' && ch <= '9')
    {
       return ( ch - '0' );
    }
 
    if (ch >= 'A' && ch <= 'F')
    {
       return ( ch - 'A' + 0xA );
    }
 
    if (ch >= 'a' && ch <= 'f')
    {
       return ( ch - 'a' + 0xA );
    }
 
    // Illegal values in the address will not be excepted
    return -1;
}

// Function: ValidateArgs()
// Description:  Parse command line arguments and set global variables accordingly
void ValidateArgs_atm(int argc, char **argv)
{
   int i;
 
   if (argc > 1)
    {
        for (i = 1; i < argc; i++)
       {
           if ( (argv[i][0] == '-') || (argv[i][0] == '/')  )
           {
              switch (tolower(argv[i][1]) )
               {
               case '?':
                   usage_atm(argv[0]);
                   break;                 
               case 'a':   // Get my ATM address
                   EnumerateATMAddresses_atm(lpSocketProtocol);
                   ExitProcess(1);
                   break; 
               case 's':   // Act as server
                   bServer = TRUE;
                   break; 
              case 'l':   // Use this local interface
                   if(i+1 < argc)
                   {
                      if(strlen(argv[i+1]) == NSAP_ADDR_LEN)
                          strncpy_s(szServerAddr, sizeof(szServerAddr),argv[i+1],NSAP_ADDR_LEN);
                       else if(strlen(argv[i+1]) == 1)
                           dwInterface = atoi(argv[i+1]);
                      else
                          usage_atm(argv[0]); 
                      ++i;
                  }
                  break;
              case 'r':   // server's address
                   if(i+1 < argc)
                   {
                      if(strlen(argv[i+1]) == NSAP_ADDR_LEN)
                          strncpy_s(szServerAddr, sizeof(szServerAddr),argv[i+1],NSAP_ADDR_LEN);
                      else
                          usage_atm(argv[0]);
                       ++i;
                   }
                  break;
                case 'p':   // server's selector byte (port)
                   if(i+1 < argc)
                   {
                       if(strlen(argv[i+1]) == SELECTOR_BYTE_LEN)
                          strncpy_s(szPort,sizeof(szPort),argv[i+1],SELECTOR_BYTE_LEN);
                       else
                           usage_atm(argv[0]);
                       ++i;
                   }
                  break;
               default:
                   usage_atm(argv[0]);
                  break;
              }
           }
       }
   }
    return;
}

// Function: GetNumATMInterfaces()
// Description: This function enumerates the number of ATM interfaces on the local machine
int GetNumATMInterfaces_atm(SOCKET s)
{
    DWORD  dwNum, nbytes=sizeof(DWORD);
 
    if (WSAIoctl(s, 
		         SIO_GET_NUMBER_OF_ATM_DEVICES, 
				 NULL, 
				 0, 
				 (LPVOID)&dwNum, sizeof(DWORD),
                 &nbytes, 
				 NULL, 
				 NULL) == SOCKET_ERROR)
    {
        return -1;
    }
	else
	    printf("WSAIoctl(SIO_GET_NUMBER_OF_ATM_DEVICES) seems working...\n");
    return dwNum;
}

// Function: GetATMAddress()
// Description:
//    This function returns the ATM_ADDRESS corresponding to
//    the given interface number. The GetNumATMInterfaces function
//    returns the number of interfaces. These number can be
//    passed as the device number into this function (ATM devices begin with number 0)
BOOL GetATMAddress_atm(SOCKET s,int device, ATM_ADDRESS *atmaddr)
{
    DWORD bytes;
 
    if (WSAIoctl(s, 
				 SIO_GET_ATM_ADDRESS, 
				 (LPVOID)&device, 
				 sizeof(DWORD),
				 (LPVOID)atmaddr, 
				 sizeof(ATM_ADDRESS), 
				 &bytes, 
				 NULL, 
				 NULL) == SOCKET_ERROR)
    {
        return FALSE;
    }
    printf("WSAIoctl(SIO_GET_ATM_ADDRESS) seems working...\n");
 
    return TRUE;
}

// Function: usage()
// Description: The usage information
int usage_atm(char *progname)
{
    printf("Usage: %s -a -s -l interface -r server-addr -p port\n", progname);
    printf("       -a          Print a list of ATM interface addresses\n");
    printf("       -s          Act as server\n");
    printf("       -l <if >    Interface to bind to:\n");
    printf("                    Interface number (e.g. 0, 1, etc.)\n");
    printf("                    NSAP ATM address (38 character address)\n");
    printf("       -r <addr>   NSAP ATM address of server\n");
    printf("       -p <port>   Selector byte (2 digit hexadecimal port number)\n");
    printf("Server example: -s -l 2 -p 12\n");
    printf("Sender example: -l 3 -r 4700918100000000613E5BFE010020480811F300 -p 12\n");
    printf("                                     -l 1 -r 11.111122223333444455556666.112233445566.00 -p 12\n");
    return 0;
}

// Function: FindProtocol()
// Description:
//    This function enumerates protocols installed on the system
//    with WSAEnumProtocols and searches for ATM transports
WSAPROTOCOL_INFO* FindProtocol_atm()
{
    WSAPROTOCOL_INFO   *lpProtocolBuf=NULL, *lpRet=NULL;
    DWORD               dwErr, dwRet, dwBufLen=0, i;
 
    if (WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen) != SOCKET_ERROR)
    {
       // This should never happen as there is a NULL buffer
       printf("WSAEnumProtocols() failed with error code %d\n", WSAGetLastError());
       return FALSE;
    }
    else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
    {
       // We failed for some reason not relating to buffer size - also odd
       printf("WSAEnumProtocols() failed with error code %d\n", dwErr);
       return FALSE;
    }
    else
       printf("WSAEnumProtocols() should be fine!\n");
 
	// Allocate the correct buffer size for WSAEnumProtocols as
	// well as the buffer to return
	lpProtocolBuf = (WSAPROTOCOL_INFO *)GlobalAlloc(GMEM_FIXED, dwBufLen);
 
    if (lpProtocolBuf == NULL)
    {
       printf("GlobalAlloc() failed with error code %d\n", GetLastError());
       return FALSE;
    }
    else
       printf("GlobalAlloc() is OK!\n");
 
    dwRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
    if (dwRet == SOCKET_ERROR)
    {
       printf("WSAEnumProtocols() failed with error code %d\n", WSAGetLastError());
       GlobalFree(lpProtocolBuf);
       return FALSE;
    }
    else
       printf("WSAEnumProtocols() should be fine!\n");
 
    // Loop through the returned protocol information looking for those
    // that are in the AF_ATM address family
    for (i=0; i < dwRet ;i++)
    {
       if ( (lpProtocolBuf[i].iAddressFamily == AF_ATM) 
		 && (lpProtocolBuf[i].iSocketType == SOCK_RAW) 
		 //&& (lpProtocolBuf[i].iProtocol == ATMPROTO_AAL5))
		 && (lpProtocolBuf[i].iProtocol == ATMPROTO_AAL2))
       {
          lpRet = GlobalAlloc(GMEM_FIXED, sizeof(WSAPROTOCOL_INFO));
          memcpy(lpRet, &lpProtocolBuf[i], sizeof(WSAPROTOCOL_INFO));
          break;
       }
    }
 
    GlobalFree(lpProtocolBuf);
    return lpRet;
}

// Function: AtoH()
// Description:
//    This function coverts the ATM address specified in the string
//    (ascii) format to the binary(hexadecimal) format
void AtoH_atm(CHAR *szDest, CHAR *szSource, INT iCount)
{
    while (iCount--)
    {
       *szDest++ = ( BtoH_atm( *szSource++ ) << 4 ) + BtoH_atm ( *szSource++ );
    }
    return;
}
 

// Function: EnumerateATMAddresses()
// Description: Enumerate all ATM interfaces and print their addresses
void EnumerateATMAddresses_atm(WSAPROTOCOL_INFO *lpProtocol)
{
    SOCKET             s;
    SOCKADDR_ATM       atm_addr;
    char               szAddress[MAX_ATM_ADDR_LEN];
    DWORD              dwNumInterfaces, dwAddrLen=MAX_ATM_ADDR_LEN, i;
 
    s = WSASocket(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, lpProtocol, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return;
    }
    else
        printf("WSASocket() is OK!\n");
 
    dwNumInterfaces = GetNumATMInterfaces_atm(s);
 
    for(i=0; i < dwNumInterfaces ;i++)
    {
		ZeroMemory((PVOID)&atm_addr, sizeof(SOCKADDR_ATM));
		ZeroMemory((PVOID)szAddress, MAX_ATM_ADDR_LEN);

		if (GetATMAddress_atm(s, i, &atm_addr.satm_number) == FALSE)
		{
			printf("GetATMAddress() failed!\n");
			break;
		}

		atm_addr.satm_family                 = AF_ATM;
		atm_addr.satm_number.AddressType     = ATM_NSAP;
		atm_addr.satm_number.NumofDigits     = ATM_ADDR_SIZE;
		atm_addr.satm_blli.Layer2Protocol    = SAP_FIELD_ANY;
		atm_addr.satm_blli.Layer3Protocol    = SAP_FIELD_ABSENT;
		atm_addr.satm_bhli.HighLayerInfoType = SAP_FIELD_ABSENT;
		if (WSAAddressToString((LPSOCKADDR)&atm_addr, sizeof(atm_addr),lpProtocol, (LPWSTR)szAddress, &dwAddrLen))
		{
			printf("WSAAddressToString() failed with error code %d\n", WSAGetLastError());
			break;
		}
		printf("ATM ADDRESS <%d>: '%s'\n", i, szAddress);
    }
    closesocket(s);
    return;
}

// Function: main()
// Description:
//    This function parses arguments and starts either the client or server
//    depending on the arguments. The server will enumerate local ATM
//    interfaces if necessary and bind to a local address and wait for
//    client connections. The server handles the client connection in the
//    same thread as the listening socket. The client only handles one
//    connection and then exits. If running the client, create a socket
//    and connect to the server. Once connected, send a message and read it back
int main_atm(int argc, char **argv)
{
    WSADATA      wsd;
    SOCKET       s;
    DWORD        dwAddrLen = sizeof(SOCKADDR_ATM);
    SOCKADDR_ATM atm_addr;
    int          ret, i;
    char         szAddress[41];
 
    // Initialize Winsock and parse command line arguments
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
       printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
       return -1;
    }
    else
       printf("WSAStartup() is OK!\n");
 
    // Find the protocol entry supporting ATM and create a socket
    lpSocketProtocol = FindProtocol_atm();
    if (lpSocketProtocol == NULL)
    {
       printf("FindProtocol() returned NULL!\n");
       return -1;
    }
    else
       printf("FindProtocol() is OK!\n");
 
    s = WSASocket(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO,
                  lpSocketProtocol, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
       printf("WSASocket() failed with error code %d\n", WSAGetLastError());
       return -1;
    }
    else
       printf("WSASocket() is OK!\n");
 
    ValidateArgs_atm(argc, argv);

    ZeroMemory((PVOID)&atm_addr, sizeof(atm_addr));
    atm_addr.satm_family                 = AF_ATM;
    atm_addr.satm_number.AddressType     = ATM_NSAP;
    atm_addr.satm_number.NumofDigits     = 20;
    atm_addr.satm_blli.Layer2Protocol    = SAP_FIELD_ABSENT;
    atm_addr.satm_blli.Layer3Protocol    = SAP_FIELD_ABSENT;
    atm_addr.satm_bhli.HighLayerInfoType = SAP_FIELD_ABSENT;
    // If an interface number was not supplied (i.e. a 38 character NSAP ATM address was supplied)
    if ((!bServer) || (dwInterface == -1))
    {
        strncpy_s(&szAddress[0], sizeof(szAddress[0]),szServerAddr, 38);
        strncpy_s(&szAddress[38], sizeof(szAddress[38]),szPort, 2);
        szAddress[40] = 0;
        AtoH_atm((char *) &atm_addr.satm_number.Addr[0], szAddress, 20);
    }
    else
    {
        // An index was supplied, look it up and get its address
        if (GetATMAddress_atm(s, dwInterface, &atm_addr.satm_number) == FALSE)
        {
            printf("Unable to get ATM interface!\n");
        }
        AtoH_atm((char *) &atm_addr.satm_number.Addr[19], szPort, 1);
    }
   
    if (bServer)
    {
        SOCKET        sclient;
        SOCKADDR_ATM  atm_client;
        int           atmaddrsz = sizeof(SOCKADDR_ATM);
        DWORD         dwClientAddrLen=MAX_ATM_ADDR_LEN;
        struct fd_set fdaccept;
        char          recvbuf[MAX_BUFFER], szClientAddr[MAX_ATM_ADDR_LEN], szAddr[MAX_BUFFER];
        dwAddrLen   = MAX_BUFFER;
 
        // Print out the address we're binding to
        if (WSAAddressToString((LPSOCKADDR)&atm_addr, sizeof(atm_addr), lpSocketProtocol, (LPWSTR)szAddr, &dwAddrLen))
        {
			printf("WSAAddressToString() failed with error code %d\n", WSAGetLastError());
        }
        else
            printf("WSAAddressToString() is OK!\n");
 
        printf("Binding to: <%s>\n", szAddr);

        if (bind(s, (SOCKADDR *)&atm_addr, sizeof(atm_addr)) == SOCKET_ERROR)
        {
            printf("bind() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("bind() should be fine!\n");
 
        if(listen(s, 7) == SOCKET_ERROR)
            printf("listen() failed with error code %d\n", WSAGetLastError());
        else
            printf("listen() is OK! I am listening...\n");

        FD_ZERO(&fdaccept);
        FD_SET(s, &fdaccept);

        if (select(0, &fdaccept, NULL, NULL, NULL) == SOCKET_ERROR)
            printf("Something wrong with select() with error code %d\n", WSAGetLastError());
        else if (select(0, &fdaccept, NULL, NULL, NULL) == 0)
            printf("select() - the time limit expired!\n");
        else
            printf("select() should be OK!\n");
 
        sclient = WSAAccept(s, (SOCKADDR *)&atm_client, &atmaddrsz, NULL, 0);
 
        if (sclient == INVALID_SOCKET)
        {
            printf("WSAAccept() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("WSAAccept() should be OK!\n");

        ZeroMemory((PVOID)szClientAddr, MAX_ATM_ADDR_LEN);
        if (WSAAddressToString((LPSOCKADDR)&atm_client, sizeof(atm_client), lpSocketProtocol, (LPWSTR)szClientAddr, &dwClientAddrLen))
        {
            printf("WSAAddressToString() failed with herror code %d\n", WSAGetLastError());
        }
        else
            printf("WSAAddressToString() is OK!\n");

        printf("Client's ATM ADDRESS: <%s>\n", szClientAddr);
 
        // Handle the client connection until it closes
        while (1)
        {
			ret = recv(sclient, recvbuf, MAX_BUFFER, 0);
			if (ret == SOCKET_ERROR)
			{
				if(WSAEDISCON == WSAGetLastError())
					printf("recv() - a connection closed by peer...\n");
				else
					printf("recv() failed with error code %d\n", WSAGetLastError());
				return -1;
			}
			else if (ret == 0)
			{
				printf("recv() - a graceful close!\n");
				break;
			}
			else
				printf("recv() is OK!\n");
 
            recvbuf[ret] = '\0';
            printf("Read: '%s'\n", recvbuf);
 
            ret = send(sclient, recvbuf, ret, 0);
 
            if (ret == SOCKET_ERROR)
            {
				if(WSAEDISCON == WSAGetLastError())
					printf("send() - a connection closed by peer!\n");
				else
					printf("send() failed with error code %d\n", WSAGetLastError());
				return -1;
            }
            else
                printf("send() should be OK!\n");

            printf("wrote %d bytes...\n", ret);
        }
        closesocket(sclient);
    }
    else
    {
        char     sendbuf[MAX_BUFFER], szAddr[MAX_BUFFER];
        dwAddrLen = MAX_BUFFER;

        // Connect and then send and recv data
        if (WSAAddressToString((LPSOCKADDR)&atm_addr, sizeof(atm_addr), lpSocketProtocol, (LPWSTR)szAddr, &dwAddrLen))
        {
            printf("WSAAddressToString() failed with error code %d\n", WSAGetLastError());
        }
        else
            printf("WSAAddressToString() is OK\n");
 
        printf("Connect to: <%s>\n", szAddr);
        if (connect(s, (SOCKADDR *)&atm_addr, sizeof(atm_addr)) == SOCKET_ERROR)
        {
            printf("connect() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
			printf("connect() is OK!\n");
 
        memset(sendbuf, '$', 512);
        for(i=0; i < 10 ;i++)
        {
            ret = send(s, sendbuf, 64, 0);
            if (ret == SOCKET_ERROR)
            {
                if(WSAEDISCON == WSAGetLastError())
					printf("send() - connection closed by peer!\n");
                else
                    printf("send() failed with error code %d\n", WSAGetLastError());
                return -1;
            }
			else
				printf("send() should be OK!\n");
 
			printf("Sent %d bytes\n", ret);
 
            ret = recv(s, sendbuf, ret, 0);
            if (ret == SOCKET_ERROR)
            {
				if(WSAEDISCON == WSAGetLastError())
					printf("recv() - connection closed by peer!\n");
				else
					printf("recv() failed with error code %d\n", WSAGetLastError());
				return -1;
            }
            else if (ret == 0)
            {
                printf("recv() - a graceful close\n");
                break;
            }
            else
                printf("recv() is OK!\n");
            sendbuf[ret] = '\0';
            printf("Read: '%s'\n", sendbuf);
        }
   }
   closesocket(s);
   WSACleanup();
   return 0;
}