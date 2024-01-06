#pragma once
#include <allegro5/utf8.h>
#include "Util.h"
//struct ALLEGRO_COLOR;
//struct ALLEGRO_USTR;
//struct ALLEGRO_USTR_INFO;

class Sprite;
struct ALLEGRO_BITMAP;

// Padding과 Margin
// Margin: 테두리(border)를 기준으로 바깥 여백
// Padding: 테두리 내부와 내용(content) 사이의 여백
class UI {
protected:
	/* 색상 관련 정보 */
	bool visible;
	bool frameVisible;
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

	bool isFrameVisible() const;
	void setFrameVisible(bool v);

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

	Rect getBorderArea() const;
	Rect getContentArea() const;
	void drawUIArea();

	virtual void draw() = 0;
};

/* 카운터 값들은 모두 프레임을 단위로 동기된다. */
// TODO: 캐릭터 얼굴, 이름 출력
// TODO: 스프라이트 기반 UI 스킨을 UI 클래스로 일반화할 것
// TODO: 대사 출력 완료 시 커서 표시
// 편의상 프리셋을 두는 것이 좋을 듯
class DialogBox : public UI {
private:
	ALLEGRO_FONT* dboxMsgFont;
	Sprite* dboxSpr;

	int cursorBlinkRate;	// 커서의 깜빡임 속도
	int blinkCnt;			// 경과 시간 측정용 카운터
	bool cursorBlink;

	int rate;				// 대사의 출력 속도
	int rateOrigin;			// 대사 빨리 넘김 시 원래 출력 속도 유지용도
	int delayCnt;			// 경과 시간 측정을 위해 사용되는 카운터

	int state = DBOX_STATE_CLOSED;
	ALLEGRO_USTR* usDisplay;
	ALLEGRO_BITMAP* dboxTargetBitmap;

	int curPos;
	int xoffset;
	int yoffset;

	UstrVector scriptList;
	int curMsgIdx;
	bool blocking;	// 대사 출력 시 게임 블로킹(일시 정지) 여부

	int xKeyCount;
	int cKeyCount;

	bool truncated = false;

public:
	enum { DBOX_STATE_CLOSED = 0, DBOX_STATE_BUSY, DBOX_STATE_IDLE, DBOX_STATE_TRUNCATED, DBOX_STATE_UNKNOWN };
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
	void setUISpriteColorKey(int r, int g, int b);
	void update();
	void draw();
	void cleanupUI(); // 반드시 dboxTargetBitmap을 target일 때 호출할 것
	void drawUIFrame();
	void drawChar();
	void drawCursor();
	bool isUICursorOn() const;
	//void drawChar(int32_t code);
	int getMessageDistance(int nextChrWidth = 0) const;
	int peekNextChar() const;

	void show(int idx, bool blocking = false);
	//void showNext(bool blocking = false);

	//void setMessageRate(int r);

	size_t getScriptLength() const;

	int getState() const;

	int getRate() const;
	void setRate(int r);
	void setCursorBlinkRate(int r);

	void reallocUITargetBitmap();
};

// class Menu: public UI {
// private:
// };

// 1. UI 객체: FIFO(또는 vector)로 리스트를 유지하는 건 어떨까?
// - 리스트 순서대로 렌더링됨
// - 마지막에 있는 UI가 자동으로 포커스
// - 레이어 번호를 유지할까?
// 
// 2. GameState에 현재 focus된 UI 객체나 객체의 ID를 지정하게 해볼까?
//

// sysUIColorKey: al_map_rgb(0xff, 0x9c, 0x00)
//enum SYSTEM_UI_REGION_IDX
enum {
	SYSTEM_UI_REGION_IDX_BACKGROUND = 0,
	SYSTEM_UI_REGION_IDX_UI_FRAME = 1,
	SYSTEM_UI_REGION_IDX_UI_FRAME_TOP_LEFT = 1,
	SYSTEM_UI_REGION_IDX_UI_FRAME_TOP_RIGHT = 4,
	SYSTEM_UI_REGION_IDX_UI_FRAME_BOTTOM_LEFT = 9,
	SYSTEM_UI_REGION_IDX_UI_FRAME_BOTTOM_RIGHT = 12,
	SYSTEM_UI_REGION_IDX_UI_FRAME_TOP = 2,
	SYSTEM_UI_REGION_IDX_UI_FRAME_BOTTOM = 10,
	SYSTEM_UI_REGION_IDX_UI_FRAME_LEFT = 5,
	SYSTEM_UI_REGION_IDX_UI_FRAME_RIGHT = 6,
	SYSTEM_UI_REGION_IDX_UI_FRAME_UP_ARROW = 13,
	SYSTEM_UI_REGION_IDX_UI_FRAME_DOWN_ARROW = 14,
	SYSTEM_UI_REGION_IDX_UI_ITEM_1 = 15,
	SYSTEM_UI_REGION_IDX_UI_ITEM_2 = 29,
	SYSTEM_UI_REGION_IDX_UI_ITEM_BLANK = 28,
	SYSTEM_UI_REGION_IDX_NUM_0 = 41
};

const Rect sysUIRegionList[] = {
	// x,  y,  w,  h
	{  0,  0, 32, 32 },	// background (idx:1)
	{ 32,  0,  8,  8 }, // UI frame
	{ 40,  0,  8,  8 },
	{ 48,  0,  8,  8 },
	{ 56,  0,  8,  8 }, 
	{ 32,  8,  8,  8 },
	{ 56,  8,  8,  8 },
	{ 32, 16,  8,  8 },
	{ 56, 16,  8,  8 },
	{ 32, 24,  8,  8 }, 
	{ 40, 24,  8,  8 }, 
	{ 48, 24,  8,  8 }, 
	{ 56, 24,  8,  8 }, 
	{ 40,  8, 16,  8 }, // UI frame up arrow (idx:14)
	{ 40, 16, 16,  8 }, // UI frame down arrow (idx:15)
	{ 64,  0,  8,  8 }, // UI item 1 (idx:16)
	{ 72,  0,  8,  8 }, 
	{ 80,  0,  8,  8 }, 
	{ 88,  0,  8,  8 }, 
	{ 64,  8,  8,  8 }, 
	{ 88,  8,  8,  8 }, 
	{ 64, 16,  8,  8 }, 
	{ 88, 16,  8,  8 }, 
	{ 64, 24,  8,  8 }, 
	{ 72, 24,  8,  8 }, 
	{ 80, 24,  8,  8 }, 
	{ 88, 24,  8,  8 }, 
	{ 72,  8,  8,  8 }, // UI item blank (green, idx:29)
	{ 96,  0,  8,  8 }, // UI item 2 (idx:30)
	{ 104,  0,  8,  8 }, 
	{ 112,  0,  8,  8 }, 
	{ 120,  0,  8,  8 }, 
	{ 96,   8,  8,  8 }, 
	{ 120,  8,  8,  8 }, 
	{ 96,  16,  8,  8 }, 
	{ 120, 16,  8,  8 }, 
	{ 96,  24,  8,  8 }, 
	{ 104, 24,  8,  8 }, 
	{ 112, 24,  8,  8 }, 
	{ 120, 24,  8,  8 }, 
	{  32, 32,  8, 16 }, // Numeric 0 (idx:42)
	{  40, 32,  8, 16 }, 
	{  48, 32,  8, 16 }, 
	{  56, 32,  8, 16 }, 
	{  64, 32,  8, 16 }, 
	{  72, 32,  8, 16 }, 
	{  80, 32,  8, 16 }, 
	{  88, 32,  8, 16 }, 
	{  96, 32,  8, 16 }, 
	{ 104, 32,  8, 16 }, 
	{ 112, 32,  8, 16 }, 
};