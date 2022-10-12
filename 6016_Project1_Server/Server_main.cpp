#include "TCP_Server.h"

int main(int argc, char** argv)
{
	TCP_Server Server;
	int result = 0;

	//initialize winsock
	result = Server.WinsockInit();
	if (result != 0)
	{
		return result;
	}
	
	//socket creation
	result = Server.SocketCreate();
	if (result != 0)
	{
		return result;
	}

	//bind
	result = Server.BindSocket();
	if (result != 0)
	{
		return result;
	}
	//listen
	result = Server.ListenConnection();
	if (result != 0)
	{
		return result;
	}
	 
	//accept 
	result = Server.AcceptConnection();
	if (result != 0)
	{
		return result;
	}

	//read


	//write


	//close socket
	Server.CloseSocket();

	//shutdown winsock
	Server.ShutdownWinsock();

	return 0;
}