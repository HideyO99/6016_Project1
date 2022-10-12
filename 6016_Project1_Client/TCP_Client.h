#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#define PORT "1112"
#define SERVERIP "127.0.0.1"

class TCP_Client
{
public:
	TCP_Client();
	~TCP_Client();

	WSADATA wsaData;
	struct addrinfo* info;
	struct addrinfo* ptr;
	struct addrinfo hints;
	SOCKET ConnectSocket;
	SOCKET ClientSocket;

	int WinsockInit();
	int SocketCreate();
	int MakeConnect();
	int CloseConnection();
	void ShutdownWinsock();
};

