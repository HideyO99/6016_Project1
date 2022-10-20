#include "TCP_Client.h"
#include <iostream>
#include <map>

std::map<std::string, int> room{ {"Network",1},{"Physic",2},{"Deploy",3} };
RoomStatus curRoom;
uint16_t roomFlag = 0;

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

	switch (cmd)
	{
	case 5: // join acknowledge
		curRoom.setActiveRoom(roomFlag, true);
		break;
	case 6: // leave acknowledge
		curRoom.setActiveRoom(roomFlag, false);
		break;
	case 3: // msg 
		switch (opcode)
		{
		case 1:
			std::cout << "[Network] ";
			break;
		case 2:
			std::cout << "[Physic] ";
			break;
		case 3:
			std::cout << "[Deploy] ";
			break;
		default:
			break;
		}
		break;
	case 0xff:
		break;
	default:
		break;
	}

	std::cout << s << std::endl;
	return result;
}

int TCP_Client::Chat(std::string user)
{
	int result = 0;
	bool terminate = false;
	std::string input;
	std::string cmd;
	
	//uint16_t opcode = 0xff;
	//std::string para;
	
	std::cout << "Type /J [room] to join a room" << std::endl;
	std::cout << "Type /L [room] to Leave a room" << std::endl;
	std::cout << "Type /S [message] to send message to a room" << std::endl;
	std::cout << "Type /X  to exit program" << std::endl;
	std::cout << "Room Available" << std::endl;
	for (auto it = room.begin();
		it != room.end(); it++)
	{
		std::cout <<"< " << it->first << "  >\t";
	}
	std::cout << std::endl;
	std::getline(std::cin, input);
	uint16_t opcode = 0xff;
	int i = 0;
	while (!terminate)
	{
		std::cout << i++ << " ";
		std::cout << user << "> ";
		std::getline(std::cin, input);
		size_t delimiterPos = input.find(' ');
		if (delimiterPos != std::string::npos)
		{
			cmd = input.substr(0, delimiterPos);
			input.erase(0, delimiterPos + 1);
			while (input.front() == ' ')
			{
				delimiterPos = input.find(' ');
				input.erase(0, delimiterPos + 1);
			}
			//join room
			if ((cmd.compare("/J") == 0) || (cmd.compare("/j") == 0))
			{
				auto found = room.find(input);
				if (found != room.end())
				{
					opcode = (uint16_t)(found->second);
					result = SendToServer(JoinRoom, opcode, user);
					roomFlag = opcode;
				}
				else
				{
					std::cout << "incorrect room" << std::endl;
				}
			}
			//leave room
			if ((cmd.compare("/L") == 0) || (cmd.compare("/l") == 0))
			{
				auto found = room.find(input);
				if (found != room.end())
				{
					opcode = (uint16_t)(found->second);
					if (curRoom.getActiveRoom(opcode))
					{
						result = SendToServer(LeaveRoom, opcode, user);
						roomFlag = opcode;
					}
					else {
						std::cout << "you are not in the room" << std::endl;
					}
				}
				else
				{
					std::cout << "incorrect room" << std::endl;
				}
			}
			//send message
			if ((cmd.compare("/S") == 0) || (cmd.compare("/s") == 0))
			{
				std::string toRoom;
				delimiterPos = input.find(' ');
				toRoom = (input.substr(0, delimiterPos));
				input.erase(0, delimiterPos + 1);
				auto found = room.find(toRoom);
				if (found != room.end())
				{
					opcode = (uint16_t)(found->second);
					result = SendToServer(SendMSG, opcode, input);
				}
				else
				{
					std::cout << "incorrect room" << std::endl;
				}
				
			}
			result = ReceiveFromServer();
			//system("Pause");
		}
		if ((input.compare("/X") == 0) || (input.compare("/x") == 0))
		{
			terminate = true;
			break;
		}
	}
	

	return result;
}

int TCP_Client::WinsockInit(PCSTR ip, PCSTR port)
{
	int result = 0;

	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		printf("- error WSAStartup %#x\n", result);
		return 1;
	}


	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo(ip, port, &hints, &info);
	if (result != 0)
	{
		printf("- error getaddrinfo %#x\n", result);
		WSACleanup();
		return 1;
	}


	return result;
}

int TCP_Client::SocketCreate()
{
	int result = 0;

	ConnectSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("- error create socket %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		return 1;
	}

	return result;
}

int TCP_Client::MakeConnect()
{
	int result = 0;

	result = connect(ConnectSocket, info->ai_addr, (int)info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("- error connecting %d\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}


	return result;
}

int TCP_Client::CloseConnection()
{
	int result = 0;

	
	result = shutdown(ConnectSocket, SD_SEND);
	if (result == SOCKET_ERROR)
	{
		printf("- error shutdown %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	return result;
}

void TCP_Client::ShutdownWinsock()
{
	closesocket(ConnectSocket);
	WSACleanup();
}
