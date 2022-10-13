#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define PORT "1112"
#define DEBUG_print true

class TCP_Server
{
public:
	TCP_Server();
	~TCP_Server();

	int Initialize();
	int ReadFromClient();
	int SendToClient();
	void CloseSocket();

	WSADATA wsaData;
	struct addrinfo* info;
	struct addrinfo hints;
	SOCKET ListenSocket;
	SOCKET ClientSocket;


private:
	int WinsockInit();
	int SocketCreate();
	int BindSocket();
	int ListenConnection();
	int AcceptConnection();
};

