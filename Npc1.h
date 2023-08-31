#pragma once
#include "GameObject.h"

class Npc1 : public GameObject
{
private:
	int count;			// step count
	bool isCollision;

public:
	Npc1();
	~Npc1();
	void customUpdate();
	void setCollision(bool c);
};