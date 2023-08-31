#include "Map.h"
#include <cstdio>
#include <cstring>

Map::Map() : bgm(nullptr), bgs(nullptr) {
	printf("Map()\n");
}

Map::~Map() {
	printf("~Map()\n");
}

bool Map::init() {

}

bool Map::loadBGM(const char* pathname) {
}

bool Map::loadBGS(const char* pathname) {

}