#include "TCP_Server.h"

TCP_Server::TCP_Server()
{
	ZeroMemory(&wsaData, sizeof(wsaData));
	info = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	ListenSocket = NULL;
	ClientSocket = NULL;
}

TCP_Server::~TCP_Server()
{
}

int TCP_Server::WinsockInit()
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
	hints.ai_flags = AI_PASSIVE;

	printf("Getaddrinfo\t");
	result = getaddrinfo(NULL, PORT, &hints, &info);
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

int TCP_Server::SocketCreate()
{
	int result = 0;

	printf("Create Socket\t");
	ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
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

int TCP_Server::BindSocket()
{
	int result = 0;
	printf("Binding\t");
	result = bind(ListenSocket, info->ai_addr, info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("- bind error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}
	return result;
}

int TCP_Server::ListenConnection()
{
	int result = 0;
	printf("listening\t");
	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		printf("- listen error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}
	return result;
}

int TCP_Server::AcceptConnection()
{
	int result = 0;

	printf("Accept\t");
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("- accept error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		printf("- success\n");
	}
	return result;
}

void TCP_Server::CloseSocket()
{
	printf("closing socket\n");
	freeaddrinfo(info);
	closesocket(ListenSocket);
	closesocket(ClientSocket);
}

void TCP_Server::ShutdownWinsock()
{
	printf("shutdown winsock\n");
	WSACleanup();
}
