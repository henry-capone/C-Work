#include <iostream>
#include "Character.hpp"
#include "Area.hpp"
#include "GameObject.hpp"
#include "GameManager.hpp"
#include <string>
#include <vector>
#include <memory>
#include <conio.h>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <functional>

void NPCUpdate(Manager* GameManager) {
	while (true) {
		if (!GameManager->running) { break; }		// if game is not running
		if (GameManager->warden.getStatus() == 1) { // if warden is awake
			if (GameManager->warden.GetLocation() == GameManager->player.GetLocation()) { // if in same room as player
				if (GameManager->time <= 20 || GameManager->time >= 6 ) { // if time is between 6 and 20 (24hr)
					std::cout << std::endl << "The warden is in the room with you - best behaviour" << std::endl;
				}
				else {								// fight player
					std::cout << "The warden has caught you out of your cell during lights out!" << GameManager->time << std::endl;
					NPC& warden = GameManager->warden;
					GameManager->player.fight(warden);
				}
			}
			std::this_thread::sleep_for(std::chrono::seconds(10)); // wait 10 seconds
			srand(time(NULL));
			int randNum = rand() % GameManager->warden.GetLocation()->GetExits().size() + 1; // select a random number based on rooms' exits
			int count = 0;
			auto roomMapCopy = GameManager->warden.GetLocation()->GetExits();
			for (auto it = roomMapCopy.begin(); it != roomMapCopy.end(); ++it) {
				count++;
				if (count == randNum) {				// if this exit index is the random number
					if (GameManager->warden.getStatus() == 1) { // if warden is awake
						if (GameManager->running) {	// if game is running
							GameManager->warden.SetLocation(it->second); // set location to destination of that exit
							// if warden has moved to freedom then move back to courtyard
							if (GameManager->warden.GetLocation()->GetDescription() == "Freedom") { GameManager->warden.SetLocation(GameManager->world.GetRoom("Courtyard")); }
							std::cout << std::endl << "The warden has moved!" << std::endl; // display warden has moved message
							if (it->second == GameManager->world.GetRoom("Personal Cell")) { // if warden has moved to personal cell
								if (GameManager->time >= 20 || GameManager->time <= 6) { // if time is not between 6 and 20
									GameManager->warden.SetLocation(GameManager->world.GetRoom("Hallway")); // move back to hallway
								}
							}
						}
						else { break; }				// if game is not running break
					}
				}
			}
		}
		else if (GameManager->warden.getStatus() == 0) { // if warden is knocked out
			std::this_thread::sleep_for(std::chrono::seconds(30)); // wait 30 seconds
			GameManager->warden.setStatus(1);		// set warden to awake
			std::cout << std::endl << "The warden has awoken and luckily forgot who attacked him" << std::endl;
		}
		else {										// else warden is handcuffed
			std::this_thread::sleep_for(std::chrono::seconds(90)); // wait 90 seconds
			GameManager->warden.setStatus(1);		// set warden to awake
			std::cout << std::endl << "The warden has broken out of the handcuffs - RUN and HIDE" << std::endl;
		}
	}
}

void TimeUpdate(Manager* GameManager) {
	while(true) {
		if (!GameManager->running) { break; }		// check if game is running
		std::this_thread::sleep_for(std::chrono::seconds(30)); // wait for 30 seconds
		if (GameManager->time == 24) {				// check if time equals 24 if so reset time
			GameManager->time = 0;
			GameManager->gameLength++;
			
			if (GameManager->world.GetRoom("Dining Hall")->GetItems().size() == 0) { // if dining hall has no items add food
				GameObject& food = *GameManager->worldItems.find("Grub")->second;
				GameManager->world.GetRoom("Dining Hall")->AddItem(food);
			}
		}
		else { GameManager->time++; }				// if time isn't 24 increase time
		if (GameManager->running) {
			std::cout << std::endl << "The time is : " << GameManager->time << ":00" << std::endl;
		}
		else { break; }

		if (GameManager->time == 20) {				// When time is 20 set lights out state
			GameManager->time++;
			std::cout << std::endl << "That is lights out - All inmates back to their cells for the night" << std::endl;
			GameManager->player.SetLocation(GameManager->world.GetRoom("Personal Cell"));
			GameManager->world.GetRoom("Personal Cell")->GetDoor()->CloseDoor();
		}
		else if (GameManager->time == 6) {			// When time is 6 reset to normal state
			GameManager->time++;
			std::cout << std::endl << "That is lights on - All inmates free to roam for the day" << std::endl;
			GameManager->world.GetRoom("Personal Cell")->GetDoor()->OpenDoor();
		}
	}
}

void PlayerUpdate(Manager* GameManager) {
	while (true) {
		if (!GameManager->running) { break; }		// check game is running
		if (GameManager->player.getHP() <= 0) {		// if player dies
			GameManager->running = false;
			std::cout << std::endl << "You have died!" << std::endl;
			GameManager->endScene();
		}
		if (GameManager->player.GetLocation()->GetDescription() == "Freedom") { // if player escapes
			GameManager->running = false;
			std::cout << std::endl << "Congratulations! You have escaped!" << std::endl;
			GameManager->endScene();
		}
	}
}

void main() {
	auto GameManager = Manager::getInstance();		// create instances and objects
	Area gameWorld;
	GameManager->world = gameWorld;
	GameManager->world.LoadMapFromFile("Game Map.txt");
	auto map = GameManager->world.GetRoomMap();
	GameManager->time = 6;
	GameManager->running = true;

	typedef std::shared_ptr<GameObject> GameObjectPtr;

	Knife knife;
	knife.setNameDesc("Shank - A knife made from various scrap materials");
	GameObjectPtr knifePtr = std::make_shared<Knife>(knife);
	GameManager->worldItems["Shank"] = knifePtr;

	Handcuff handCuff;
	handCuff.setNameDesc("Handcuffs - The guards stolen handcuffs");
	GameObjectPtr handcuffPtr = std::make_shared<Handcuff>(handCuff);
	GameManager->worldItems["Handcuffs"] = handcuffPtr;

	Keycard keycard;
	keycard.setNameDesc("Keycard - The guards stolen keycard");
	GameObjectPtr keycardPtr = std::make_shared<Keycard>(keycard);
	GameManager->worldItems["Keycard"] = keycardPtr;

	Keycard exitKey;
	exitKey.setNameDesc("Exit Key - The key to the main exit gate!");
	GameObjectPtr exitKeyPtr = std::make_shared<Keycard>(exitKey);
	GameManager->worldItems["Exit Key"] = exitKeyPtr;

	Food food;
	food.setNameDesc("Grub - The disgusting food served at the dining hall");
	GameObjectPtr foodPtr = std::make_shared<Food>(food);
	GameManager->worldItems["Grub"] = foodPtr;

	Weights weights;
	weights.setNameDesc("Dumbells - The weights used in the gym");
	GameObjectPtr weightsPtr = std::make_shared<Weights>(weights);
	GameManager->worldItems["Dumbells"] = weightsPtr;

	Player player1("Hero", 100, 2);
	GameManager->player = player1;
	CommandInterpreter interpreter(&GameManager->player);
	auto startLoc = GameManager->world.GetRoom("Personal Cell");
	GameManager->player.SetLocation(startLoc);

	NPC NPC1("Warden", 90, 6);
	GameManager->warden = NPC1;
	GameManager->warden.SetLocation(GameManager->world.GetRoom("Guards Office"));

	std::string lastCmd;

	//introduction:
	bool introRun = true;
	while (introRun) {
		std::cout << "Welcome to Text Adventure Game!" << std::endl << "Enter '1' to start a new game or '2' to load from last save" << std::endl;
		std::string loadQ;
		std::cin >> loadQ;
		if (loadQ == "2") {
			GameManager->loadProgress();
			introRun = false;
		}
		else if (loadQ != "1") {
			std::cout << "Invalid Input" << std::endl;
		}
		else { introRun = false; GameManager->loadDefault(); }
	}

	std::cout << "********************* Story *********************" << std::endl;
	std::cout << "You have been locked in prison! The warden wanders the prison watching over you. You get one meal per day! You can see the main exit to freedom in the courtyard. But where is the key?" << std::endl;
	std::cout << "The warden will know. Perhaps in his office you think to yourself. But there is no way to get in there without a keycard." << std::endl;
	std::cout << "How can I get the warden's you wonder" << std::endl << std::endl;

	std::cout << "Enter the following instructions as shown below:" << std::endl;
	std::cout << "Look around = 'Look'" << std::endl;
	std::cout << "Movement = 'Move X' - X is the room name or direction e.g (North,N)" << std::endl;
	std::cout << "Pick up = 'Take X' - X is the item name e.g (Take Shank)" << std::endl;
	std::cout << "Use = 'Use X on Y' - X & Y are the object names e.g (Use Food / Use Keycard on Door)" << std::endl;
	std::cout << "Fight = 'Attack' - Will start a fight with the warden" << std::endl;
	std::cout << "Inventory = 'Inv'" << std::endl;
	std::cout << "Quit = 'Quit'" << std::endl;
	std::cout << "Settings = 'Settings'" << std::endl;
	std::cout << "Use '&' to string commands together e.g (Move North & Look)" << std::endl;
	std::cout << "Press 'Z/z' to get the last use command" << std::endl;
	std::cout << "Don't forget capital letters!" << std::endl;
	std::cout << "Have FUN!" << std::endl << ">";

	std::thread NPCThread(NPCUpdate, GameManager);
	std::thread TimeThread(TimeUpdate, GameManager);
	std::thread PlayerThread(PlayerUpdate, GameManager);

	std::string input = "";
	bool inputBracket = false;
	while (GameManager->running) {		// user input section
		
		if (!inputBracket) {			// displays '>' symbol
			std::cout << std::endl << "> ";
			inputBracket = true;
		}

		if (_kbhit()) {
			char ch = _getch();
			if (ch == 90 || ch == 122) {	// if z or Z is pressed get last command
				input = lastCmd;
				std::cout << input;
			} 
			else if (ch == 10 || ch == 13) { // if enter or line break is pressed
				lastCmd = input;

				std::cout << std::endl << "Current Location: " << GameManager->player.GetLocation()->GetDescription() << std::endl;
				std::cout << "Current Time: " << GameManager->time << ":00" << std::endl;

				if (input == "Settings") {
					std::cout << "Enter the following instructions as shown below:" << std::endl;
					std::cout << "Look around = 'Look'" << std::endl;
					std::cout << "Movement = 'Move X' - X is the room name" << std::endl;
					std::cout << "Pick up = 'Take X' - X is the item name" << std::endl;
					std::cout << "Use = 'Use X on Y' - X & Y are the item names" << std::endl;
					std::cout << "Attack = 'Attack' - can only attack warden" << std::endl;
					std::cout << "Quit = 'Quit'" << std::endl;
					std::cout << "Settings = 'Settings'" << std::endl;
					std::cout << "Use '&' to string commands together" << std::endl;
					std::cout << "Save Progress = 'Save'" << std::endl;
					std::cout << "Don't forget capital letters!" << std::endl;
					input = "";
				}
				if (input == "Quit") {
					std::cout << "Thank you for playing" << std::endl;
					GameManager->running = false;
					GameManager->endScene();
					input = "";
				}

				interpreter.interpretCommand(input);
				inputBracket = false;
				input = "";
			} 
			else if (ch == 8) {			// if backspace is pressed
				if (input.length()!=0) {
					input.pop_back();
					std::cout << std::endl << ">" << input;
				}
				else {
					std::cout << "Nothing to delete" << std::endl;
					inputBracket = false;
				}
			} 
			else {						// if regular letter is pressed
				input += ch;
				std::cout << ch;
				inputBracket = true;
			}
		}
	}
	NPCThread.join();					// wait for threads to finish - only runs if game isn't running
	TimeThread.join();
	PlayerThread.join();
}