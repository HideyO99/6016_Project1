#include "TCP_Client.h"
#include <iostream>

TCP_Client::TCP_Client()
{
	ZeroMemory(&wsaData, sizeof(wsaData));
	info = nullptr;
	ptr = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	ConnectSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;
}

TCP_Client::~TCP_Client()
{
}

int TCP_Client::initialize(PCSTR ip, PCSTR port)
{
	int result = 0;

	//initialize winsock
	result = WinsockInit(ip,port);
	if (result != 0)
	{
		return result;
	}

	//socket creation
	result = SocketCreate();
	if (result != 0)
	{
		return result;
	}

	//connect
	result = MakeConnect();
	if (result != 0)
	{
		return result;
	}

	return result;
}

void TCP_Client::CloseSocket()
{
	CloseConnection();
	ShutdownWinsock();
}

int TCP_Client::SendToServer(uint16_t cmd, uint16_t opcode, std::string s)
{
	int result = 0;
	//Buffer buffer(8);
	ProtocolChat frame;

	frame.cmd = cmd; //send to server
	frame.opcode = opcode; //room
	frame.content = s;

	Buffer buffer(frame.frameSize());
	buffer.WriteInt32BE(frame.frameSize());
	buffer.WriteShort16BE(frame.cmd);
	buffer.WriteShort16BE(frame.opcode);
	buffer.WriteInt32BE(frame.contentSize());
	buffer.WriteString(frame.content);

	result = send(ConnectSocket, (const char*)&(buffer.m_buffer[0]), frame.frameSize(), 0);
	return result;
}

int TCP_Client::ReceiveFromServer()
{
	int result = 0;
	ProtocolChat frame;
	const int rcvBuffLen = 512;
	char rcvBuff[rcvBuffLen];
	Buffer buff(512);

	//receive 
	int recvResult = recv(ConnectSocket, rcvBuff, rcvBuffLen, 0);
	//transfer to buffer
	buff.m_buffer.insert(buff.m_buffer.begin(), rcvBuff, rcvBuff + rcvBuffLen);
	uint32_t buffLen = buff.ReadInt32BE(0);
	buff.m_buffer.resize(buffLen + 1);
	uint16_t cmd = buff.ReadShort16BE(4);
	uint16_t opcode = buff.ReadShort16BE(6);
	uint32_t contentLen = buff.ReadInt32BE(8);
	std::string s = buff.ReadString(12, contentLen);

	std::cout << s;
	return result;
}

int TCP_Client::WinsockInit(PCSTR ip, PCSTR port)
{
	int result = 0;

	printf("Initialize WSA\t");
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("- error WSAStartup %#x\n", result);
		return 1;
	}
	else
	{
		printf("- success\n");
	}

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	printf("Getaddrinfo\t");
	result = getaddrinfo(ip, port, &hints, &info);
	if (result != 0)
	{
		printf("- error getaddrinfo %#x\n", result);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}

	return result;
}

int TCP_Client::SocketCreate()
{
	int result = 0;

	printf("create socket\t");
	ConnectSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("- error create socket %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}

	return result;
}

int TCP_Client::MakeConnect()
{
	int result = 0;

	printf("connecting\t");
	result = connect(ConnectSocket, info->ai_addr, (int)info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("- error connecting %d\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}

	return result;
}

int TCP_Client::CloseConnection()
{
	int result = 0;

	printf("shutdown\t");
	result = shutdown(ConnectSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("- error shutdown %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}

	return result;
}

void TCP_Client::ShutdownWinsock()
{
	printf("closing\t");
	closesocket(ConnectSocket);
	WSACleanup();
}
