
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#ifndef WINSOCKET_H
#define WINSOCKET_H

void StartMain();
void BindSocket();
int TcpServer();
void UniversalServer();

#endif