#pragma once
#include "Util.h"

class GameState;
class Sprite;
class SpriteAnimation;

enum Direction {
	DIRECT_UP = 0,
	DIRECT_RIGHT,
	DIRECT_DOWN,
	DIRECT_LEFT,
	DIRECT_UNKNOWN
};

enum State {
	STATE_STOP = 0,
	STATE_IDLE,
	STATE_WALK,
	STATE_RUN,
	STATE_UNKNOWN
};

#define NUM_OF_DIRECT	5
#define DEFAULT_SPEED	10

class GameObject {
protected:
	SpriteAnimation* sprAni;	// 스프라이트 애니메이션 정보를 포함하고 업데이트하는 객체
	Sprite* ptSpr;				// sprAni가 포함하고 있는 Sprite 실객체를 가리키는 포인터
	Direction dir;				// 현재 방향
	State state;				// 현재 (이동 또는 정지 등의) 상태
	float x, y;					// 실제 좌표
	float prevX, prevY;			// 이전 프레임에서의 실제 좌표
	float scrX, scrY;			// 화면상에 출력되는 좌표
	float prevScrX, prevScrY;	// 이전 프레임에서 화면상에 출력되는 좌표
	float vx, vy;				// 게임 오브젝트의 속도 벡터
	bool scrollMode;			// 게임 오브젝트가 스크롤링에 영향을 받는지 여부
	int scrWidth, scrHeight;	// 화면 크기 (스크롤링 처리용)

	Rect bndBox;				// 경계 상자 (충돌처리 및 디버깅 용)
	bool bndBoxVisible;			// 경계 상자 시각화 여부

	int stopIdx[NUM_OF_DIRECT];	// 정지 시 인덱스

public:
	GameObject();
	virtual ~GameObject();
	virtual void update();
	virtual void customUpdate();

	bool init(const char* filename);
	void draw();
	void drawBoundaryBox();

	SpriteAnimation* getSpriteAnimation() const;
	bool isScrollMode() const;
	bool isBoundaryBoxVisible() const;
	bool isVisible() const;

	float getX() const;
	float getY() const;
	float getScrX() const;
	float getScrY() const;
	float getVx() const;
	float getVy() const;
	float getScaleX() const;
	float getScaleY() const;
	Direction getDirect() const;
	const Rect& getBoundaryBox();

	void setScrollMode(bool s);
	void setBoundaryBoxVisible(bool v);
	void setColorKey(int r, int g, int b);
	void setAnimationSpeed(int s);
	void setVisible(bool v);
	void setXY(float x, float y);
	void setScale(float s);
	void setScaleX(float s);
	void setScaleY(float s);

	void setDirect(int dir);
	State getState() const;	// State형을 GameObjectState형으로 이름 변경?
	void setState(int s);
	bool aabbIntersection(const Rect& rc);
};