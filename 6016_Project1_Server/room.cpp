#include "room.h"


room::room(int id)
{
	roomID = id;
	member.clear();

}

room::~room()
{
}

void room::join(std::string s)
{
	member.push_back(s);
}

void room::leave(std::string s)
{
	member.remove(s);
}




