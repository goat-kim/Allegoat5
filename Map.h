#pragma once
#include "Sound.h"
#include "Tilemap.h"
#include <vector>
#include <string>
using std::string;

class Map {
private:
	Sound *bgm;
	Sound *bgs;
	bool bgmLoop;
	bool bgsLoop;

	Tilemap* map;

	// TODO: 그외 전환 효과

public:
	Map();
	~Map();
	bool init();
	bool loadBGM(const char* pathname);
	bool loadBGS(const char* pathname);
	// Map *load
};

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
