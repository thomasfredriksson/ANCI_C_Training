
// Description:
//
//    This sample illustrates how to develop a simple echo server Winsock
//    application using the Overlapped I/O model with event notification. This
//    sample is implemented as a console-style application and simply prints
//    messages when connections are established and removed from the server.
//    The application listens for TCP connections on port 5150 and accepts them
//    as they arrive. When this application receives data from a client, it
//    simply echos (this is why we call it an echo server) the data back in
//    it's original form until the client closes the connection.
//
//    Note: There are no command line options for this sample.
// Link to ws2_32.lib

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "winsocketoverlapped.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512

typedef struct _SOCKET_INFORMATION_OVERLAPPED {
   CHAR Buffer[DEFAULT_BUFLEN];
   WSABUF DataBuf;
   SOCKET Socket;
   WSAOVERLAPPED Overlapped;
   DWORD BytesSEND;
   DWORD BytesRECV;
} SOCKET_INFORMATION_OVERLAPPED, *LPSOCKET_INFORMATION_OVERLAPPED;

DWORD WINAPI ProcessIO_Overlapped(LPVOID lpParameter);

DWORD EventTotal_Overlapped = 0;
WSAEVENT EventArray_Overlapped[WSA_MAXIMUM_WAIT_EVENTS];
LPSOCKET_INFORMATION_OVERLAPPED SocketArray_Overlapped[WSA_MAXIMUM_WAIT_EVENTS];
CRITICAL_SECTION CriticalSection_Overlapped;

void main_overlapped()
{
   WSADATA wsaData;
   SOCKET ListenSocket, AcceptSocket;
   SOCKADDR_IN InternetAddr;
   DWORD Flags;
   DWORD ThreadId;
   DWORD RecvBytes;
 
   InitializeCriticalSection(&CriticalSection_Overlapped);
 
   if (WSAStartup((2,2),&wsaData) != 0)
   {
      printf("WSAStartup() failed with error %d\n", WSAGetLastError());
      WSACleanup();
      return 1;
   }
   else
      printf("WSAStartup() looks nice!\n");
 
   if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
   {
      printf("Failed to get a socket %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("WSASocket() is OK lol!\n");
 
   InternetAddr.sin_family = AF_INET;
   InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   InternetAddr.sin_port = htons(5150);
 
   if (bind(ListenSocket, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
   {
      printf("bind() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("YOu see, bind() is working!\n");
 
   if (listen(ListenSocket, 5))
   {
      printf("listen() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("listen() is OK maa...\n");
 
   // Setup the listening socket for connections
   if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
   {
      printf("Failed to get a socket %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("WSASocket() looks OK!\n");
 
   if ((EventArray_Overlapped[0] = WSACreateEvent()) == WSA_INVALID_EVENT)
   {
      printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("WSACreateEvent() is OK!\n");
 
   EventTotal_Overlapped = 1;

   // Create a thread to service overlapped requests
   if (CreateThread(NULL, 0, ProcessIO_Overlapped, NULL, 0, &ThreadId) == NULL)
   {
      printf("CreateThread() failed with error %d\n", GetLastError());
      return 1;
   }
   else
      printf("Nothing to say, CreateThread() is OK!\n");

   while(TRUE)
   {
       // Accept inbound connections
      if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) == INVALID_SOCKET)
      {
          printf("accept() failed with error %d\n", WSAGetLastError());
          return 1;
      }
      else
          printf("accept() is OK!\n");
 
      EnterCriticalSection(&CriticalSection_Overlapped);
      // Create a socket information structure to associate with the accepted socket
      if ((SocketArray_Overlapped[EventTotal_Overlapped] = (LPSOCKET_INFORMATION_OVERLAPPED) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION_OVERLAPPED))) == NULL)
      {
         printf("GlobalAlloc() failed with error %d\n", GetLastError());
         return 1;
      }
      else
         printf("GlobalAlloc() for LPSOCKET_INFORMATION is pretty fine!\n");
 
      // Fill in the details of our accepted socket
      SocketArray_Overlapped[EventTotal_Overlapped]->Socket = AcceptSocket;
      ZeroMemory(&(SocketArray_Overlapped[EventTotal_Overlapped]->Overlapped), sizeof(OVERLAPPED));
      SocketArray_Overlapped[EventTotal_Overlapped]->BytesSEND = 0;
      SocketArray_Overlapped[EventTotal_Overlapped]->BytesRECV = 0;
      SocketArray_Overlapped[EventTotal_Overlapped]->DataBuf.len = DEFAULT_BUFLEN;
      SocketArray_Overlapped[EventTotal_Overlapped]->DataBuf.buf = SocketArray_Overlapped[EventTotal_Overlapped]->Buffer;
 
      if ((SocketArray_Overlapped[EventTotal_Overlapped]->Overlapped.hEvent = EventArray_Overlapped[EventTotal_Overlapped] = WSACreateEvent()) == WSA_INVALID_EVENT)
      {
         printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
         return 1;
      }
      else
         printf("WSACreateEvent() is OK!\n");
 
      // Post a WSARecv() request to to begin receiving data on the socket
      Flags = 0;
	  /* lpOverlapped [in]
       * A pointer to a WSAOVERLAPPED structure (ignored for nonoverlapped sockets). 
	   */
      if (WSARecv(
		     SocketArray_Overlapped[EventTotal_Overlapped]->Socket,
             &(SocketArray_Overlapped[EventTotal_Overlapped]->DataBuf), 
			 1, 
			 &RecvBytes, 
			 &Flags, 
			 &(SocketArray_Overlapped[EventTotal_Overlapped]->Overlapped), 
			 NULL) == SOCKET_ERROR)
      {
         if (WSAGetLastError() != ERROR_IO_PENDING)
         {
            printf("WSARecv() failed with error %d\n", WSAGetLastError());
            return 1;
         }
      }
      else
         printf("WSARecv() should be working!\n");
 
      EventTotal_Overlapped++;
      LeaveCriticalSection(&CriticalSection_Overlapped);
 
      // Signal the first event in the event array to tell the worker thread to
      // service an additional event in the event array
	  /* The WSASetEvent function sets the state 
	   * of the specified event object to signaled. 
	   */
      if (WSASetEvent(EventArray_Overlapped[0]) == FALSE)
      {
         printf("WSASetEvent() failed with error %d\n", WSAGetLastError());
         return 1;
      }
      else
         printf("Don't worry, WSASetEvent() is OK!\n");
   }
}

DWORD WINAPI ProcessIO_Overlapped(LPVOID lpParameter)
{
   DWORD Index;
   DWORD Flags;
   LPSOCKET_INFORMATION_OVERLAPPED SI;
   DWORD BytesTransferred;
   DWORD i;
   DWORD RecvBytes, SendBytes;
 
   // Process asynchronous WSASend, WSARecv requests
   while(TRUE)
   {
      if ((Index = WSAWaitForMultipleEvents(EventTotal_Overlapped, EventArray_Overlapped, FALSE,  WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
      {
         printf("WSAWaitForMultipleEvents() failed %d\n", WSAGetLastError());
         return 0;
      }
      else
         printf("WSAWaitForMultipleEvents() is OK!\n");
 
      // If the event triggered was zero then a connection attempt was made
      // on our listening socket.
      if ((Index - WSA_WAIT_EVENT_0) == 0)
      {
         WSAResetEvent(EventArray_Overlapped[0]);
         continue;
      }
 
      SI = SocketArray_Overlapped[Index - WSA_WAIT_EVENT_0];
      WSAResetEvent(EventArray_Overlapped[Index - WSA_WAIT_EVENT_0]);
 
	  /* The WSAGetOverlappedResult function retrieves the results 
	   * of an overlapped operation on the specified socket. 
	   * s [in]
       *   A descriptor identifying the socket.
	   * lpOverlapped [in]
       *   A pointer to a WSAOVERLAPPED structure that was specified 
	   *   when the overlapped operation was started. 
	   *   This parameter must not be a NULL pointer.
	   * lpcbTransfer [out]
       *   A pointer to a 32-bit variable that receives the number of bytes 
	   *   that were actually transferred by a send or receive operation, or by the WSAIoctl function. 
	   *   This parameter must not be a NULL pointer.
	   */
      if (WSAGetOverlappedResult(SI->Socket, &(SI->Overlapped), &BytesTransferred, FALSE, &Flags) == FALSE || BytesTransferred == 0)
      {
         printf("Closing socket %d\n", SI->Socket);
 
         if (closesocket(SI->Socket) == SOCKET_ERROR)
         {
            printf("closesocket() failed with error %d\n", WSAGetLastError());
         }
         else
            printf("closesocket() is OK!\n");
 
         GlobalFree(SI);
         WSACloseEvent(EventArray_Overlapped[Index - WSA_WAIT_EVENT_0]);
         // Cleanup SocketArray and EventArray by removing the socket event handle
         // and socket information structure if they are not at the end of the arrays
         EnterCriticalSection(&CriticalSection_Overlapped);
 
         if ((Index - WSA_WAIT_EVENT_0) + 1 != EventTotal_Overlapped)
            for (i = Index - WSA_WAIT_EVENT_0; i < EventTotal_Overlapped; i++)
            {
               EventArray_Overlapped[i] = EventArray_Overlapped[i + 1];
               SocketArray_Overlapped[i] = SocketArray_Overlapped[i + 1];
            }
 
         EventTotal_Overlapped--;
         LeaveCriticalSection(&CriticalSection_Overlapped);
         continue;
      }
      // Check to see if the BytesRECV field equals zero. If this is so, then
      // this means a WSARecv call just completed so update the BytesRECV field
      // with the BytesTransferred value from the completed WSARecv() call.
      if (SI->BytesRECV == 0)
      {
         SI->BytesRECV = BytesTransferred;
         SI->BytesSEND = 0;
      }
      else
      {
         SI->BytesSEND += BytesTransferred;
      }
 
      if (SI->BytesRECV > SI->BytesSEND)
      {
         // Post another WSASend() request.
         // Since WSASend() is not guaranteed to send all of the bytes requested,
         // continue posting WSASend() calls until all received bytes are sent
         ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
         SI->Overlapped.hEvent = EventArray_Overlapped[Index - WSA_WAIT_EVENT_0];
 
         SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
         SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;
 
         if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0, &(SI->Overlapped), NULL) == SOCKET_ERROR)
         {
            if (WSAGetLastError() != ERROR_IO_PENDING)
            {
               printf("WSASend() failed with error %d\n", WSAGetLastError());
               return 0;
            }
         }
         else
            printf("WSASend() is OK!\n");
      }
      else
      {
         SI->BytesRECV = 0;
         // Now that there are no more bytes to send post another WSARecv() request
         Flags = 0;
         ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
         SI->Overlapped.hEvent = EventArray_Overlapped[Index - WSA_WAIT_EVENT_0];
 
         SI->DataBuf.len = DEFAULT_BUFLEN;
         SI->DataBuf.buf = SI->Buffer;
 
         if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, &(SI->Overlapped), NULL) == SOCKET_ERROR)
         {
            if (WSAGetLastError() != ERROR_IO_PENDING)
            {
               printf("WSARecv() failed with error %d\n", WSAGetLastError());
               return 0;
            }
         }
         else
             printf("WSARecv() is OK!\n");
      }
   }
}

