#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Room.hpp"

class Room;
class GameObject;
class Keycard;
class Door;
class Knife;
class Handcuff;
class Food;
class Weights;
class NPC;

class Character {
protected:
	std::string name;
	int health;
	int strength;
	std::vector<std::shared_ptr<GameObject>> inventory;
public:
	Character();
	Character(const std::string& name, int health, int strength);
};

class Player : public Character {
private:
	Room* location;
public:
	Player();
	Player(const std::string& name, int health, int strength);
	void SetLocation(Room* location);
	Room* GetLocation();
	void AddToInv(const GameObject& item);
	void move() { std::cout << "Player has "; };
	void pickup() { std::cout << "Player has picked up "; };
	void lookA() { std::cout << "Player has looked around" << std::endl; };
	void interact() { std::cout << "Player has tried to use " << std::endl; }
	void fight(NPC& enemy);
	void setStr(int val) { this->strength = val; }
	int getStr() { return this->strength; }
	void setHP(int val) { this->health = val; }
	int getHP() { return this->health; }
	std::string GetStats();
	void SetStats(int health, int strength);
	std::vector < std::shared_ptr<GameObject>> GetInv();
	void removeItem(GameObject& item);
};

class NPC : public Character {
private:
	Room* location;
	int awake = 1;
public:
	NPC();
	NPC(const std::string& name, int health, int strength);
	void SetLocation(Room* location);
	Room* GetLocation();
	void AddToInv(const GameObject& item);
	std::string GetStats();
	int GetHP() { return this->health; }
	int GetStr() { return this->strength; }
	void SetHP(int val) { this->health = val; }
	void SetStats(int health, int strength);
	void setStatus(int state) { this->awake = state; }
	int getStatus() { return this->awake; }
	std::vector < std::shared_ptr<GameObject>> GetInv();
	void removeItem(GameObject& item);
};

class CommandInterpreter {
public:
	CommandInterpreter(Player* player) : player_(player) {}

	void interpretCommand(const std::string& command);
private:
	Player* player_;
};