#pragma once
#include <iostream>
#include <fstream>
#include "Area.hpp"

class Manager 
{
private:
	Manager() {};
	static Manager* instance;
	std::shared_ptr<GameObject> getItem(std::string name);
public:
	static Manager* getInstance();
	Area world;
	std::map < std::string, std::shared_ptr<GameObject >> worldItems;
	Player player;
	NPC warden;
	int gameLength;
	int time;
	bool running;
	std::map<int, std::shared_ptr<GameObject>> doorList;
	void updateTime();
	const int saveProgres();
	void loadProgress();
	void loadDefault();
	void endScene();
};