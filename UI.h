#pragma once
#include <allegro5/utf8.h>
#include "Util.h"
//struct ALLEGRO_COLOR;
//struct ALLEGRO_USTR;
//struct ALLEGRO_USTR_INFO;

class Sprite;
struct ALLEGRO_BITMAP;

class UI {
protected:
	/* 색상 관련 정보 */
	bool visible;
	unsigned char alpha;	// 투명도
	ColorRGB tint;			// 색상 효과
	ColorRGB colorKey;		// 컬러키 (투명색)
	// int id;
	// int type; -> 클래스 자료형으로 판별 가능

	/* 크기 및 위치 */
	Rect rect;
	int marginTop, marginBottom, marginLeft, marginRight;
	int paddingTop, paddingBottom, paddingLeft, paddingRight;

	// 특정 위치에 도킹 시 사용되는 값
	bool docked;
	Rect rcDock;
public:
	UI();
	virtual ~UI();

	bool isVisible() const;
	void setVisible(bool v);
	virtual void setColorKey(unsigned char r, unsigned char g, unsigned char b); // 자식 클래스에 따라 구현이 달라질 수 있음
	void setTintColor(unsigned char r, unsigned char g, unsigned char b);
	int getAlpha() const;
	void setAlpha(unsigned char a);

	int getX() const;
	int getY() const;
	void setXY(int x, int y);
	void setWidth(int width);
	void setHeight(int height);

	int getWidth() const;
	int getHeight() const;

	int getMarginTop() const;
	int getMarginBottom() const;
	int getMarginLeft() const;
	int getMarginRight() const;

	int getPaddingTop() const;
	int getPaddingBottom() const;
	int getPaddingLeft() const;
	int getPaddingRight() const;

	void setMargin(int top, int bottom, int left, int right);
	void setPadding(int top, int bottom, int left, int right);
	
	void setMarginTop(int m);
	void setMarginBottom(int m);
	void setMarginLeft(int m);
	void setMarginRight(int m);

	void setPaddingTop(int p);
	void setPaddingBottom(int p);
	void setPaddingLeft(int p);
	void setPaddingRight(int p);
	
	/* 화면 사이즈 기준 1/3 크기 */
	void dockTop();
	void dockBottom();
	void dockMiddle();

	void dockTopHalf();
	void dockBottomHalf();
	void dockLeftHalf();
	void dockRightHalf();

	void undock();			// 기본값; rect의 정보에 따라 배치됨

	virtual void draw() = 0;
};

// 유니코드 문자, 문자열을 어떻게 관리할지?
// 대사 처리 등등.. (파일 입출력, 유니코드 문제)
// 



class DialogBox : public UI {
private:
	ALLEGRO_FONT* dboxMsgFont;
	Sprite* dboxSpr;

	int rate;
	int rateOrigin;
	int delayCnt;
	//int cursorPos;
	//bool busy;
	int state = DBOX_STATE_NONE;
	ALLEGRO_USTR* usDisplay;
	ALLEGRO_BITMAP* dboxTargetBitmap;

	int curPos;
	int xoffset;
	int yoffset;

	UstrVector scriptList;
	int curMsgIdx;
	bool blocking;	// 대사 출력 시 게임 블로킹(일시 정지) 여부

	bool frameVisible;

	int xKeyCount;
	int cKeyCount;

public:
	enum { DBOX_STATE_NONE = 0, DBOX_STATE_BUSY, DBOX_STATE_IDLE, DBOX_STATE_CLOSING };
	// message
	// cursor
	// current cursor: 현재 문자가 출력될 위치
	// character face
	// rate: 문자가 출력되는 속도 (0 이하의 값으로 설정 시 지연 없이 바로 출력)
	// rateCtr: 문자 출력을 지연시킬 때 내부적으로 사용하는 카운터 (매 프레임마다 업데이트)
	// msgSound?: 문자 출력 시 효과음

	DialogBox();
	~DialogBox();

	bool loadScript(const char* pathname);
	bool loadUISprite(const char* pathname);
	void update();
	void draw();
	void drawUIFrame();
	void drawChar();
	//void drawChar(int32_t code);
	int getMessageDistance(int nextChrWidth = 0) const;
	int peekNextChar() const;

	void show(int idx, bool blocking = false);
	void showNext(bool blocking = false);

	//void setMessageRate(int r);

	size_t getScriptLength() const;

	int getState() const;

	int getRate() const;
	void setRate(int r);

	Rect getTextArea() const;
};

// 1. UI 객체: FIFO(또는 vector)로 리스트를 유지하는 건 어떨까?
// - 리스트 순서대로 렌더링됨
// - 마지막에 있는 UI가 자동으로 포커스
// - 레이어 번호를 유지할까?
// 
// 2. GameState에 현재 focus된 UI 객체나 객체의 ID를 지정하게 해볼까?
//