#pragma once
#include "GameManager.hpp"
#include <iostream>
#include <string>

Manager* Manager::instance = nullptr;

Manager* Manager::getInstance() {
	if (!instance) {
		instance = new Manager();
	}
	return instance;
}

std::shared_ptr<GameObject> Manager::getItem(std::string name) {
	auto it = worldItems.find(name);
	if (it != worldItems.end()) {
		return it->second;
	}
	return nullptr;
}

const int Manager::saveProgres() {
	std::ofstream ProgressFile("Progress.txt");			// overwrite file - if doesn't exist, create then write to

	// write player stats
	ProgressFile << player.GetLocation()->GetDescription() << " | " << player.GetStats() << " | ";
	// write player inventory
	for (std::shared_ptr<GameObject> i : player.GetInv()) {

		GameObject& x = *i;
		if (typeid(x) == typeid(Knife)) { ProgressFile << "Shank | "; }
		else if (typeid(x) == typeid(Handcuff)) { ProgressFile << "Handcuffs | "; }
		else if (typeid(x) == typeid(Keycard)) { 
			if (x.getName() == "Exit Key - The key to the main exit gate!") { ProgressFile << "Exit Key | "; }
			else { ProgressFile << "Keycard | ";}
		}
		else if (typeid(x) == typeid(Food)) { ProgressFile << "Grub | "; }
		else if (typeid(x) == typeid(Weights)) { ProgressFile << "Dumbells | "; }

	}

	// write warden stats
	ProgressFile << this->warden.GetLocation()->GetDescription() << " | " << this->warden.GetStats() << " | ";
	// write warden status
	if (this->warden.getStatus() == 0) {
		ProgressFile << "0 | ";
	}
	else if (this->warden.getStatus() == 1) {
		ProgressFile << "1 | ";
	}
	else { ProgressFile << "2 | "; }
	// write warden inventory
	for (std::shared_ptr<GameObject> i : this->warden.GetInv()) {
		GameObject& x = *i;
		if (typeid(x) == typeid(Keycard)) { ProgressFile << "Keycard | "; }
	}

	// write game stats
	ProgressFile << this->time << " | " << this->gameLength << " | ";

	// write items in each room
	auto worldMap = this->world.GetRoomMap();
	for (auto i = worldMap.begin(); i != worldMap.end(); i++) {
		ProgressFile << i->first << " | ";
		for (std::shared_ptr<GameObject> j : i->second->GetItems()) {
			GameObject& x = *j;
			ProgressFile << x.getName() << " | ";
		}
	}
	ProgressFile.close();
	return 1;
}

void Manager::loadProgress() {
	std::ifstream report("Progress.txt");		// read from file
	std::string temp, section;
	int pos;
	while (std::getline(report, temp)) {
		// find and set player location
		pos = temp.find("|");
		section = temp.substr(0, pos - 1);
		Room* location = world.GetRoom(section);
		player.SetLocation(location);

		// find and set player stats
		temp = temp.substr(pos+2);
		pos = temp.find("-");
		std::string hp = temp.substr(0, pos);
		temp = temp.substr(pos + 1);
		pos = temp.find("|");
		std::string str = temp.substr(0, pos - 1);
		player.SetStats(std::stoi(hp), std::stoi(str));

		// find and set player inventory
		for (auto i = worldItems.begin(); i != worldItems.end(); i++) {
			try {
				temp = temp.substr(pos + 1);
				pos = temp.find("|");
				section = temp.substr(1, pos - 2);
				auto it = worldItems.find(section);
				if (it != worldItems.end()) {
					GameObject& x = *it->second;
					player.AddToInv(x);
				}
				else {
					break;
				}
			}
			catch (...) {
				break;
			}
		} std::cout << std::endl;

		// find and set warden location
		temp = temp.substr(1);
		pos = temp.find("|");
		section = temp.substr(0, pos - 1);
		this->warden.SetLocation(this->world.GetRoom(section));

		// find and set warden stats
		temp = temp.substr(pos + 2);
		pos = temp.find("-");
		std::string wardenhp = temp.substr(0, pos);
		temp = temp.substr(pos + 1);
		pos = temp.find("|");
		std::string wardenstr = temp.substr(0, pos - 1);
		this->warden.SetStats(std::stoi(hp), std::stoi(str));

		// find and set warden status
		pos = temp.find("|");
		temp = temp.substr(pos + 2);
		pos = temp.find("|");
		section = temp.substr(0, pos - 1);
		this->warden.setStatus(stoi(section));

		// find and set warden inventory
		for (auto i = this->worldItems.begin(); i != this->worldItems.end(); i++) {
			try {
				temp = temp.substr(pos + 1);
				pos = temp.find("|");
				section = temp.substr(1, pos - 2);
				auto it = this->worldItems.find(section);
				if (it != this->worldItems.end()) {
					GameObject& x = *it->second;
					this->warden.AddToInv(x);
				}
				else {
					break;
				}
			}
			catch (...) {
				break;
			}
		} std::cout << std::endl;

		// find and set game time
		pos = temp.find("|");
		section = temp.substr(1, pos - 1);
		this->time = stoi(section);
		temp = temp.substr(pos + 2);
		
		// find and set game length
		pos = temp.find("|");
		section = temp.substr(0, pos - 1);
		this->gameLength = stoi(section);
		temp = temp.substr(pos + 2);

		// find and set room items
		auto map = this->world.GetRoomMap();
		bool flag = true;
		while (flag) {
			pos = temp.find("|");
			if (pos == -1) {
				flag = false;
			}
			else {
				section = temp.substr(0, pos - 1);
				temp = temp.substr(pos + 2);
				bool flag2 = true;
				std::string lastPotItem;
				while (flag2) {
					pos = temp.find("|");
					std::string potItem = temp.substr(0, pos - 1);
					pos = potItem.find("-");
					potItem = potItem.substr(0, pos - 1);
					auto it = this->worldItems.find(potItem);
					if (it != this->worldItems.end() && lastPotItem != potItem) {
						GameObject& x = *it->second;
						this->world.GetRoom(section)->AddItem(x);
						lastPotItem = potItem;
						pos = temp.find("|");
						temp = temp.substr(pos + 2);
					}
					else { flag2 = false; }
				}
			}
				

		}
	}
	report.close();			// close file once complete
}

void Manager::loadDefault() {
	// load default items in rooms
	for (auto i = this->worldItems.begin(); i != this->worldItems.end(); i++) {
		GameObject& x = *i->second;
		if (i->second) {
			if (i->first == "Shank") {
				this->world.GetRoom("Personal Cell")->AddItem(x);
			}
			else if (i->first == "Handcuffs") {
				this->world.GetRoom("Guards Office")->AddItem(*i->second);
			}
			else if (i->first == "Exit Key") {
				this->world.GetRoom("Guards Office")->AddItem(*i->second);
			}
			else if (i->first == "Keycard") {
				this->warden.AddToInv(*i->second);
			}
			else if (i->first == "Grub") {
				this->world.GetRoom("Dining Hall")->AddItem(*i->second);
			}
			else if (i->first == "Dumbells") {
				this->world.GetRoom("Gym")->AddItem(*i->second);
			}
			else { std::cout << "error in loadDefault()" << std::endl; }
		}
	}
}

void Manager::updateTime() {
	this->time++;
}

void Manager::endScene() {
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "********************* GAME OVER *********************" << std::endl;
	std::cout << "End Stats: (HP-Strength) " << this->player.GetStats() << std::endl;
	std::cout << "Game completed in: " << this->gameLength << " days and " << this->time - 6 << " hours" << std::endl;
}