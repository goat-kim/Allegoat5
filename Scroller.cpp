#include "Scroller.h"
#include "GameState.h"
#include "Player.h"

#include <cstdio>

Scroller::Scroller() 
	: vp(0.0f, 0.0f, (float)TARGET_BITMAP_WIDTH, (float)TARGET_BITMAP_HEIGHT), 
	bgBnd(), 
	ptPlayer(nullptr), 
	prevX(0), prevY(0),
	dx(0), dy(0),
	fixedX(false), fixedY(false),
	svp()
{
	printf("Scroller()\n");
}

Scroller::~Scroller() {
	printf("~Scroller()\n");
}

//bool Scroller::init(const Rect& bgBound, Player* pt) {
// 현재 맵 크기를 기준으로 스크롤러의 배경 크기 적용
bool Scroller::init() {
	GameState* gs = GameState::getInstance();
	Rect rcBG(0.0f, 0.0f, gs->getMapWidth(), gs->getMapHeight());
	return init(rcBG);
}

bool Scroller::init(const Rect& bgBound) {
	GameState* gs = GameState::getInstance();

	bgBnd = bgBound;
	ptPlayer = gs->getCurrentPlayer();
	float px = ptPlayer->getX();
	float py = ptPlayer->getY();
	float hw = vp.width / 2.0f;
	float hh = vp.height / 2.0f;

	vp.x = px - hw;
	vp.y = py - hh;

	// 뷰포트의 왼쪽 끝이 배경 경계를 벗어나는 경우
	if (vp.x < bgBnd.x)
		vp.x = bgBnd.x;
	// 오른쪽 끝
	else if (vp.x + vp.width > bgBnd.x + bgBnd.width)
		vp.x = bgBnd.x + bgBnd.width - vp.width;
	// 위쪽 끝
	if (vp.y < bgBnd.y)
		vp.y = bgBnd.y;
	// 아래쪽 끝
	else if (vp.y + vp.height > bgBnd.y + bgBnd.height)
		vp.y = bgBnd.y + bgBnd.height - vp.height;

	prevX = vp.x;
	prevY = vp.y;

	printf("Scroller::init(const Rect&)=======\n");
	printf("playerXY: [%f, %f]\n", px, py);
	printf("vp: [%d, %d, %d, %d]\n", vp.x, vp.y, vp.width, vp.height);
	printf("bgBnd: [%d, %d, %d, %d]\n", bgBnd.x, bgBnd.y, bgBnd.width, bgBnd.height);
	printf("prevXY: [%f, %f]\n", prevX, prevY);
	printf("==================================\n");
	return true;
}

void Scroller::update() {
	if (fixedX && fixedY) {
		dx = dy = 0.0f;
		return;
	}

	float halfWidth = vp.width / 2;
	float halfHeight = vp.height / 2;
	float curX = ptPlayer->getX();
	float curY = ptPlayer->getY();
	
	// 플레이어 이동에 따라 뷰포트 업데이트
	//vp.x = curX - halfWidth;
	//vp.y = curY - halfHeight;
	if (!fixedX)
		vp.x += ptPlayer->getVx();
	if (!fixedY)
		vp.y += ptPlayer->getVy();
	//printf("bgBnd:\t[%f, %f, %f, %f]\n", bgBnd.x, bgBnd.y, bgBnd.width, bgBnd.height);
	
	//printf("\tvp=[%f, %f]\n", vp.x, vp.y);

	if (!fixedX) {
		// 왼쪽 끝
		if (vp.x < bgBnd.x) {
			//printf("if (vp.x < bgBnd.x)\n");
			vp.x = bgBnd.x;
		}
		// 오른쪽 끝
		else if (vp.x + vp.width > bgBnd.x + bgBnd.width) {
			//printf("else if (vp.x + vp.width > bgBnd.x + bgBnd.width)\n");
			vp.x = bgBnd.x + bgBnd.width - vp.width;
		}
	}
	if (!fixedY) {
		// 위쪽 끝
		if (vp.y < bgBnd.y)
			vp.y = bgBnd.y;
		// 아래쪽 끝
		else if (vp.y + vp.height > bgBnd.y + bgBnd.height)
			vp.y = bgBnd.y + bgBnd.height - vp.height;
	}
	//printf("vp:\t[%f, %f, %f, %f]\n", vp.x, vp.y, vp.width, vp.height);

	dx = vp.x - prevX;
	dy = vp.y - prevY;

	prevX = vp.x;
	prevY = vp.y;
}

void Scroller::setPlayer(Player* pt) {
	ptPlayer = pt;
}

const Rect* Scroller::getViewPort() const {
	return &vp;
}

// 배경 화면이 s배 확대된 경우에는 원래 좌표로 변환해야 한다
// 원래 비트맵 크기를 기준으로 영역을 지정하고 s배 확대하여 draw하는 것이기 때문
const Rect* Scroller::getScaledViewPort(float s) {
	svp.x = vp.x / s;
	svp.y = vp.y / s;
	svp.width = vp.width / s;
	svp.height = vp.height / s;
	return &svp;
}

int Scroller::getDx() const {
	return (int)dx;
}

int Scroller::getDy() const {
	return (int)dy;
}

void Scroller::setFixX(bool f) {
	fixedX = f;
}

void Scroller::setFixY(bool f) {
	fixedY = f;
}

void Scroller::setFix(bool f) {
	fixedX = fixedY = f;
}

bool Scroller::isFixX() const {
	return fixedX;
}

bool Scroller::isFixY() const {
	return fixedY;
}