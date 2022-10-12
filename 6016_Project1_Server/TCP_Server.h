#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define PORT "1112"

class TCP_Server
{
public:
	TCP_Server();
	~TCP_Server();

	int WinsockInit();
	int SocketCreate();
	int BindSocket();
	int ListenConnection();
	int AcceptConnection();
	void CloseSocket();
	void ShutdownWinsock();

	WSADATA wsaData;
	struct addrinfo* info;
	struct addrinfo hints;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
};

