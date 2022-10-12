#include "TCP_Client.h"

int main(int argc, char** argv)
{
	TCP_Client Client;
	int result = 0;

	//initialize winsock
	result = Client.WinsockInit();
	if (result != 0);
	{
		return result;
	}

	//socket creation
	result = Client.SocketCreate();
	if (result != 0);
	{
		return result;
	}

	//connect
	result = Client.MakeConnect();
	if (result != 0);
	{
		return result;
	}

	//write


	//read


	//close
	result = Client.CloseConnection();
	if (result != 0);
	{
		return result;
	}

	//shutdown winsock
	Client.ShutdownWinsock();

	
	return 0;
}