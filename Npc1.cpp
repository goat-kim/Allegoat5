#include <cstdio>
#include "Npc1.h"
#include "SpriteAnimation.h"
#include <cstdio>

Npc1::Npc1() : count(0), isCollision(0) {
	this->dir = DIRECT_RIGHT;
	this->state = STATE_WALK;

	this->stopIdx[DIRECT_UP] = 1;
	this->stopIdx[DIRECT_DOWN] = 25;
	this->stopIdx[DIRECT_LEFT] = 37;
	this->stopIdx[DIRECT_RIGHT] = 13;
}
Npc1::~Npc1() {
	printf("~Npc1()\n");
}

void Npc1::customUpdate() {
	float speed = 2.0f;
	SpriteAnimation *ptAni = getSpriteAnimation();
	//printf("myUpdate()\n");
	if (count == 200) {
		count = 0;
		if (dir == DIRECT_RIGHT)
			dir = DIRECT_LEFT;
		else
			dir = DIRECT_RIGHT;
	}

	if (isCollision) {
		setState(STATE_STOP);
		//vx = 0;
		
	}
	else {
		setState(STATE_WALK);
	}

	switch (state) {
	case STATE_STOP:
		sprAni->setStop(true);
		sprAni->setCurrentRegionIdx(this->stopIdx[dir]);
		break;
	case STATE_WALK:
		sprAni->setStop(false);
		if (this->dir == DIRECT_RIGHT) {
			vx = speed;
		}
		else {
			vx = -speed;
		}
		x += vx;
		count++;
		break;
	}
}

void Npc1::setCollision(bool c) {
	isCollision = c;
}