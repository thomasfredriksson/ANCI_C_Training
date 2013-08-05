

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "winsocketeventselect.h"

#pragma comment(lib, "Ws2_32.lib")

void socket_server_event()
{
	SOCKET SocketArray [WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT EventArray [WSA_MAXIMUM_WAIT_EVENTS], NewEvent;
	SOCKADDR_IN InternetAddr;
	SOCKET Accept, Listen;
	DWORD EventTotal = 0;
	DWORD Index, i;

    // Set up a TCP socket for listening on port 5150
    Listen = socket(PF_INET, SOCK_STREAM, 0);

    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(5150);
 
    bind(Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr));

	/* The WSACreateEvent function creates a new event object.
	 * The WSACreateEvent function creates a manual-reset event object 
	 * with an initial state of nonsignaled. 
	 * The handle of the event object returned cannot be inherited by child processes. 
	 * The event object is unnamed.
	 */
    NewEvent = WSACreateEvent();

	/* The WSAEventSelect function specifies an event object 
	 * to be associated with the specified set of FD_XXX network events. 
	 * s [in]
     *   A descriptor identifying the socket.
	 * hEventObject [in]
     *   A handle identifying the event object 
	 *   to be associated with the specified 
	 *   set of FD_XXX network events.
	 * lNetworkEvents [in]
     *   A bitmask that specifies the combination of FD_XXX 
	 *   network events in which the application has interest.
	 */
    WSAEventSelect(Listen, NewEvent, FD_ACCEPT │ FD_CLOSE);
    listen(Listen, 5);
    SocketArray[EventTotal] = Listen;
    EventArray[EventTotal] = NewEvent;
    EventTotal++;

    while(TRUE)
    {
       // Wait for network events on all sockets
	   /* The WSAWaitForMultipleEvents function returns 
	    * when one or all of the specified event objects 
		* are in the signaled state, when the time-out interval expires, 
		* or when an I/O completion routine has executed.
		*
		* cEvents [in]
        *  The number of event object handles in the array pointed to by lphEvents. 
		*
		* lphEvents [in]
        *  A pointer to an array of event object handles. 
		*  The array can contain handles of objects of different types. 
		*  It may not contain multiple copies of the same handle 
		*  if the fWaitAll parameter is set to TRUE. 
		*
		* fWaitAll [in]
        *  A value that specifies the wait type. 
		*  If TRUE, the function returns when the state of 
		*  all objects in the lphEvents array is signaled. 
		*  If FALSE, the function returns when any of the event objects is signaled. 
		*  In the latter case, the return value minus WSA_WAIT_EVENT_0 
		*  indicates the index of the event object whose state caused the function to return.
		*  
		* dwTimeout [in]
        *  The time-out interval, in milliseconds. 
		*  WSAWaitForMultipleEvents returns if the time-out interval expires, 
		*  even if conditions specified by the fWaitAll parameter are not satisfied.
		*  If dwTimeout is WSA_INFINITE, WSAWaitForMultipleEvents waits forever; 
		*  that is, the time-out interval never expires.
		* 
		* fAlertable [in]
        *  A value that specifies whether the thread is placed 
		*  in an alertable wait state so the system can execute I/O completion routines. 
		*  If TRUE, the thread is placed in an alertable wait state 
		*  and WSAWaitForMultipleEvents can return 
		*  when the system executes an I/O completion routine. 
		*  In this case, WSA_WAIT_IO_COMPLETION is returned and the event that was being waited on is not signaled yet. The application must call the WSAWaitForMultipleEvents function again. If FALSE, the thread is not placed in an alertable wait state and I/O completion routines are not executed.
		* 
	    */
       Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);
       Index = Index - WSA_WAIT_EVENT_0;
 
       // Iterate through all events to see if more than one is signaled
       for(i=Index; i < EventTotal ;i++)
       {
          Index = WSAWaitForMultipleEvents(1, &EventArray[i], TRUE, 1000, FALSE);
          if ((Index == WSA_WAIT_FAILED) ││ (Index == WSA_WAIT_TIMEOUT))
             continue;
          else
          {

	      }
	   }
    }
}