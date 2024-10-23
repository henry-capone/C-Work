#include "Room.hpp"
#include "GameObject.hpp"
#include "GameManager.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

Room::Room(const std::string name) {
	this->name = name;			// set the room name
}

void Room::AddItem(const GameObject& item) {

	std::shared_ptr<GameObject> newItem = item.createCopy(); // create new smart pointer
	newItem->setNameDesc(item.getName()); // set name and description to initial reference

	if (newItem != nullptr) {	// check the new smart pointer points to valid object
		this->items.push_back(newItem); // add to a list of items for this room
	}
	else {						// return error otherwise
		std::cout << "Error adding the item to the room" << std::endl;
	}
}

void Room::RemoveItem(const GameObject& item) {
	this->items.erase(std::remove_if(this->items.begin(), this->items.end(),
		[&item](const std::shared_ptr<GameObject>& ptr) {
			return ptr->getName() == item.getName();
		}),
		this->items.end()
	);
}

void Room::AddExit(const std::string direction, Room* exit) {
	exits.insert({ direction, exit }); // add exit to current room
}

std::string Room::GetDescription() {
	return this->name;			// return room name
}

std::vector<std::shared_ptr<GameObject>> Room::GetItems() {
	std::vector<std::shared_ptr<GameObject>> x = items; // create vector reference
	return x;					// return rooms' items
}

Room* Room::GetExit(std::string direction) {
	// set varied user inputs to the same direction
	if (direction == "n" || direction == "N" || direction=="North" || direction == "north") { direction = "North"; }
	else if (direction == "e" || direction == "E" || direction == "East" || direction == "east") { direction = "East"; }
	else if (direction == "s" || direction == "S" || direction == "South" || direction == "south") { direction = "South"; }
	else if (direction == "w" || direction == "W" || direction == "West" || direction == "west") { direction = "West"; }

	for (auto i=exits.begin();i != exits.end();i++) { // loop through rooms exits list
		if (direction == i->first) { // if direction exits in passed direction
			return i->second;	// return pointer to that room
		}
	}
	return nullptr;				// otherwise pass nullptr
}

std::map<std::string, Room*> Room::GetExits() {
	return this->exits;
}

std::shared_ptr<Door> Room::GetDoor() {
	auto GameManager = Manager::getInstance();
	auto doorList = GameManager->doorList;
	for (auto i = doorList.begin(); i != doorList.end(); i++) {
		if ( this->doorID == i->first) {
			return std::dynamic_pointer_cast<Door>(i->second);
		}
	}
	return nullptr;
}