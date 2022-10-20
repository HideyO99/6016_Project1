#pragma once
#include <string>
//#include <vector>
//#include <map>
#include <list>
enum roomID
{
	Network = 1,
	Physic,
	Deploy,
	BroadCast = 0xff

};

class room
{
public:
	room(int id);
	~room();

	int roomID;
	std::list<std::string> member;
	void join(std::string s);
	void leave(std::string s);

};

