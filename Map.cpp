#include "Map.h"
#include <cstdio>
#include <cstring>

Map::Map() : bgm(nullptr), bgs(nullptr) {
#ifdef DEBUG
	printf("Map()\n");
#endif
}

Map::~Map() {
#ifdef DEBUG
	printf("~Map()\n");
#endif
}

bool Map::init() {

}

bool Map::loadBGM(const char* pathname) {
}

bool Map::loadBGS(const char* pathname) {

}

int MapStat::getId() const {
	return id;
}

string MapStat::getLabel() const {
	return label;
}

bool MapStat::idValid() const {
	return valid;
}