#pragma once
#include "Sound.h"
#include "Tilemap.h"
#include <vector>

class Map {
private:
	Sound *bgm;
	Sound *bgs;
	bool bgmLoop;
	bool bgsLoop;

	Tilemap* map;

public:
	Map();
	~Map();
	bool init();
	bool loadBGM(const char* pathname);
	bool loadBGS(const char* pathname);
};