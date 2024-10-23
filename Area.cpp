#include "Area.hpp"
#include "Room.hpp"
#include "GameManager.hpp"
#include <iostream>
#include <fstream>
#include <string>

void Area::AddRoom(const std::string& name, Room* room) {
	this->rooms.insert({ name,room });	// insert parameters as room in rooms list
}

Room* Area::GetRoom(const std::string& name) {
	auto it = rooms.find(name);			// find passed name in rooms list
	if (it != rooms.end()) {			// check "it" is in rooms list
		Room* x = it->second;			// create room class pointer to desired room
		return x;						// return room pointer
	}
	else {								// return null otherwise
		return nullptr;
	}
}

void Area::ConnectRooms(const std::string& direction, Room* room) {
	this->exits.insert({ direction, room }); // insert exit into exit list
}

void Area::LoadMapFromFile(const std::string& filename) {
	auto GameManager = Manager::getInstance();
	std::ifstream map(filename);		// read from file passed
	std::string name, temp, rest, direction, destination, doorID, doorName;
	int pos, pos2;
	bool flag = false;
	while (getline(map, temp)) {		// loop through lines inside file

		pos = temp.find("|");
		name = temp.substr(0, pos - 1);
		// loop through rooms to check if room already exists in rooms list
		for (auto i = rooms.begin();i != rooms.end();i++) {
			if (i->first == name) {		
				flag = true;
			}
		}
		// if it does not, create room then add to rooms list
		if (!flag) {
			auto room = new Room(name);
			this->AddRoom(name, room);
		}
		flag = false;
		rest = temp.substr(pos + 2);
		pos2 = rest.find("|");
		direction = rest.substr(0, pos2 - 1);

		rest = rest.substr(pos2 + 2);
		pos = rest.find("|");
		destination = rest.substr(0, pos - 1);

		try {
			rest = rest.substr(pos + 2);
			pos = rest.find("|");
			doorID = rest.substr(0, pos - 1);
			doorName = rest.substr(pos + 2);
		} catch (...) {}

		// loop through rooms to check if exit destination already exists, if so add to exit list
		for (auto i = rooms.begin();i != rooms.end();i++) {
			if (i->first == destination) {
				auto destinationAddress = i->second;
				auto it = rooms.find(name);
				if (it != rooms.end()) {
					it->second->AddExit(direction, destinationAddress);
					flag = true;
				}
			}
		}
		// otherwise create destination room and add to exit list
		if (!flag) {
			auto room = new Room(destination);
			this->AddRoom(destination, room);
			for (auto i = rooms.begin();i != rooms.end();i++) {
				if (i->first == destination) {
					auto destinationAddress = i->second;
					auto it = rooms.find(name);
					if (it != rooms.end()) {
						it->second->AddExit(direction, destinationAddress);
					}
				}
			}
		}
		flag = false;
		// check if doorID already exists
		for (auto i = GameManager->doorList.begin(); i != GameManager->doorList.end(); i++) {
			if (stoi(doorID) == i->first) {
				flag = true;
			}
		}
		// if doorID doesn't already exist create door and add it to room and doorList
		if (!flag) {
			Door door;
			door.setNameDesc(doorName);
			if (doorID == "1") { door.OpenDoor(); }
			GameManager->doorList.insert({ stoi(doorID),std::make_shared<Door>(door)});
			GameManager->world.GetRoom(name)->AddDoor(stoi(doorID));
		}
		else { GameManager->world.GetRoom(name)->AddDoor(0); }
		flag = false;
	}
	map.close();						// close file once done
}

std::map<std::string, Room*> Area::GetRoomMap() {
	return this->rooms;					// return room map
}