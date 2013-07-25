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
 
#define MAX_ATM_ADDR_LEN        64
#define MAX_BUFFER              1024
#define NSAP_ADDR_LEN           38
#define SELECTOR_BYTE_LEN       2
 
WSAPROTOCOL_INFO  *lpSocketProtocol=NULL;
 
char szServerAddr[39],        // Server's ATM NSAP address
           szPort[3];         // Port number
BOOL bServer = FALSE;
DWORD dwInterface=-1;         // Which interface to bind to?

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
		 && (lpProtocolBuf[i].iProtocol == ATMPROTO_AAL5))
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
       *szDest++ = ( BtoH ( *szSource++ ) << 4 ) + BtoH ( *szSource++ );
    }
    return;
}
 
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
 
    dwNumInterfaces = GetNumATMInterfaces(s);
 
    for(i=0; i < dwNumInterfaces ;i++)
    {
		ZeroMemory((PVOID)&atm_addr, sizeof(SOCKADDR_ATM));
		ZeroMemory((PVOID)szAddress, MAX_ATM_ADDR_LEN);

		if (GetATMAddress(s, i, &atm_addr.satm_number) == FALSE)
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