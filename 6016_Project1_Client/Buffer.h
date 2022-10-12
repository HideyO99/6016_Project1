#pragma once
#include <vector>

class Buffer
{
public:
	Buffer(size_t size);
	~Buffer();
	
	void WriteUInt32LE(int32_t value);
	void WriteUInt32LE(std::size_t index, int32_t value);
	uint32_t ReadUInt32LE();
	uint32_t ReadUInt32LE(std::size_t index);


private:
	std::vector<uint8_t> m_buffer;
	int m_WriteIndex;
	int m_ReadIndex;
};

