#pragma once
#include <vector>
#include <string>

class Buffer
{
public:
	Buffer(size_t size);
	~Buffer();

	//write
	void WriteInt32LE(int32_t value);
	void WriteInt32LE(std::size_t index, int32_t value);
	void WriteInt32BE(int32_t value);
	void WriteInt32BE(std::size_t index, int32_t value);
	void WriteShort16LE(int16_t value);
	void WriteShort16LE(std::size_t index, int16_t value);
	void WriteShort16BE(int16_t value);
	void WriteShort16BE(std::size_t index, int16_t value);
	void WriteString(std::string s);
	void WriteString(std::size_t index, std::string s);

	//read
	int32_t ReadInt32LE();
	int32_t ReadInt32LE(std::size_t index);
	int32_t ReadInt32BE();
	int32_t ReadInt32BE(std::size_t index);
	int16_t ReadShort16LE();
	int16_t ReadShort16LE(std::size_t index);
	int16_t ReadShort16BE();
	int16_t ReadShort16BE(std::size_t index);
	std::string ReadString();
	std::string ReadString(std::size_t index, uint32_t len);



//private:
	std::vector<int8_t> m_buffer;
	int m_WriteIndex;
	int m_ReadIndex;
};

