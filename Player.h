#pragma once
#include "Util.h"
#include "GameObject.h"

class Player : public GameObject {
private:
	bool isMoveKeyPressed() const;

private:
	bool willUpdateScrollerX;
	bool willUpdateScrollerY;
	float speed;

	int cKeyCount;

public:
	Player();
	~Player();
	void update();
	float getSpeed() const;
	void setSpeed(float s);
};