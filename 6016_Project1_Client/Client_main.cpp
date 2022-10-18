#include "TCP_Client.h"

#define PORT "1112"
#define SERVERIP "127.0.0.1"

int main(int argc, char** argv)
{
	TCP_Client Client;
	int result = 0;

	//initialize winsock
	result = Client.initialize(SERVERIP, PORT);
	if (result != 0)
	{
		return result;
	}



	//write


	//read



	//shutdown winsock
	Client.CloseSocket();

	
	return 0;
}