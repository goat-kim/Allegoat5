#include "Map.h"
#include <cstdio>
#include <cstring>

Map::Map(int mapid) : id(mapid), bgm(nullptr), bgs(nullptr) {
#ifdef DEBUG
	printf("Map(id=%d)\n", id);
#endif
}

Map::~Map() {
#ifdef DEBUG
	printf("~Map(), id=%d\n", id);
#endif
}

bool Map::init() {

}

bool Map::loadBGM(const char* pathname) {
}

bool Map::loadBGS(const char* pathname) {

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