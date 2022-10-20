#include "TCP_Server.h"
#include "ProtocolChat.h"
#include "Buffer.h"

room room1(Network);
room room2(Physic);
room room3(Deploy);

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

int TCP_Server::Initialize(PCSTR port)
{
	int result = 0;

	//initialize winsock
	result = WinsockInit(port);
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

void TCP_Server::JoinRoom(uint16_t roomID, std::string user)
{
	switch (roomID)
	{
	case Network:
		room1.join(user);
		break;
	case Physic:
		room2.join(user);
	case Deploy:
		room3.join(user);
	default:
		break;
	}
}

void TCP_Server::LeaveRoom(uint16_t roomID, std::string user)
{
	switch (roomID)
	{
	case Network:
		room1.leave(user);
		break;
	case Physic:
		room2.leave(user);
	case Deploy:
		room3.leave(user);
	default:
		break;
	}
}

int TCP_Server::WinsockInit(PCSTR port)
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
	hints.ai_flags = AI_PASSIVE;



	result = getaddrinfo(NULL, port, &hints, &info);
	if (result != 0)
	{
		printf("- error getaddrinfo %#x\n", result);
		WSACleanup();
		return 1;
	}

	return result;
}

int TCP_Server::SocketCreate()
{
	int result = 0;


	ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("- error create socket %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		WSACleanup();
		return 1;
	}

	return result;
}

int TCP_Server::BindSocket()
{
	int result = 0;

	result = bind(ListenSocket, info->ai_addr, (int)info->ai_addrlen);
	if (result == SOCKET_ERROR)
	{
		printf("- bind error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	return result;
}

int TCP_Server::ListenConnection()
{
	int result = 0;

	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		printf("- listen error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	return result;
}

int TCP_Server::AcceptConnection()
{
	int result = 0;

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		printf("- accept error %#x\n", WSAGetLastError());
		freeaddrinfo(info);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	return result;
}

int TCP_Server::ReadFromClient()
{
	int result = 0;
	ProtocolChat frame;
	const int rcvBuffLen = 512;
	char rcvBuff[rcvBuffLen];
	Buffer buff(512);

	int recvResult = recv(ClientSocket, rcvBuff, rcvBuffLen, 0);
	//buff.m_buffer.push_back((int8_t)rcvBuff[0]);
	buff.m_buffer.insert(buff.m_buffer.begin(), rcvBuff, rcvBuff + rcvBuffLen);
	//buff.m_buffer.assign(rcvBuff,);
	uint32_t buffLen = buff.ReadInt32BE(0);
	buff.m_buffer.resize(buffLen+1);
	uint16_t cmd = buff.ReadShort16BE(4);
	uint16_t opcode = buff.ReadShort16BE(6);
	uint32_t contentLen = buff.ReadInt32BE(8);
	std::string s = buff.ReadString(12,contentLen);

	if (recvResult == (int)buff.ReadInt32BE(0))
	{
		switch (cmd)
		{
		case 0:
			SendToClient(3, 0,"Hello " + s);
			break;
		case 1:
			JoinRoom(opcode, s);
			switch (opcode)
			{
			case 1:
				s.append(" has joined the Network room");
				break;
			case 2:
				s.append(" has joined the Physic room");
				break;
			case 3:
				s.append(" has joined the Deploy room");
				break;
			default:
				break;
			}
			SendToClient(5, BroadCast, s);
			break;
		case 2:
			LeaveRoom(opcode, s);
			switch (opcode)
			{
			case 1:
				s.append(" has left the Network room");
				break;
			case 2:
				s.append(" has left the Physic room");
				break;
			case 3:
				s.append(" has left the Deploy room");
				break;
			default:
				break;
			}
			SendToClient(6, BroadCast, s);
			break;
		case 4:
			SendToClient(3, opcode, s);
			break;
		default:
			break;
		}
	}
	return result;
}

int TCP_Server::SendToClient(uint16_t cmd, uint16_t opcode, std::string s)
{
	int result = 0;
	//Buffer buffer(8);
	int packetLen = 0;
	ProtocolChat frame;

	frame.cmd = cmd; // send to client
	frame.opcode = opcode; //room
	frame.content = s;
	Buffer buffer(frame.frameSize());
	buffer.WriteInt32BE(frame.frameSize());
	buffer.WriteShort16BE(frame.cmd);
	buffer.WriteShort16BE(frame.opcode);
	buffer.WriteInt32BE(frame.contentSize());
	buffer.WriteString(frame.content);

	result = send(ClientSocket, (const char*)&(buffer.m_buffer[0]), frame.frameSize(), 0);
	//if (opcode == BroadCast) //boardcast send message to every room
	//{
	//	frame.opcode = Network;
	//	buffer.WriteShort16BE(6, frame.opcode);
	//	result = send(ClientSocket, (const char*)&(buffer.m_buffer[0]), frame.frameSize(), 0);
	//	frame.opcode = Physic;
	//	buffer.WriteShort16BE(6, frame.opcode);
	//	result = send(ClientSocket, (const char*)&(buffer.m_buffer[0]), frame.frameSize(), 0);
	//	frame.opcode = Deploy;
	//	buffer.WriteShort16BE(6, frame.opcode);
	//	result = send(ClientSocket, (const char*)&(buffer.m_buffer[0]), frame.frameSize(), 0);
	//}

	return result;
}

void TCP_Server::CloseSocket()
{

	freeaddrinfo(info);
	closesocket(ListenSocket);
	closesocket(ClientSocket);

	WSACleanup();
}
