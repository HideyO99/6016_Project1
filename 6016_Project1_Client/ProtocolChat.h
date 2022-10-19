#pragma once
#include <string>

class ProtocolChat
{
public:
	ProtocolChat();
	~ProtocolChat();

	uint32_t frameSize();
	uint16_t cmd;
	uint16_t opcode;
	uint32_t contentSize();
	std::string content;

private:

};


