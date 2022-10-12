#include "TCP_Client.h"

TCP_Client::TCP_Client()
{
	ZeroMemory(&wsaData, sizeof(wsaData));
	info = nullptr;
	ptr = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	ConnectSocket = NULL;
	ClientSocket = NULL;
}

TCP_Client::~TCP_Client()
{
}

int TCP_Client::WinsockInit()
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
	result = getaddrinfo(SERVERIP, PORT, &hints, &info);
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
	result = connect(ConnectSocket, info->ai_addr, info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("- error connecting %#x\n", WSAGetLastError());
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
