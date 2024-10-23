#include "Character.hpp"
#include "Room.hpp"
#include "GameManager.hpp"
#include <iostream>
#include <string>
#include <time.h>

// default constructors
Character::Character() {}
Player::Player() {}
NPC::NPC() {}

Character::Character(const std::string& name, int health, int strength) { // setting private members as passed data
	this->name = name;
	this->health = health;
	this->strength = strength;
}

Player::Player(const std::string& name, int health, int strengh) { // setting private members as passed data
	this->name = name;
	this->health = health;
	this->strength = strengh;
}

NPC::NPC(const std::string& name, int health, int strengh) { // setting private members as passed data
	this->name = name;
	this->health = health;
	this->strength = strengh;
}


void Player::SetLocation(Room* location) {
	this->location = location;					// setting private member location
}

Room* Player::GetLocation() {					// returning private member current location
	return this->location;
}

void NPC::SetLocation(Room* location) {			// setting private member location
	this->location = location;
}

Room* NPC::GetLocation() {						// returning private member current location
	return this->location;
}

void Player::AddToInv(const GameObject& item) { // add item to player inventory
	std::shared_ptr<GameObject> newItem;

	if (typeid(item) == typeid(Knife)) { newItem = std::make_shared<Knife>(); newItem->setNameDesc(item.getName()); }
	else if (typeid(item) == typeid(Handcuff)) { newItem = std::make_shared<Handcuff>(); newItem->setNameDesc(item.getName());}
	else if (typeid(item) == typeid(Keycard)) { newItem = std::make_shared<Keycard>(); newItem->setNameDesc(item.getName());}
	else if (typeid(item) == typeid(Food)) { newItem = std::make_shared<Food>(); newItem->setNameDesc(item.getName());}
	else if (typeid(item) == typeid(Weights)) { newItem = std::make_shared<Weights>(); newItem->setNameDesc(item.getName());}
	else {
		std::cout << "Error adding the item to the inventory" << std::endl;
	}

	this->inventory.push_back(newItem);
	this->location->RemoveItem(item);
}

void NPC::AddToInv(const GameObject& item) { // add item to NPC inventory
	std::shared_ptr<GameObject> newItem;

	if (typeid(item) == typeid(Knife)) { newItem = std::make_shared<Knife>(); newItem->setNameDesc(item.getName()); }
	else if (typeid(item) == typeid(Handcuff)) { newItem = std::make_shared<Handcuff>(); newItem->setNameDesc(item.getName()); }
	else if (typeid(item) == typeid(Keycard)) { newItem = std::make_shared<Keycard>(); newItem->setNameDesc(item.getName()); }
	else if (typeid(item) == typeid(Food)) { newItem = std::make_shared<Food>(); newItem->setNameDesc(item.getName()); }
	else if (typeid(item) == typeid(Weights)) { newItem = std::make_shared<Weights>(); newItem->setNameDesc(item.getName()); }
	else {
		std::cout << "Error adding the item to the inventory" << std::endl;
	}

	this->inventory.push_back(newItem);
	this->location->RemoveItem(item);
}

void Player::fight(NPC& enemy) {			// fight between player and warden
	auto GameManager = Manager::getInstance();
	if (this->GetLocation() == GameManager->warden.GetLocation()) {
				
		int additionalStr = 0;				// modifies player strength if they have a weapon
		auto inv = GameManager->player.GetInv();
		for (auto i = inv.begin(); i != inv.end(); i++) {
			if (typeid(i) == typeid(Knife)) {
				additionalStr = 20;
			}
		}

		int total = this->strength + enemy.GetStr() + additionalStr; // random number based fighting
		srand(time(NULL));
		int randNum = rand() % total + 1;
		if (randNum > this->strength) {
			this->health -= 20;
			this->SetLocation(GameManager->world.GetRoom("Personal Cell"));
			std::cout << "The warden has knocked you out! You'll be locked back in your cell" << std::endl;
		}
		else {
			enemy.SetHP(enemy.GetHP() - 20);
			std::cout << "The warden has been knocked out! You can now take the keycard from him." << std::endl;
			enemy.setStatus(0);
		}
	}
	else {
		std::cout << "You must be in the same room as the warden to start a fight!" << std::endl;
	}
}

std::string Player::GetStats() {	// returns player stats
	std::string stats = std::to_string(this->health) + "-" + std::to_string(this->strength);
	return stats;
}

void Player::SetStats(int health, int strength) { // sets player stats
	this->health = health;
	this->strength = strength;
}

std::string NPC::GetStats() {		// returns NPC stats
	std::string stats = std::to_string(this->health) + "-" + std::to_string(this->strength);
	return stats;
}

void NPC::SetStats(int health, int strength) { // sets NPC stats
	this->health = health;
	this->strength = strength;
}

std::vector<std::shared_ptr<GameObject>> Player::GetInv() {
	// return the inventory vector
	return this->inventory;
}

std::vector<std::shared_ptr<GameObject>> NPC::GetInv() {
	// return the inventory vector
	return this->inventory;
}

void Player::removeItem(GameObject& item) { // removes item from player's inventory
	this->inventory.erase(std::remove_if(this->inventory.begin(), this->inventory.end(),
		[&item](const std::shared_ptr<GameObject>& ptr) {
			return ptr->getName() == item.getName();
		}),
		this->inventory.end()
	);
}

void NPC::removeItem(GameObject& item) { // removes item from NPC's inventory
	this->inventory.erase(std::remove_if(this->inventory.begin(), this->inventory.end(),
		[&item](const std::shared_ptr<GameObject>& ptr) {
			return ptr->getName() == item.getName();
		}),
		this->inventory.end()
	);
}

void CommandInterpreter::interpretCommand(const std::string& command) { // determines what to do from player input

	// checks whether command is multiple strung together
	bool dblCmd = false;
	if (command.find("&") < command.length()) {
		dblCmd = true;
	}
	auto GameManager = Manager::getInstance();
	int pos = command.find(" ");
	std::string argument = command.substr(0, pos);
	std::string variable = command.substr(pos + 1);
	if (dblCmd) { int pos2 = variable.find("&"); variable = variable.substr(0, pos2 - 1); }
	bool validCommand = false;

	if (argument == "Look") {			// if player input is 'Look'
		this->player_->lookA();
		validCommand = true;
		bool msgres = false;
		std::cout << "Items nearby:" << std::endl; // print out all the items in the room
		for (auto element : this->player_->GetLocation()->GetItems()) {
			std::cout << "- " << element->getName() << std::endl;
		}
		if (this->player_->GetLocation() == GameManager->warden.GetLocation()) { // if player is in same room as warden
			if (GameManager->warden.getStatus() == 0) { // if knocked out print NPC inventory to steal from
				std::cout << "The warden is knocked out on the floor - you can steal:" << std::endl;
				auto wardenInv = GameManager->warden.GetInv();
				for (auto j = wardenInv.begin(); j != wardenInv.end(); j++) {
					auto& x = *j;
					std::cout << "- " << x->getName() << std::endl;
				}
				msgres = true;
			}
			else if (GameManager->warden.getStatus() == 2) { // if handcuffed print NPC inventory to steal from
				std::cout << "The warden is handcuffed on the floor - you can steal:" << std::endl;
				auto wardenInv = GameManager->warden.GetInv();
				for (auto j = wardenInv.begin(); j != wardenInv.end(); j++) {
					auto& x = *j;
					std::cout << "- " << x->getName() << std::endl;
				}
				msgres = true;
			}
		}
		auto wardenNear = GameManager->warden.GetLocation()->GetExits();
		for (auto i = wardenNear.begin(); i != wardenNear.end(); i++) { // if warden's location has exit to player's location
			if (this->player_->GetLocation() == i->second) {
				if (!msgres) {
					std::cout << "You hear the warden's footsteps in a room nearby!" << std::endl;
					msgres = true;
				}
			}
		}
		if (!msgres) { 
			if (this->player_->GetLocation() == GameManager->warden.GetLocation()) { // if warden's location is same as player's
				std::cout << "The warden is in the same room as you!" << std::endl;
			} else std::cout << "The warden is not nearby" << std::endl; // if warden is not near player
		}
	}
	if (argument == "Move") {		// if player input is 'Move'
			this->player_->move();
			if (GameManager->world.GetRoom(variable) != nullptr) { // if player input is a room
				// if that room doesn't have a door or has an open door
				if (GameManager->world.GetRoom(variable)->GetDoor() == nullptr || GameManager->world.GetRoom(variable)->GetDoor()->GetDoorState()) {
					// if player location doesn't have a door or has an open door
					if (this->player_->GetLocation()->GetDoor() == nullptr || this->player_->GetLocation()->GetDoor()->GetDoorState()) {
						this->player_->SetLocation(GameManager->world.GetRoom(variable));
						std::cout << "moved to " << variable << std::endl;
					} else { std::cout << "tried to move to " << variable << " but the " << this->player_->GetLocation()->GetDoor()->getName() << " is locked blocking your path" << std::endl; }
				}
				else { std::cout << "tried to move to " << variable << " but the " << GameManager->world.GetRoom(variable)->GetDoor()->getName() << " is locked blocking your path" << std::endl; }
			}
			else {					// otherwise input is direction
				std::string direction = variable;
				// get direction
				if (direction == "n" || direction == "N" || direction == "North" || direction == "north") { direction = "North"; }
				else if (direction == "e" || direction == "E" || direction == "East" || direction == "east") { direction = "East"; }
				else if (direction == "s" || direction == "S" || direction == "South" || direction == "south") { direction = "South"; }
				else if (direction == "w" || direction == "W" || direction == "West" || direction == "west") { direction = "West"; }
				if (GameManager->player.GetLocation()->GetExit(direction) != nullptr) { // if player location has exit in that direction
					auto destination = GameManager->player.GetLocation()->GetExit(direction);
					// if that room doesn't have a door or has an open door
					if (destination->GetDoor() == nullptr || destination->GetDoor()->GetDoorState()) {
						// if player location doesn't have a door or has an open door
						if (this->player_->GetLocation()->GetDoor() == nullptr || this->player_->GetLocation()->GetDoor()->GetDoorState()) {
							std::cout << "moved to " << GameManager->player.GetLocation()->GetExit(direction)->GetDescription() << std::endl;
							this->player_->SetLocation(GameManager->player.GetLocation()->GetExit(direction));
						}
						else { std::cout << "tried to move to " << variable << " but the " << this->player_->GetLocation()->GetDoor()->getName() << " is locked blocking your path" << std::endl; }
					}
					else {
						std::cout << "tried to move to " << GameManager->player.GetLocation()->GetExit(direction)->GetDescription() << " but the " << GameManager->player.GetLocation()->GetExit(direction)->GetDoor()->getName() << " is locked blocking your path" << std::endl;
					}
				}
				else { std::cout << "You cannot go that way!" << std::endl; }
			}
			validCommand = true;
		}
	if (argument == "Take") {			// if player input is 'Take'
			this->player_->pickup();
			bool msgres = false;
			for (std::shared_ptr<GameObject> item : this->player_->GetLocation()->GetItems()) { // check if item is in room
				int pos = item->getName().find("-");
				std::string itemName = item->getName().substr(0, pos - 1);
				if (itemName == variable) {
					GameObject& itemRef = *item;
					this->player_->AddToInv(itemRef);
					this->player_->GetLocation()->RemoveItem(itemRef);
					msgres = true;
				}
			}
			// if warden is knocked out or handcuffed check if item is in inventory
			if (GameManager->warden.getStatus() != 1 && GameManager->warden.GetLocation() == this->player_->GetLocation()) {
				for (auto element : GameManager->warden.GetInv()) {
					int pos = element->getName().find("-");
					std::string elementName = element->getName().substr(0, pos - 1);
					if (elementName == variable) {
						GameObject& elementRef = *element;
						this->player_->AddToInv(elementRef);
						GameManager->warden.removeItem(elementRef);
						msgres = true;
					}
				}
			}
			// if no item is found then display error
			if (!msgres) { std::cout << "Invalid Item" << std::endl; }
			validCommand = true;
		}
	if (argument == "Use") {		// if user input is 'Use'
		this->player_->interact();
		std::cout << variable << std::endl;
		int pos2 = variable.find("on");
		if (pos2 != -1) {			// if use command has a target
			std::string variableFrom = variable.substr(0, pos2-1);
			std::string variableTo = variable.substr(pos2 + 3);
			if (variableTo == "warden" || variableTo == "Warden") { // if target is warden
				if (this->player_->GetLocation() == GameManager->warden.GetLocation()) { // check player is in same room as warden
					if (GameManager->player.GetInv().size() == 0) { // check has items in inventory
						std::cout << "There are no items in your inventory" << std::endl;
					}
					for (auto i : GameManager->player.GetInv()) { 
						std::string name = i->getName();
						int pos = name.find("-");
						name = name.substr(0, pos - 1);
						if (name == variableFrom) {
							GameObject& usedItem = *i;
							if (typeid(usedItem) == typeid(Knife)) { // if player uses knife on warden
								NPC& NPCRef = GameManager->warden;
								std::cout << "The Warden didn't like that! You've started a fight with the Warden!" << std::endl;
								this->player_->fight(NPCRef);
							}
							else if (typeid(usedItem) == typeid(Handcuff)) { // if warden uses handcuffs on warden
								std::cout << "You've restrained the Warden - Quick before he escapes" << std::endl;
								GameManager->warden.setStatus(2);
							}
						}
					}
				}
			}
			else if (variableTo == "door" || variableTo == "Door") { // otherwise check if target is door
				// if item used from is a key
				if (variableFrom == "keycard" || variableFrom == "Keycard" || variableFrom == "Exit Key") {
					bool msgres = false;
					if (this->player_->GetLocation()->GetDoor() != nullptr) { // if there is a door in player's location
						this->player_->GetLocation()->GetDoor()->OpenDoor();
					}
					else { // else door is attached to room next to player's location
						auto surrDoors = this->player_->GetLocation()->GetExits();
						for (auto i = surrDoors.begin(); i != surrDoors.end();i++) {
							// if room next to player's location has a door and it's closed
							if (i->second->GetDoor() != nullptr && !i->second->GetDoor()->GetDoorState()) {
								if (i->second->getDoorID()==3) { // check if door is main exit
									for (auto j : this->player_->GetInv()) {
										GameObject& jRef = *j;
										if (jRef.getName() == "Exit Key - The key to the main exit gate!") { // check player is using correct key
											i->second->GetDoor()->OpenDoor();
											std::cout << i->second->GetDoor()->getName() << " has been opened!" << std::endl;
											msgres = true;
										}
									}
									if (!msgres) std::cout << "But you require the exit key to do that!" << std::endl; msgres = true;
								}
								else {			// otherwise door is not main exit
									for (auto j : this->player_->GetInv()) {
										GameObject& jRef = *j;
										if (jRef.getName() == "Keycard - The guards stolen keycard") { // check player has a keycard
											i->second->GetDoor()->OpenDoor();
											std::cout << i->second->GetDoor()->getName() << " has been opened!" << std::endl;
											msgres = true;
										}
									}
									if (!msgres) std::cout << "But you require a keycard to do that!" << std::endl; msgres = true;
								}
							}
						}
						if (!msgres) std::cout << "But there is no door to interact with" << std::endl;
					}
				}
				else { std::cout << "But you cannot interact with the door that way" << std::endl; }
			}
		}
		else {				// otherwise if command doesn't have target
			if (GameManager->player.GetInv().size() == 0) { // check player has items in inventory
				std::cout << "But there are no items in your inventory" << std::endl;
			}
			bool msgres = false;
			for (auto j : GameManager->player.GetInv()) {
				std::string name = j->getName();
				int pos = name.find("-");
				name = name.substr(0, pos - 1);
				if (name == variable) {
					GameObject& it = *j;
					if (typeid(it) == typeid(Weights)) { // if player is attempting to use weights
						if (this->player_->getStr() != 10) { // check strength isn't at max strength
							this->player_->setStr(this->player_->getStr() + 1);
							std::cout << "You have increased your strength" << std::endl;
						}
						else { std::cout << "But you cannot get any stronger" << std::endl; }
						msgres = true;
					}
					else if (typeid(it) == typeid(Food)) { // if player is attempting to use food
						if (this->player_->getHP() != 100) { // check player isn't at max health
							if (this->player_->getHP() > 90) { // if player has over 90 health - add remaining to get to max
								this->player_->setHP(this->player_->getHP() + (100 - this->player_->getHP()));
								std::cout << "You have increased your health" << std::endl;
								this->player_->removeItem(it);
							}
							else { this->player_->setHP(this->player_->getHP() + 10); } // otherwise player has less than 90 - add 10
							std::cout << "You have increased your health" << std::endl;
							this->player_->removeItem(it);
							}
						}
						msgres = true;
					}
			}
			if (!msgres) { std::cout << "But the item is not in your inventory" << std::endl; }
		}
			validCommand = true;
		}
	if (argument == "Inv") { // if user input is 'Inv'
		std::cout << "Player Stats: HP-Strength = " << this->player_->GetStats() << std::endl;
		std::cout << "The time is: " << GameManager->time << ":00" << std::endl;
		std::cout << "Items in Inventory: " << std::endl;
		for (std::shared_ptr<GameObject> item : GameManager->player.GetInv()) {
			std::cout << "- " << item->getName() << std::endl;
		} std::cout << std::endl;
	}
	if (argument == "Attack") { // if user input is 'Attack'
		this->player_->fight(GameManager->warden);
	}
	if (argument == "Save") { // if user input is 'Save'
		if (GameManager->saveProgres() == 1)
			std::cout << "Save Successful" << std::endl;
		else std::cout << "Save Unsuccessful" << std::endl;
	}
	if (command.find("&") < command.length()) { // if there are multiple commands recursively call function
		pos = command.find("&");
		std::string rest = command.substr(pos+2);
		interpretCommand(rest);
		validCommand = true;
	}
	if (validCommand == false) { std::cout << "Invalid Input" << std::endl; } // if no input was called return error
}