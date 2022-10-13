#include "TCP_Server.h"

TCP_Server::TCP_Server()
{
	ZeroMemory(&wsaData, sizeof(wsaData));
	info = nullptr;
	ZeroMemory(&hints, sizeof(hints));
	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

}

TCP_Server::~TCP_Server()
{
}

int TCP_Server::Initialize()
{
	int result = 0;

	//initialize winsock
	result = WinsockInit();
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

	//bind
	result = BindSocket();
	if (result != 0)
	{
		return result;
	}
	//listen
	result = ListenConnection();
	if (result != 0)
	{
		return result;
	}

	//accept 
	result = AcceptConnection();
	if (result != 0)
	{
		return result;
	}
	
	return result;
}

int TCP_Server::WinsockInit()
{
	int result = 0;

#if DEBUG_print
	printf("Initialize WSA\t");
#endif

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("- error WSAStartup %#x\n", result);
		return 1;
	}
#if DEBUG_print
	else
	{
		printf("- success\n");
	}
#endif

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

#if DEBUG_print
	printf("Getaddrinfo\t");
#endif

	result = getaddrinfo(NULL, PORT, &hints, &info);
	if (result != 0)
	{
		printf("- error getaddrinfo %#x\n", result);
		WSACleanup();
		return 1;
	}
#if DEBUG_print
	else
	{
		printf("- success\n");
	}
#endif
	return result;
}

int TCP_Server::SocketCreate()
{
	int result = 0;

#if DEBUG_print
	printf("Create Socket\t");
#endif
	ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("- error create socket %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		return 1;
	}
#if DEBUG_print
	else
	{
		printf("- success\n");
	}
#endif
	return result;
}

int TCP_Server::BindSocket()
{
	int result = 0;
#if DEBUG_print
	printf("Binding\t");
#endif
	result = bind(ListenSocket, info->ai_addr, (int)info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("- bind error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
#if DEBUG_print
	else
	{
		printf("- success\n");
	}
#endif
	return result;
}

int TCP_Server::ListenConnection()
{
	int result = 0;
#if DEBUG_print
	printf("listening\t");
#endif
	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		printf("- listen error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
#if DEBUG_print
	else
	{
		printf("- success\n");
	}
#endif
	return result;
}

int TCP_Server::AcceptConnection()
{
	int result = 0;
#if DEBUG_print
	printf("Accept\t");
#endif
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("- accept error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
#if DEBUG_print
	else
	{
		printf("- success\n");
	}
#endif
	return result;
}

int TCP_Server::ReadFromClient()
{
	int result = 0;


	return result;
}

int TCP_Server::SendToClient()
{
	int result = 0;


	return result;
}

void TCP_Server::CloseSocket()
{
#if DEBUG_print
	printf("closing socket\n");
#endif
	freeaddrinfo(info);
	closesocket(ListenSocket);
	closesocket(ClientSocket);
#if DEBUG_print
	printf("shutdown winsock\n");
#endif
	WSACleanup();
}
