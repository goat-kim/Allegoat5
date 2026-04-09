#pragma once
#include "Sound.h"
#include <vector>
#include <string>

class Map {
private:
	int id;
	std::string name;

	Sound *bgm;
	Sound *bgs;
	bool bgmLoop;
	bool bgsLoop;

	// TODO: 그외 전환 효과 추가

public:
	Map(int mapid);
	Map(int mapid, const char *mapname);
	~Map();
	bool init();
	bool loadBGM(const char *pathname);
	bool loadBGS(const char *pathname);
	int getId() const;
	virtual int getMapWidth() const = 0;
	virtual int getMapHeight() const = 0;
	virtual int getMapWidthPx() const = 0;
	virtual int getMapHeightPx() const = 0;
	virtual void draw() = 0;
	// Map *load
};

// TODO: 맵의 메모리 할당 및 해제를 관리하는 메커니즘의 필요
/*
// MapStatList entry
class MapStat {
private:
	int id;			// 맵을 식별할 수 있는 고유번호, MapStatList의 인덱스로 사용된다.
	string label;	// 
	bool valid;		// 맵의 로드(할당) 상태를 나타냄
	Map *map;		// Map 객체의 포인터

public:
	int getId() const;
	string getLabel() const;
	void setLabel(const string& l);
	bool isValid() const;
};
*/