#include <cstdio>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "UI.h"
#include "GameState.h"
#include "Sprite.h"

UI::UI()
	: visible(true), frameVisible(true), alpha(255), tint(0, 0, 0), colorKey(0, 0, 0),
	rect(0, 0, DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_HEIGHT),
	docked(false), rcDock(),
	marginTop(0), marginBottom(0), marginLeft(0), marginRight(0),
	paddingTop(0), paddingBottom(0), paddingLeft(0), paddingRight(0)
{
	printf("UI()\n");
}

UI::~UI() {
	printf("~UI()\n");
}

bool UI::isVisible() const {
	return visible;
}

void UI::setVisible(bool v) {
	visible = v;
}

bool UI::isFrameVisible() const {
	return frameVisible;
}

void UI::setFrameVisible(bool v) {
	frameVisible = v;
}

void UI::setColorKey(unsigned char r, unsigned char g, unsigned char b) {
	colorKey.r = r;
	colorKey.g = g;
	colorKey.b = b;
}

void UI::setTintColor(unsigned char r, unsigned char g, unsigned char b) {
	tint.r = r;
	tint.g = g;
	tint.b = b;
}

int UI::getAlpha() const {
	return alpha;
}

void UI::setAlpha(unsigned char a) {
	alpha = a;
}

int UI::getX() const {
	return rect.x;
}

int UI::getY() const {
	return rect.y;
}

void UI::setXY(int x, int y) {
	rect.x = x;
	rect.y = y;
}

int UI::getWidth() const {
	return rect.width;
}

int UI::getHeight() const {
	return rect.height;
}

int UI::getMarginTop() const {
	return marginTop;
}

int UI::getMarginBottom() const {
	return marginBottom;
}

int UI::getMarginLeft() const {
	return marginLeft;
}

int UI::getMarginRight() const {
	return marginRight;
}

int UI::getPaddingTop() const {
	return paddingTop;
}

int UI::getPaddingBottom() const {
	return paddingBottom;
}

int UI::getPaddingLeft() const {
	return paddingLeft;
}

int UI::getPaddingRight() const {
	return paddingRight;
}

void UI::setWidth(int width) {
	rect.width = width;
}

void UI::setHeight(int height) {
	rect.height = height;
}

void UI::setMargin(int top, int bottom, int left, int right) {
	marginTop = top;
	marginBottom = bottom;
	marginLeft = left;
	marginRight = right;
}

void UI::setPadding(int top, int bottom, int left, int right) {
	paddingTop = top;
	paddingBottom = bottom;
	paddingLeft = left;
	paddingRight = right;
}

void UI::setMarginTop(int m) {
	marginTop = m;
}

void UI::setMarginBottom(int m) {
	marginBottom = m;
}

void UI::setMarginLeft(int m) {
	marginLeft = m;
}

void UI::setMarginRight(int m) {
	marginRight = m;
}

void UI::setPaddingTop(int p) {
	paddingTop = p;
}

void UI::setPaddingBottom(int p) {
	paddingBottom = p;
}

void UI::setPaddingLeft(int p) {
	paddingLeft = p;
}

void UI::setPaddingRight(int p) {
	paddingRight = p;
}

void UI::dockTop() {
	rect.x = 0;
	rect.y = 0;
	rect.width = DEFAULT_DISPLAY_WIDTH;
	rect.height = DEFAULT_DISPLAY_HEIGHT / 3;
}

void UI::dockMiddle() {
	rect.x = 0;
	rect.y = DEFAULT_DISPLAY_HEIGHT / 3;
	rect.width = DEFAULT_DISPLAY_WIDTH;
	rect.height = DEFAULT_DISPLAY_HEIGHT / 3;
}

void UI::dockBottom() {
	rect.x = 0;
	rect.y = (DEFAULT_DISPLAY_HEIGHT * 2) / 3;
	rect.width = DEFAULT_DISPLAY_WIDTH;
	rect.height = DEFAULT_DISPLAY_HEIGHT / 3;
/*
	marginLeft = 10;
	marginRight = 10;
	paddingTop = 10;
	paddingBottom = 10;
	paddingLeft = 10;
	paddingRight = 10;
	*/
}

void UI::dockTopHalf() {
	rect.x = 0;
	rect.y = 0;
	rect.width = DEFAULT_DISPLAY_WIDTH;
	rect.height = DEFAULT_DISPLAY_HEIGHT / 2;
}

void UI::dockBottomHalf() {
	rect.x = 0;
	rect.y = DEFAULT_DISPLAY_HEIGHT / 2;
	rect.width = DEFAULT_DISPLAY_WIDTH;
	rect.height = DEFAULT_DISPLAY_HEIGHT / 2;
}

void UI::dockLeftHalf() {
	rect.x = 0;
	rect.y = 0;
	rect.width = DEFAULT_DISPLAY_WIDTH / 2;
	rect.height = DEFAULT_DISPLAY_HEIGHT;
}

void UI::dockRightHalf() {
	rect.x = DEFAULT_DISPLAY_WIDTH / 2;
	rect.y = 0;
	rect.width = DEFAULT_DISPLAY_WIDTH / 2;
	rect.height = DEFAULT_DISPLAY_HEIGHT;
}

void UI::undock() {
	// 원래 정보로 복구
}

Rect UI::getBorderArea() const {
	return Rect(
		rect.x + marginLeft,
		rect.y + marginTop,
		rect.width - marginLeft - marginRight,
		rect.height - marginTop - marginBottom
	);
}

Rect UI::getContentArea() const {
	return Rect(
		rect.x + marginLeft + paddingLeft,
		rect.y + marginTop + paddingTop,
		rect.width - marginLeft - marginRight - paddingLeft - paddingRight,
		rect.height - marginTop - marginBottom - paddingTop - paddingBottom
	);
}

void UI::drawUIArea() {
	// margin을 포함한 영역
	al_draw_filled_rectangle(
		rect.x,
		rect.y, 
		rect.x + rect.width,
		rect.y + rect.height, 
		al_map_rgba(0, 0, 128, 128)
	);
	// border 영역
	al_draw_filled_rectangle(
		rect.x + marginLeft,
		rect.y + marginTop,
		rect.x + rect.width - marginRight,
		rect.y + rect.height - marginBottom,
		al_map_rgba(128, 128, 0, 128)
	);
	// cotent 영역
	al_draw_filled_rectangle(
		rect.x + marginLeft + paddingLeft,
		rect.y + marginTop + paddingTop,
		rect.x + rect.width - marginRight - paddingRight,
		rect.y + rect.height - marginBottom - paddingBottom,
		al_map_rgba(128, 0, 0, 128)
	);
}

// Prototype Code:
//al_wait_for_event(eventQueue, &ev);
//if (ev.type == ALLEGRO_EVENT_TIMER) {
//	onPaint = true;
//	// update routines
//	if (dbox.busy) {
//		dbox.delayCnt++;
//		if (dbox.delayCnt >= dbox.rate) {
//			dbox.delayCnt = 0;
//			code = al_ustr_get_next(usList[4], &curPos);
//			printf("%d ", code);
//			if (code == -1)
//				dbox.busy = false;
//			else {
//				//al_ustr_append_chr(dbox.usDisplay, code);
//				al_ustr_set_chr(dbox.usDisplay, 0, code);
//				al_set_target_bitmap(dbox.dboxBitmap);
//				al_draw_ustr(sans18, al_map_rgb(255, 255, 255), dbox.marginLeft + dbox.paddingLeft + xoffset, dbox.marginTop + dbox.paddingTop + yoffset, 0, dbox.usDisplay);
//				al_set_target_bitmap(targetOrigin);
//
//				xoffset += al_get_ustr_width(sans18, dbox.usDisplay);
//			}
//		}
//	}
//}

DialogBox::DialogBox() : 
	dboxSpr(nullptr),
	xoffset(0), yoffset(0),
	xKeyCount(-1),
	cKeyCount(-1) {
	printf("DialogBox()\n");

	rate = rateOrigin = 5;
	delayCnt = 0;
	curPos = 0;
	usDisplay = al_ustr_new(" ");
	//busy = false;
	state = DBOX_STATE_CLOSED;
	/* Default setting: dockBottom */
	dockBottom();
	
	dboxTargetBitmap = al_create_bitmap(rect.width, rect.height);
	dboxMsgFont = GameState::getInstance()->getSystemFont(GAME_STATE_SYSTEM_FONT_SANS, 12);
	
	curMsgIdx = 0;
	blocking = false;

	truncated = false;
}

DialogBox::~DialogBox() {
	printf("~DialogBox()\n");

	if (usDisplay)
		al_ustr_free(usDisplay);
	if (dboxTargetBitmap)
		al_destroy_bitmap(dboxTargetBitmap);
	if (dboxSpr)
		delete dboxSpr;
}

bool DialogBox::loadScript(const char* pathname) {
	return scriptList.load(pathname);
}

bool DialogBox::loadUISprite(const char* pathname) {
	return dboxSpr->load(pathname);
}

void DialogBox::update() {
	int32_t code;
	GameState* ptGameState = GameState::getInstance();
	//ALLEGRO_BITMAP* targetOrigin = nullptr;
	//const ALLEGRO_FONT* msgFont = ptGameState->getSystemFont(GAME_STATE_SYSTEM_FONT_SANS, 12);
	
	if (ptGameState->isKeyDown(ALLEGRO_KEY_X)) {
		if (xKeyCount == -1) {
			xKeyCount = 1;
		}
		if (xKeyCount > 0) {
			/* Burst Mode */
			// busy 상태(텍스트 출력 중)일 때 x키가 눌리면
			// 해당 텀에서는 rate를 일시적으로 0으로 조정하여 텍스트를 빠르게 출력한다.
			if (state == DBOX_STATE_BUSY) {
				rate = 0;
			}
			xKeyCount--;
		}
	}

	if (ptGameState->isKeyDown(ALLEGRO_KEY_C)) {
		if (cKeyCount == -1) {
			cKeyCount = 1;
		}
		// on key down
		if (cKeyCount > 0) {
			/* Next */
			// IDLE 상태일 때(텍스트 출력을 마치고 다음 입력을 대기하는 상태)
			// showNext는 벡터 scriptList의 인덱스를 다음으로 조정한다.
			if (state == DBOX_STATE_IDLE) {
				if (!truncated) { // 대사가 길어서 잘린 경우 인덱스를 증가시키지 않는다(즉, 다음 대사로 넘어가지 않고 남은 대사를 마저 출력한다)
					// 다음 대사로 넘어간다
					curMsgIdx++;
					if (curMsgIdx >= (int)scriptList.length()) { // 모든 대사를 가져왔을 경우 UI를 닫는다
						curMsgIdx = 0;
						state = DBOX_STATE_CLOSED;
						return;
					}
				}
				show(curMsgIdx, false);
			}
			cKeyCount--;
		}
		// cKeyCount가 0이면 처리X
	}

	if (ptGameState->isKeyUp(ALLEGRO_KEY_X)) {
		xKeyCount = -1;
	}
	if (ptGameState->isKeyUp(ALLEGRO_KEY_C)) {
		cKeyCount = -1;
	}

	if (state == DBOX_STATE_BUSY) { // 대사 출력 상태
		/* rate로 지정된 수 이상의 update tick이 경과했을 경우 다음 문자를 출력한다. */
		delayCnt++;
		if (delayCnt >= rate) {
			delayCnt = 0;
			code = ustrGetNextChar(scriptList.ustrAt(curMsgIdx), &curPos);
			//nextCode = peekNextChar();
			//printf("(%d, %d)", code, nextCode);
			printf("%d ", code);

			if (code == -1) {
				// 텍스트의 끝에 도달할 경우 입력 대기 상태인 DBOX_STATE_IDLE로 전환한다.
				state = DBOX_STATE_IDLE;
				printf("\n");
				printf("DialogBox.state: DBOX_STATE_IDLE\n");
			}
			else {
				ustrSetChar(usDisplay, 0, code);
				drawChar();
			}
		}
	}
	//else if (state == DBOX_STATE_IDLE) { // 화살표 커서 출력
	//	//if (ptGameState->getKeyDown(ALLEGRO_KEY_C) && ptGameState->getPrevKeyUp(ALLEGRO_KEY_C)) {
	//	//	rate = rateOrigin;
	//	//	showNext();
	//	//}
	//}
}

// 다음에 출력할 문자의 코드를 얻어온다
int DialogBox::peekNextChar() const {
	return ustrGetChar(scriptList.ustrAt(curMsgIdx), curPos);
}

void DialogBox::clear() {
	al_clear_to_color(al_map_rgba(0, 0, 0, 0));
}

void DialogBox::draw() {
	if (visible && state != DBOX_STATE_CLOSED) {
		// target bitmap에 드로잉한 UI를 화면에 출력한다.
		al_draw_bitmap(dboxTargetBitmap, rect.x, rect.y, 0);
	}
}

void DialogBox::drawUIFrame() {
	// al_draw_tinted_bitmap;
	if (!dboxSpr) {
		al_draw_filled_rectangle(
			marginLeft, marginTop,
			rect.width - marginRight,
			rect.height - marginBottom,
			al_map_rgba(0, 128, 0, 128));
	}
	else {
		//dboxSpr->draw();
	}
}

void DialogBox::drawChar() {
	GameState* ptGameState = GameState::getInstance();
	ALLEGRO_BITMAP* targetOrigin = nullptr;
	targetOrigin = ptGameState->setTargetBitmap(dboxTargetBitmap);
	al_draw_ustr(dboxMsgFont,
		al_map_rgb(255, 255, 255),
		marginLeft + paddingLeft + xoffset,
		marginTop + paddingTop + yoffset,
		0,
		usDisplay);
	ptGameState->setTargetBitmap(targetOrigin);
	int chrWidth = al_get_ustr_width(dboxMsgFont, usDisplay);
	int chrHeight = al_get_font_line_height(dboxMsgFont);
	xoffset += chrWidth;

	// marginLeft + xoffset + chrWidth >= rect.width - marginRight - paddingRight
	// marginLeft + xoffset + chrWidth + marginRight + paddingRight >= rect.width
	// distance: (rect.width - marginRight - paddingRight) - (marginLeft + paddingLeft + xoffset + <앞으로 출력될 문자의 너비>) 

	// message disatnce: UI의 오른쪽 경계와 x커서 사이의 거리.
	/* distance가 충분히 좁을 경우 개행 */
	if (getMessageDistance(chrWidth) < paddingRight) {
		printf("distance: %d\n", getMessageDistance(chrWidth));
		xoffset = 0;
		yoffset += chrHeight;
	}

	/* 텍스트 행이 UI의 Content 영역을 벗어날 경우 나머지 텍스트는 다음 텀에서 출력해야 한다 */
	if (marginTop + paddingTop + yoffset + chrHeight >= rect.height - marginBottom - paddingBottom) {
		state = DBOX_STATE_IDLE;
		truncated = true;
		printf("DialogBox.state: DBOX_STATE_IDLE (truncated)\n");
	}
}

// 
// (rect.width - marginRight - paddingRight) - (marginLeft + paddingLeft + xoffset + <앞으로 출력될 문자의 너비>)
int DialogBox::getMessageDistance(int nextChrWidth) const {
	return (rect.width - marginRight - paddingRight) - (marginLeft + paddingLeft + xoffset + nextChrWidth);
}

void DialogBox::show(int idx, bool blocking) {
	GameState* ptGameState = GameState::getInstance();
	ALLEGRO_BITMAP* targetOrigin = nullptr;

	rate = getRate();
	xoffset = 0;
	yoffset = 0;
	curMsgIdx = idx;
	if (!truncated) {	// 공간 때문에 잘린 나머지 대사를 처리하려면 이전의 커서 값을 보존해야 한다
		curPos = 0;	
	}
	truncated = false;

	// UI frame을 먼저 출력하고 텍스트 출력 모드인 DBOX_STATE_BUSY로 전환한다.
	targetOrigin = ptGameState->setTargetBitmap(dboxTargetBitmap);
	clear();
	if (frameVisible) {
		drawUIFrame();
	}
	ptGameState->setTargetBitmap(targetOrigin);
	//busy = true;
	state = DBOX_STATE_BUSY;
}

// void DialogBox::showNext(bool blocking) {
// 	curMsgIdx++;
// 	if (curMsgIdx >= (int)scriptList.length()) {
// 		// closing
// 		curMsgIdx = 0;
// 		state = DBOX_STATE_CLOSED;
// 		return;
// 	}
// 	show(curMsgIdx, blocking);
// }

size_t DialogBox::getScriptLength() const {
	return scriptList.length();
}

int DialogBox::getState() const {
	return state;
}

void DialogBox::setRate(int r) {
	rateOrigin = r;
	rate = r;
}

int DialogBox::getRate() const {
	return rateOrigin;
}

void DialogBox::reallocUITargetBitmap() {
	if (dboxTargetBitmap) {
		al_destroy_bitmap(dboxTargetBitmap);
		dboxTargetBitmap = al_create_bitmap(rect.width, rect.height);
	}
}