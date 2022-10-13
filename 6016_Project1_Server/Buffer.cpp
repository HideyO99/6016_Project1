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

void Buffer::WriteInt32LE(int32_t value)
{
	m_buffer[m_WriteIndex++] = value >> 0;
	m_buffer[m_WriteIndex++] = value >> 8;
	m_buffer[m_WriteIndex++] = value >> 16;
	m_buffer[m_WriteIndex++] = value >> 24;
}

void Buffer::WriteInt32LE(std::size_t index, int32_t value)
{
	m_buffer[index] = value >> 0;
	m_buffer[index + 1] = value >> 8;
	m_buffer[index + 2] = value >> 16;
	m_buffer[index + 3] = value >> 24;
}

void Buffer::WriteInt32BE(int32_t value)
{
	m_buffer[m_WriteIndex++] = value >> 24;
	m_buffer[m_WriteIndex++] = value >> 16;
	m_buffer[m_WriteIndex++] = value >> 8;
	m_buffer[m_WriteIndex++] = value >> 0;
}

void Buffer::WriteInt32BE(std::size_t index, int32_t value)
{
	m_buffer[index] = value >> 24;
	m_buffer[index + 1] = value >> 16;
	m_buffer[index + 2] = value >> 8;
	m_buffer[index + 3] = value >> 0;
}

void Buffer::WriteShort16LE(int16_t value)
{
	m_buffer[m_WriteIndex++] = value >> 0;
	m_buffer[m_WriteIndex++] = value >> 8;
}

void Buffer::WriteShort16LE(std::size_t index, int16_t value)
{
	m_buffer[index] = value >> 0;
	m_buffer[index + 1] = value >> 8;
}

void Buffer::WriteShort16BE(int16_t value)
{
	m_buffer[m_WriteIndex++] = value >> 8;
	m_buffer[m_WriteIndex++] = value >> 0;
}

void Buffer::WriteShort16BE(std::size_t index, int16_t value)
{
	m_buffer[index] = value >> 8;
	m_buffer[index + 1] = value >> 0;
}

void Buffer::WriteString(std::string s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		m_buffer[m_WriteIndex + i] = s[i];
	}
	m_WriteIndex += (int)s.size();
}

void Buffer::WriteString(std::size_t index, std::string s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		m_buffer[index + i] = s[i];
	}
}

int32_t Buffer::ReadInt32LE()
{
	int32_t ReadBuffer = m_buffer[m_ReadIndex++];
	ReadBuffer += m_buffer[m_ReadIndex++] << 8;
	ReadBuffer += m_buffer[m_ReadIndex++] << 16;
	ReadBuffer += m_buffer[m_ReadIndex++] << 24;
	return ReadBuffer;
}

int32_t Buffer::ReadInt32LE(std::size_t index)
{
	int32_t ReadBuffer = m_buffer[index];
	ReadBuffer += m_buffer[index + 1] << 8;
	ReadBuffer += m_buffer[index + 2] << 16;
	ReadBuffer += m_buffer[index + 3] << 24;
	return ReadBuffer;
}

int32_t Buffer::ReadInt32BE()
{
	int32_t ReadBuffer = m_buffer[m_ReadIndex++] << 24;
	ReadBuffer += m_buffer[m_ReadIndex++] << 16;
	ReadBuffer += m_buffer[m_ReadIndex++] << 8;
	ReadBuffer += m_buffer[m_ReadIndex++];
	return ReadBuffer;
}

int32_t Buffer::ReadInt32BE(std::size_t index)
{
	int32_t ReadBuffer = m_buffer[index] << 24;
	ReadBuffer += m_buffer[index + 1] << 16;
	ReadBuffer += m_buffer[index + 2] << 8;
	ReadBuffer += m_buffer[index + 3];
	return ReadBuffer;
}

int16_t Buffer::ReadShort16LE()
{
	int16_t ReadBuffer = m_buffer[m_ReadIndex++];
	ReadBuffer += m_buffer[m_ReadIndex++] << 8;
	return ReadBuffer;
}

int16_t Buffer::ReadShort16LE(std::size_t index)
{
	int16_t ReadBuffer = m_buffer[index];
	ReadBuffer += m_buffer[index + 1] << 8;
	return ReadBuffer;
}

int16_t Buffer::ReadShort16BE()
{
	int16_t ReadBuffer = m_buffer[m_ReadIndex++] << 8;
	ReadBuffer += m_buffer[m_ReadIndex++];
	return ReadBuffer;
}

int16_t Buffer::ReadShort16BE(std::size_t index)
{
	int16_t ReadBuffer = m_buffer[index] << 8;
	ReadBuffer += m_buffer[index + 1];
	return ReadBuffer;
}

std::string Buffer::ReadString()
{
	std::string s;

	while (m_buffer[m_ReadIndex] != '\0')
	{
		s.push_back(m_buffer[m_ReadIndex]);
		m_ReadIndex++;
	}

	return s;
}

std::string Buffer::ReadString(std::size_t index)
{
	std::string s;
	int i = 0;

	while (m_buffer[index + i] != '\0')
	{
		s.push_back(m_buffer[index + i]);
		i++;
	}

	return s;
}