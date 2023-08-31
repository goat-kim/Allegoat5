#pragma once
#include "Util.h"

class Player;
class Scroller
{
private:
	Rect bgBnd;			// background boundary
	Rect vp;			// view port; 기본값은 0, 0과 화면 크기
	Player* ptPlayer;	// 현재 컨트롤 중인 플레이어 객체의 포인터
	float prevX, prevY;	// 이전 프레임에서의 vp.x, vp.y
	float dx, dy;		// 현재 프레임에서의 배경 이동 정도
	bool fixedX, fixedY;
	Rect svp;			// scaled view port
public:
	Scroller();
	~Scroller();
	//bool init(const Rect& bgBound, Player* pt);
	bool init();
	bool init(const Rect& bgBound);
	void update();
	void setPlayer(Player* pt);
	const Rect* getViewPort() const;
	const Rect* getScaledViewPort(float s);

	int getDx() const;
	int getDy() const;

	void setFixX(bool f);
	void setFixY(bool f);
	void setFix(bool f);
	bool isFixX() const;
	bool isFixY() const;
};