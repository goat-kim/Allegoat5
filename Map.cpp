#include "Map.h"
#include <cstdio>
#include <cstring>

Map::Map(int mapid)
	: id(mapid), bgm(nullptr), bgs(nullptr) {
#ifdef DEBUG
	printf("Map(id=%d)\n", id);
#endif
}

Map::Map(int mapid, const char *mapname)
	: Map(mapid) {
	name = mapname;
#ifdef DEBUG
	printf("Map(id=%d, name=%s)\n", id, name);
#endif
}

Map::~Map() {
#ifdef DEBUG
	printf("~Map(), id=%d, name=%s\n", id, name);
#endif
}

bool Map::init() {
	return true;
}

bool Map::loadBGM(const char* pathname) {
	return true;
}

bool Map::loadBGS(const char* pathname) {
	return true;
}

int Map::getId() const {
	return id;
}

/*
int MapStat::getId() const {
	return id;
}

string MapStat::getLabel() const {
	return label;
}

bool MapStat::idValid() const {
	return valid;
}
*/