#pragma once
#include <cstdint>
class RoomStatus
{
public:
	RoomStatus();
	~RoomStatus();

	bool r1Status;
	bool r2Status;
	bool r3Status;

	void setActiveRoom(uint16_t r, bool val);
	bool getActiveRoom(uint16_t r);
};

