#include "ProtocolChat.h"

ProtocolChat::ProtocolChat()
{
	cmd = 0xf;
	opcode = 0xf;
	content = '\0';
}

ProtocolChat::~ProtocolChat()
{
}

uint32_t ProtocolChat::frameSize()
{

	return sizeof(cmd) + sizeof(opcode) + contentSize();
}

uint32_t ProtocolChat::contentSize()
{
	return (uint32_t)content.size();
}
