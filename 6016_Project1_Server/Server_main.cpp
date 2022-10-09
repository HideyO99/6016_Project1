#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char** argv)
{
	WSADATA wsadata;
	int result = 0;
	result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0)
	{
		printf("error WSAStartup %#x", result);
		return 1;
	}

	struct addrinfo* info = nullptr;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	return 0;
}