#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "GameObject.hpp"

class Room {
private:
	std::string name;
	std::vector<std::shared_ptr<GameObject>> items;
	std::map<std::string, Room*> exits;
	int doorID;
public:
	Room(const std::string name);
	void AddItem(const GameObject& item);
	void RemoveItem(const GameObject& item);
	void AddExit(const std::string direction, Room* exit);
	std::string GetDescription();
	std::vector<std::shared_ptr<GameObject>> GetItems();
	Room* GetExit(std::string direction);
	std::map<std::string, Room*> GetExits();
	void AddDoor(int ID) { this->doorID = ID; }
	std::shared_ptr<Door> GetDoor();
	int getDoorID() { return this->doorID; }
};

