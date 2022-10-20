#include "TCP_Client.h"
#include <iostream>

#define PORT "1112"
#define SERVERIP "127.0.0.1"

int main(int argc, char** argv)
{
	TCP_Client Client;
	int result = 0;
	std::string name;

	std::cout << "Enter your name:";
	std::cin >> name;

	std::cout << "Connecting to chat server....."<<std::endl;
	//initialize winsock
	result = Client.initialize(SERVERIP, PORT);
	if (result != 0)
	{
		return result;
	}

	result = Client.SendToServer(SetName, 0xff, name); // send name to server
	result = Client.ReceiveFromServer(); //acknowledge from server

	result = Client.Chat(name);


	//shutdown winsock
	Client.CloseSocket();

	
	return 0;
}