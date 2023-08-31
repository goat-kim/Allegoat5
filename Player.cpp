#include "Player.h"
#include "Scroller.h"
#include "GameState.h"
#include "SpriteAnimation.h"
#include <cstdio>
#include <cstring>
#include <allegro5/allegro_primitives.h>

Player::Player() : GameObject(), 
	willUpdateScrollerX(false), willUpdateScrollerY(false),
	speed(DEFAULT_SPEED),
	cKeyCount(-1), collision(false)
{
	printf("Player()\n");
}

Player::~Player() {
	printf("~Player()\n");
}

bool Player::isMoveKeyPressed() const {
	const bool* kp = GameState::getInstance()->getKeyState();
	return (kp[ALLEGRO_KEY_UP] || kp[ALLEGRO_KEY_DOWN] ||
		kp[ALLEGRO_KEY_LEFT] || kp[ALLEGRO_KEY_RIGHT]);
}

void Player::update() {
	GameState* gameState = GameState::getInstance();

	if (collision) {
		this->setState(STATE_STOP);
		sprAni->setCurrentRegionIdx(this->stopIdx[dir]);
		return;
	}

	if (gameState->isKeyDown(ALLEGRO_KEY_C)) {
		if (cKeyCount == -1) {
			cKeyCount = 1;
		}
		if (cKeyCount > 0) {
			if (this->x == 100 && this->y == 100) {
				printf("YAMSO\n");
			}
			cKeyCount--;
		}
	}
	if (gameState->isKeyUp(ALLEGRO_KEY_C)) {
		cKeyCount = -1;
	}

	if (isMoveKeyPressed()) {
		state = STATE_WALK;
		if (gameState->isKeyDown(ALLEGRO_KEY_UP)) {
			//printf("Pressed: up\n");
			dir = DIRECT_UP;
		}
		else if (gameState->isKeyDown(ALLEGRO_KEY_DOWN)) {
			dir = DIRECT_DOWN;
		}
		else if (gameState->isKeyDown(ALLEGRO_KEY_LEFT)) {
			dir = DIRECT_LEFT;
		}
		else if (gameState->isKeyDown(ALLEGRO_KEY_RIGHT)) {
			dir = DIRECT_RIGHT;
		}
		vx = 0.0f, vy = 0.0f;
		switch (dir) {
		case DIRECT_UP:
			vy = -speed;
			break;
		case DIRECT_DOWN:
			vy = speed;
			break;
		case DIRECT_LEFT:
			vx = -speed;
			break;
		case DIRECT_RIGHT:
			vx = speed;
			break;
		}
	}
	else {
		state = STATE_STOP;
		vx = 0.0f, vy = 0.0f;
	}

	// 현재는 특정 캐릭터에 대해 인덱스가 하드코딩된 상태

	//00 01 02	11	UP
	//12 13 14	23	RIGHT
	//24 25 26	35	DOWN
	//36 37 38	47	LEFT
	switch (state) {
	case STATE_WALK:
		sprAni->setStop(false);
		sprAni->update(dir); // SpriteAnimation::update(aniListIdx)
		break;
	case STATE_STOP:
	default:
		sprAni->setStop(true);
		if (dir < DIRECT_UP || dir > DIRECT_LEFT)
			dir = DIRECT_DOWN;
		sprAni->setCurrentRegionIdx(this->stopIdx[dir]);
		break;
	}

	x += vx;
	y += vy;
	
	float halfWidth = scrWidth / 2;
	float halfHeight = scrHeight / 2;
	float mapWidth = gameState->getMapWidth();
	float mapHeight = gameState->getMapHeight();
	Scroller* ptScroller = gameState->getScroller();

	// 충돌처리
	if (x < 0.0f)
		x = 0;
	else if (x > mapWidth)
		x = mapWidth;
	if (y < 0.0f)
		y = 0;
	else if (y > mapHeight)
		y = mapHeight;

	// 화면상에 출력될 x, y 좌표
	scrX = x;
	scrY = y;

	if (willUpdateScrollerX) {
		ptScroller->setFixX(false);
		willUpdateScrollerX = false;
	}

	if (willUpdateScrollerY) {
		ptScroller->setFixY(false);
		willUpdateScrollerY = false;
	}

	if (scrollMode) {
		//printf("hw: %f, hh: %f\n", halfWidth, halfHeight);
		//printf("mw: %f, mh: %f\n", mapWidth, mapHeight);

		// 플레이어가 양쪽 끝에 있지 않으면 x축 배경 스크롤ON,
		// 플레이어는 화면 중간으로 고정
		if (x >= halfWidth && x <= mapWidth - halfWidth) {
			scrX = halfWidth;
			//ptScroller->setFixX(false);
			willUpdateScrollerX = true;
		}
		else {
			scrX = (x <= halfWidth) ? x : scrWidth - (mapWidth - x);
			ptScroller->setFixX(true);
		}
		// 플레이어가 위아래 끝에 있지 않으면 y축 배경 스크롤,
		// 플레이어는 화면 중간으로 고정
		if (y >= halfHeight && y <= mapHeight - halfHeight) {
			scrY = halfHeight;
			//ptScroller->setFixY(false);
			willUpdateScrollerY = true;
		}
		else {
			scrY = (y <= halfHeight) ? y : scrHeight - (mapHeight - y);
			ptScroller->setFixY(true);
		}

		//printf("XY: [%f, %f]\n", x, y);
	}

	ptSpr->setXY(scrX, scrY);
}

float Player::getSpeed() const {
	return speed;
}

void Player::setSpeed(float s) {
	speed = s;
}

void Player::setCollision(bool c) {
	collision = c;
}