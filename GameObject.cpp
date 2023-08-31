#include "GameObject.h"
#include "Sprite.h"
#include "SpriteAnimation.h"
#include "GameState.h"
#include "Scroller.h"
#include <cstdio>
#include <cstring>
#include <allegro5/allegro_primitives.h>

GameObject::GameObject()
	: sprAni(nullptr), ptSpr(nullptr),
	dir(Direction::DIRECT_DOWN), state(State::STATE_STOP),
	x(0.0f), y(0.0f), scrX(0.0f), scrY(0.0f), vx(0.0f), vy(0.0f),
	scrollMode(true),
	scrWidth(0), scrHeight(0),
	bndBox(), bndBoxVisible(false)
{
	memset(stopIdx, 0, sizeof(int) * NUM_OF_DIRECT);
	printf("GameObject()\n");
}

GameObject::~GameObject() {
	printf("~GameObject(): %s\n", ptSpr->getFilename());
	delete sprAni;
}

bool GameObject::init(const char* filename) {
	bool ret = false;
	sprAni = new SpriteAnimation();
	ret = sprAni->load(filename);
	ret = sprAni->createAutomatedRegionList(SpriteAnimation::DEFAULT_REGION_ROW, SpriteAnimation::DEFAULT_REGION_COL);
	ptSpr = sprAni->getSprite();
	ptSpr->setCenter(0.5f, 0.5f);
	//scrWidth = GameState::getInstance()->getDisplayWidth();
	//scrHeight = GameState::getInstance()->getDisplayHeight();

	scrWidth = GameState::getInstance()->getTargetBitmapWidth();
	scrHeight = GameState::getInstance()->getTargetBitmapHeight();

	// TODO: stopIdx의 초기화를 자동화(일반화)할 것
	stopIdx[DIRECT_UP] = 1;
	stopIdx[DIRECT_DOWN] = 25;
	stopIdx[DIRECT_LEFT] = 37;
	stopIdx[DIRECT_RIGHT] = 13;
	return ret;
}

// Todo: 크롭 영역 및 인덱스 매핑 정보를 파일(sprdata)로부터 읽어옴
// 이 기능은 GameObject가 아니라 SpriteAnimation 클래스에 정의해야 하지 않을까?
// 물론 GameObject가 SpriteAnimation 객체를 포함하고 있으므로, 이 함수에서 SpriteAnimation의 해당 루틴을 호출해야 할 것
// bool GameObject::init(const char* filename, const char* sprdata) {}

void GameObject::update() {
	const GameState* gameState = GameState::getInstance();
	const Scroller* ptScroller = gameState->getScroller();

	//state = STATE_WALK;
	//dir = DIRECT_LEFT;
	sprAni->setStop(false);
	sprAni->update(dir);

	// 스크롤러 뷰포트의 델타 값을 넣어보면 어떨까?
	// TODO: 스크롤 때문에 화면상에서 이동하더라도 실제 좌표 정보는 그대로 유지돼야 함

	/* 이전 프레임에서의 화면상 좌표를 기준으로 업데이트 */
	scrX = prevScrX;
	scrY = prevScrY;

	/* 배경 스크롤의 반영 */
	scrX -= ptScroller->getDx();
	scrY -= ptScroller->getDy();

	/* call custom update routine */
	customUpdate();

	float dx = x - prevX;
	float dy = y - prevY;
	scrX += dx; scrY += dy;

	prevX = x; prevY = y;
	prevScrX = scrX; prevScrY = scrY;
	ptSpr->setXY(scrX, scrY);
}

void GameObject::customUpdate() { }

void GameObject::draw() {
	sprAni->draw();
}

void GameObject::drawBoundaryBox() {
	const Rect& rc = getBoundaryBox();
	al_draw_rectangle(rc.x, rc.y, rc.x + rc.width, rc.y + rc.height,
		al_map_rgb(255, 255, 0), 1.0f);
}

SpriteAnimation* GameObject::getSpriteAnimation() const {
	return sprAni;
}

bool GameObject::isVisible() const {
	return sprAni->getSprite()->isVisible();
}

void GameObject::setVisible(bool v) {
	ptSpr->setVisible(v);
}

void GameObject::setXY(float x, float y) {
	this->x = x;
	this->y = y;
	scrX = prevScrX = prevX = x;
	scrY = prevScrY = prevY = y;
}

float GameObject::getX() const {
	return x;
}

float GameObject::getY() const {
	return y;
}

float GameObject::getScrX() const {
	return scrX;
}

float GameObject::getScrY() const {
	return scrY;
}

float GameObject::getVx() const {
	return vx;
}

float GameObject::getVy() const {
	return vy;
}

float GameObject::getScaleX() const {
	return ptSpr->getScaleX();
}

float GameObject::getScaleY() const {
	return ptSpr->getScaleX();
}

void GameObject::setScale(float s) {
	ptSpr->setScale(s);
	// bndBox도 업데이트 => bndBox는 호출할 때마다 계산할 것
}

void GameObject::setScaleX(float s) {
	ptSpr->setScaleX(s);
}

void GameObject::setScaleY(float s) {
	ptSpr->setScaleY(s);
}

void GameObject::setColorKey(int r, int g, int b) {
	ptSpr->setColorKey(r, g, b);
}

void GameObject::setAnimationSpeed(int s) {
	sprAni->setAnimationSpeed((AnimSpeed)s);
}

Direction GameObject::getDirect() const {
	return dir;
}

const Rect& GameObject::getBoundaryBox() {
	float uw = (float)sprAni->getUnitWidth() * ptSpr->getScaleX();
	float uh = (float)sprAni->getUnitHeight() * ptSpr->getScaleY();
	float cx = ptSpr->getCenterX();
	float cy = ptSpr->getCenterY();
	bndBox.x = ptSpr->getX() - (int)(cx * uw);
	bndBox.y = ptSpr->getY() - (int)(cy * uh);
	bndBox.width = (int)(sprAni->getUnitWidth() * ptSpr->getScaleX());
	bndBox.height = (int)(sprAni->getUnitHeight() * ptSpr->getScaleY());

	return bndBox;
}

bool GameObject::isScrollMode() const {
	return scrollMode;
}

void GameObject::setScrollMode(bool s) {
	scrollMode = s;
}

void GameObject::setBoundaryBoxVisible(bool v) {
	bndBoxVisible = v;
}

bool GameObject::isBoundaryBoxVisible() const {
	return bndBoxVisible;
}

void GameObject::setDirect(int dir) {
	this->dir = (Direction)dir;
}

State GameObject::getState() const {
	return state;
}

void GameObject::setState(int s) {
	if (s < STATE_STOP || s > STATE_UNKNOWN)
		s = STATE_UNKNOWN;
	state = (State)s;
}

bool GameObject::aabbIntersection(const Rect& rc) {
	getBoundaryBox();
	// 대상 오브젝트가 오른쪽에 떨어져 있거나 왼쪽에 있음)
	if (bndBox.x + bndBox.width < rc.x || rc.x + rc.width < bndBox.x)
		return false;
	// 대상 오브젝트가 아래에 떨어져 있거나 위에 있음
	if (bndBox.y + bndBox.height < rc.y || rc.y + rc.height < bndBox.y)
		return false;
	return true;
}