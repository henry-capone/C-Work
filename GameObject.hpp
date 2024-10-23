#include <iostream>
#include "Character.hpp"

#ifndef GameObject_H
#define GameObject_H

// Object Interaction Framework
class GameObject {
protected:
	std::string name;
public:
	virtual std::shared_ptr<GameObject> createCopy() const = 0;
	virtual std::string getName() const = 0;
	void setNameDesc(std::string name) { this->name = name; }
	bool operator== (const GameObject& other) const { return other.name == name; }
	bool operator== (const std::shared_ptr<GameObject>& other) const { return other->name == name; }

};

class Keycard : public GameObject {
public:
	std::shared_ptr<GameObject> createCopy() const override {
		return std::make_shared<Keycard>(*this);
	}
	void setNameDesc(std::string name) {
		this->name = name;
	}
	std::string getName() const override { return this->name; }

};

class Door : public GameObject {
private:
	bool isOpen = false;
public:
	std::shared_ptr<GameObject> createCopy() const override {
		return std::make_shared<Door>(*this);
	}
	void OpenDoor() { this->isOpen = true; }
	void CloseDoor() { this->isOpen = false; }
	bool GetDoorState() const { return isOpen; }
	void setNameDesc(std::string name ) {
		this->name = name; 
	}
	std::string getName() const override { return this->name; }
};

class Knife : public GameObject {
public:
	std::shared_ptr<GameObject> createCopy() const override {
		return std::make_shared<Knife>(*this);
	}
	void setNameDesc(std::string name) { 
		this->name = name;
	}
	std::string getName() const override { return this->name; }
};

class Handcuff : public GameObject {
public:
	std::shared_ptr<GameObject> createCopy() const override {
		return std::make_shared<Handcuff>(*this);
	}
	void setNameDesc(std::string name) {
		this->name = name;
	}
	std::string getName() const override { return this->name; }
};

class Food : public GameObject {
public:
	std::shared_ptr<GameObject> createCopy() const override {
		return std::make_shared<Food>(*this);
	}
	void setNameDesc(std::string name) {
		this->name = name;
	}
	std::string getName() const override { return this->name; }
};

class Weights : public GameObject {
public:
	std::shared_ptr<GameObject> createCopy() const override {
		return std::make_shared<Weights>(*this);
	}
	void setNameDesc(std::string name) {
		this->name = name;
	}
	std::string getName() const override { return this->name; }
}; 

#endif 