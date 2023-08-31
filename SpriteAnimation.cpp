#include <cstdio>
#include <cstring>
#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation()
	: spr(nullptr), regList(nullptr),
	idxBound(0), curIdx(0), tickCnt(0), animSpeed(AnimSpeed::ANIM_NORMAL),
	unitWidth(0), unitHeight(0),
	stop(true)
{
	printf("SpriteAnimation()\n");
	aniList.clear();
}

bool SpriteAnimation::load(const char* filename) {
	bool ret = false;
	spr = new Sprite();
	ret = spr->load(filename);
	printf("SpriteAnimation::open(%s): %d\n", filename, ret);
	return ret;
}

SpriteAnimation::~SpriteAnimation() {
	printf("~SpriteAnimation(): %s\n", spr->getFilename());

	clearAniList();
	releaseRegionList();
	if (spr)
		delete spr;
}

bool SpriteAnimation::createAutomatedRegionList(int row, int col) {
	// 일단 Sprite 객체 spr이 생성돼 있어야 한다.
	if (!spr->isOpen())
		return false;

	int bitmapWidth = spr->getBitmapWidth();
	int bitmapHeight = spr->getBitmapHeight();
	int unitWidth = bitmapWidth / col;
	int unitHeight = bitmapHeight / row;
	idxBound = row * col;

	this->unitWidth = unitWidth;
	this->unitHeight = unitHeight;

	// Create region list
	regList = new Rect[idxBound];
	if (!regList) {
		fprintf(stderr, "E: failed to create regList\n");
	}

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int curIdx = i * col + j;
			printf("curIdx/idxBound: %d/%d\n", curIdx, idxBound);
			regList[curIdx].x = unitWidth * j;
			regList[curIdx].y = unitHeight * i;
			regList[curIdx].width = unitWidth;
			regList[curIdx].height = unitHeight;
		}
	}
	printRegionList();
	
	/* Create AniList */
	// aniSection: 8개 캐릭터 중 첫 번째 캐릭터의 움직임을 나타내는 인덱스 집합
	// 이 변수는 테스트를 위해 하드코딩한 데이터이므로, 추후 이 부분에 대한 일반화 작업이 완성되면 삭제할 것
	int aniSection[4][3] = {
		{ 0, 1, 2 },	// 위쪽으로 걷기
		{ 12, 13, 14 },	// 오른쪽으로 걷기
		{ 24, 25, 26 },	// 아래쪽으로 걷기
		{ 36, 37, 38 }	// 왼쪽으로 걷기
	};

	/* 하나의 AniListEntry는 하나의 모션을 나타내는 여러 인덱스를 저장하는 객체다. */
	for (int i = 0; i < 4; i++) {
		insertAniListEntry(3, aniSection[i]); // 한 엔트리 당 3개 인덱스
	}

	printf("SpriteAnimation::createAutomatedRegionList(): '%d * %d' region list created and '%zu' aniList entry inserted\n", row, col, aniList.size());
	for (int i = 0; i < 4; i++) {
		aniList[i]->printList();
	}
	return true;
}

void SpriteAnimation::releaseRegionList() {
	if (regList)
		delete[] regList;
	regList = nullptr;
}

// int aniListIdx: 일반적으로는 Direction형 열거상수를 이용하여 방향에 따른 애니메이션을 출력하는 용도로 사용
void SpriteAnimation::update(int aniListIdx) {
	if (stop)
		return;

	int period = 0;
	switch (animSpeed) {
	case AnimSpeed::ANIM_SLOW:
		period = 20;
		break;
	case AnimSpeed::ANIM_VERY_SLOW:
		period = 30;
		break;
	case AnimSpeed::ANIM_FAST:
		period = 3;
		break;
	case AnimSpeed::ANIM_VERY_FAST:
		period = 1;
		break;
	case AnimSpeed::ANIM_NORMAL:
	default:
		period = 6;
		break;
	}
	// Update current region index
	if (tickCnt >= period) {
		tickCnt = 0;
		if (aniListIdx == -1) { // default
			curIdx++;
			if (curIdx >= idxBound)
				curIdx = 0;
		}
		else { // using aniList
			curIdx = aniList[aniListIdx]->idxNext();
		}
		//printf("curIdx: %d\n", curIdx);
	}
	tickCnt++;
}

void SpriteAnimation::draw() {
	//printf("curIdx: %d\n", curIdx);
	//printf("%f %f %f %f\n", regList[curIdx].x, regList[curIdx].y, regList[curIdx].width, regList[curIdx].height);
	spr->draw(&regList[curIdx]);
}

int SpriteAnimation::getRegionListLength() const {
	return idxBound;
}

Sprite* SpriteAnimation::getSprite() {
	return spr;
}

AnimSpeed SpriteAnimation::getAnimationSpeed() const {
	return animSpeed;
}

void SpriteAnimation::setAnimationSpeed(AnimSpeed s) {
	animSpeed = s;
}

int SpriteAnimation::getCurrentRegionIdx() const {
	return curIdx;
}

void SpriteAnimation::setCurrentRegionIdx(int i) {
	curIdx = i;
}

int SpriteAnimation::getCurrentSpriteWidth() const {
	return (int)regList[curIdx].width;
}

int SpriteAnimation::getCurrentSpriteHeight() const {
	return (int)regList[curIdx].height;
}

bool SpriteAnimation::isStop() const {
	return stop;
}

void SpriteAnimation::setStop(bool s) {
	stop = s;
}

void SpriteAnimation::insertAniListEntry(int size, const int* list) {
	aniList.push_back(new AniListEntry(size, list));
}

void SpriteAnimation::clearAniList() {
	for (int i = 0; i < (int)aniList.size(); i++) {
		delete aniList[i];
	}
	aniList.clear();
}

int SpriteAnimation::getUnitWidth() const {
	return unitWidth;
}

int SpriteAnimation::getUnitHeight() const {
	return unitHeight;
}

int SpriteAnimation::getUnitWidth(int idx) const {
	return (int)regList[idx].width;
}

int SpriteAnimation::getUnitHeight(int idx) const {
	return (int)regList[idx].height;
}

void SpriteAnimation::printRegionList() const {
	for (int i = 0; i < idxBound; i++) {
		printf("regList[%d]: [%d, %d, %d, %d]\n",
			i, regList[i].x, regList[i].y, regList[i].width, regList[i].height);
	}
}