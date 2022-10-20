#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Buffer.h"
#include "ProtocolChat.h"
#include "RoomStatus.h"

#pragma comment(lib, "Ws2_32.lib")

enum ClientCMD
{
	SetName = 0,
	JoinRoom,
	LeaveRoom,
	SendMSG = 4
};

class TCP_Client
{
public:
	TCP_Client();
	~TCP_Client();

	int initialize(PCSTR ip, PCSTR port);
	void CloseSocket();
	int SendToServer(uint16_t cmd, uint16_t opcode, std::string s);
	int ReceiveFromServer();
	int Chat(std::string user);

	WSADATA wsaData;
	struct addrinfo* info;
	struct addrinfo* ptr;
	struct addrinfo hints;
	SOCKET ConnectSocket;
	SOCKET ClientSocket;



private:
	int WinsockInit(PCSTR ip, PCSTR port);
	int SocketCreate();
	int MakeConnect();
	int CloseConnection();
	void ShutdownWinsock();
};

