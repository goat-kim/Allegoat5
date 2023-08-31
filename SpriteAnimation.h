#pragma once
#include "Sprite.h"
#include <vector>
#include <cstdio>
#include <cstring>

enum AnimSpeed {
	ANIM_NORMAL = 0,
	ANIM_VERY_SLOW,
	ANIM_SLOW,
	ANIM_FAST,
	ANIM_VERY_FAST
};

// AniListEntry: 하나의 연속적인 애니메이션 표현 (예: 위로 걷기, 오른쪽으로 걷기, 아래쪽으로 뛰기 등을 각각 하나의 AniListEntry에 저장)
// 전체 region list(SpriteAnimation::regList)의 인덱스 리스트(정수 배열)를 저장함
// 하나의 AniListEntry는 하나의 모션을 나타내는 여러 인덱스를 저장하는 객체다.
class AniListEntry {
private:
	int len;
	int* list;
	int curIdx;
public:
	AniListEntry(int l, const int *lst) : len(l), list(nullptr), curIdx(0) {
		list = new int[len];
		memcpy(list, lst, sizeof(int) * len);
		//for (int i = 0; i < len; i++) {
		//	printf("%d ", list[i]);
		//}
		//printf("\n");
		printf("AniListEntry()\n");
	}
	~AniListEntry() {
		if (list)
			delete[] list;
		printf("~AniListEntry()\n");
	}

	int idxCur() const {
		return list[curIdx];
	}

	int idxNext() {
		curIdx++;
		if (curIdx >= len)
			curIdx = 0;
		return list[curIdx];
	}

	int idxPrev() {
		curIdx--;
		if (curIdx < 0)
			curIdx = len - 1;
		return list[curIdx];
	}

	void printList() const {
		for (int i = 0; i < len; i++)
			printf("%d ", list[i]);
		printf("\n");
	}
};

class SpriteAnimation {
private:
	Sprite* spr;
	Rect* regList;
	int idxBound;
	int curIdx;
	int tickCnt;
	AnimSpeed animSpeed;
	std::vector<AniListEntry*> aniList;

	bool stop;

	// For automated region list
	int unitWidth, unitHeight;
public:
	static const int DEFAULT_REGION_ROW = 8;
	static const int DEFAULT_REGION_COL = 12;

public:
	SpriteAnimation();
	~SpriteAnimation();
	bool load(const char* filename);
	bool createAutomatedRegionList(int row, int col);
	void releaseRegionList();

	void update(int aniListIdx = -1);
	void draw();

	int getRegionListLength() const;

	Sprite* getSprite();
	AnimSpeed getAnimationSpeed() const;
	void setAnimationSpeed(AnimSpeed s);
	int getCurrentRegionIdx() const;
	void setCurrentRegionIdx(int i);
	int getCurrentSpriteWidth() const;
	int getCurrentSpriteHeight() const;

	bool isStop() const;
	void setStop(bool s);

	void insertAniListEntry(int size, const int* list);
	void clearAniList();

	/* for automated region list */
	int getUnitWidth() const;
	int getUnitHeight() const;

	/* for general case */
	int getUnitWidth(int idx) const;
	int getUnitHeight(int idx) const;

	void printRegionList() const;
};
