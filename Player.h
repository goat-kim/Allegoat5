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

	bool collision;

public:
	Player();
	~Player();
	void update();
	float getSpeed() const;
	void setSpeed(float s);
	void setCollision(bool c); // on test
};