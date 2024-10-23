#pragma once
#include <iostream>
#include "Room.hpp"

class Area
{
private:
	std::map<std::string, Room*> exits;
	std::map<std::string, Room*> rooms;
public:
	void AddRoom(const std::string& name, Room* room);
	Room* GetRoom(const std::string& name);
	std::map<std::string, Room*> GetRoomMap();
	void ConnectRooms(const std::string&, Room*);
	void LoadMapFromFile(const std::string& filename);
};

