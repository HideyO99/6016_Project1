#include "TCP_Server.h"
#include "room.h"



int main(int argc, char** argv)
{
	TCP_Server Server;
	int result = 0;

	//initialize winsock
	result = Server.Initialize(PORT);
	if (result != 0)
	{
		return result;
	}
	

	//read
	result = Server.ReadFromClient();
	if (result != 0)
	{
		return result;
	}

	//write
	//result = Server.SendToClient();
	if (result != 0)
	{
		return result;
	}

	//close socket
	Server.CloseSocket();


	return 0;
}