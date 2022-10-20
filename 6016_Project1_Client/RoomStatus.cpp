#include "RoomStatus.h"

RoomStatus::RoomStatus()
{
	r1Status = false;
	r2Status = false;
	r3Status = false;
}

RoomStatus::~RoomStatus()
{
}

void RoomStatus::setActiveRoom(uint16_t r, bool val)
{
	switch (r)
	{
	case 1:
		r1Status = val;
		break;
	case 2:
		r2Status = val;
		break;
	case 3:
		r3Status = val;
		break;
	default:
		break;
	}
}

bool RoomStatus::getActiveRoom(uint16_t r)
{
	bool result = false;
	switch (r)
	{
	case 1:
		if(r1Status) result = true;
		break;
	case 2:
		if(r2Status) result = true;
		break;
	case 3:
		if(r3Status) result = true;
		break;
	default:
		break;
	}
	return result;
}
