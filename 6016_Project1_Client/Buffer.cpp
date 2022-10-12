#include "Buffer.h"

Buffer::Buffer(size_t size)
{
	m_buffer.resize(size);
	m_ReadIndex = 0;
	m_WriteIndex = 0;
}

Buffer::~Buffer()
{
}

void Buffer::WriteUInt32LE(int32_t value)
{
	m_buffer[m_WriteIndex++] = value;
	m_buffer[m_WriteIndex++] = value >> 8;
	m_buffer[m_WriteIndex++] = value >> 16;
	m_buffer[m_WriteIndex++] = value >> 24;
}

void Buffer::WriteUInt32LE(std::size_t index, int32_t value)
{
	m_buffer[index] = value;
	m_buffer[index + 1] = value >> 8;
	m_buffer[index + 2] = value >> 16;
	m_buffer[index + 3] = value >> 24;
}

uint32_t Buffer::ReadUInt32LE()
{
	uint32_t ReadBuffer = m_buffer[m_ReadIndex++];
	ReadBuffer += m_buffer[m_ReadIndex++] << 8;
	ReadBuffer += m_buffer[m_ReadIndex++] << 16;
	ReadBuffer += m_buffer[m_ReadIndex++] << 24;
	return ReadBuffer;
}

uint32_t Buffer::ReadUInt32LE(std::size_t index)
{
	uint32_t ReadBuffer = m_buffer[index];
	ReadBuffer += m_buffer[index + 1] << 8;
	ReadBuffer += m_buffer[index + 2] << 16;
	ReadBuffer += m_buffer[index + 3] << 24;
	return 0;
}
