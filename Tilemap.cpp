#include "Tilemap.h"
#include "Sprite.h"
#include "Util.h"
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <allegro5/allegro5.h>

Tileset::Tileset(int tw, int th) : tileWidth(tw), tileHeight(th), 
	tilesetWidth(0), tilesetHeight(0), mapspr(nullptr) {
	printf("Tileset()\n");
}

Tileset::~Tileset() {
	delete mapspr;
	printf("~Tileset()\n");
}

bool Tileset::load(const char* filename) {
	mapspr = new Sprite();
	if (!mapspr->load(filename)) {
		fprintf(stderr, "Tileset::load(): failed to load %s\n", filename);
		return false;
	}
	tilesetWidth = mapspr->getWidth() / tileWidth;
	tilesetHeight = mapspr->getHeight() / tileHeight;
	return true;
}

int Tileset::getTileWidth() const {
	return tileWidth;
}

int Tileset::getTileHeight() const {
	return tileHeight;
}

int Tileset::getTilesetWidth() const {
	return mapspr->getWidth() / tileWidth;
}

int Tileset::getTilesetHeight() const {
	return mapspr->getHeight() / tileHeight;
}

int Tileset::getTilesetSpriteWidth() const {
	return mapspr->getWidth();
}

int Tileset::getTilesetSpriteHeight() const {
	return mapspr->getHeight();
}

Sprite* Tileset::getSprite() const {
	return mapspr;
}

Rect Tileset::toRect(int tid) const {
	Rect rc;
	rc.x = (tid % tilesetWidth) * tileWidth;
	rc.y = (tid / tilesetWidth) * tileHeight;
	rc.width = tileWidth;
	rc.height = tileHeight;
	return rc;
}

//TilemapLayer::TilemapLayer()
//	: mapWidth(0), mapHeight(0),
//	layerBitmap(nullptr), ptTileset(nullptr) {
//	printf("TilemapLayer()\n");
//	map.clear();
//}

TilemapLayer::TilemapLayer(Tileset* tsPtr)
	: mapWidth(0), mapHeight(0),
	layerBitmap(nullptr), ptTileset(tsPtr) {
	printf("TilemapLayer(Tileset*)\n");
	map.clear();
}

TilemapLayer::~TilemapLayer() {
	if (layerBitmap)
		al_destroy_bitmap(layerBitmap);
	map.clear();
	printf("~TilemapLayer()\n");
}

// -1: null (\0)
// -2: LF (\n)
int TilemapLayer::tok(char** buf) {
	char curCh;
	int tileIdx = 0;

	while (true) {
		curCh = **buf;
		if (isdigit(curCh)) {
			int curDigit = curCh - '0';
			char nextCh;
			if (tileIdx <= 0) {
				tileIdx = curDigit;
			}
			else {
				tileIdx *= 10;
				tileIdx += curDigit;
			}
			// 다음 문자에 개행이나 널 문자가 들어오면 타일 번호 반환
			*buf += 1;
			nextCh = **buf;
			if (nextCh == '\r' || nextCh == '\n' || nextCh == 0)
				return tileIdx;
			else if (nextCh == ',') {
				*buf += 1;		// 다음 토큰 인식을 위해 콤마를 넘어간다.
				return tileIdx;
			}
		}
		else if (curCh == 0) {	// null 문자는 -1 반환
			return -1;
		}
		else if (curCh == '\n') { // 개행 문자(LF)는 -2 반환
			*buf += 1;
			return -2;
		}
		else { // 그 외 문자들(예: CR)은 무시
			*buf += 1;
		}
	}
	return -1;
}

bool TilemapLayer::load(const char* filename) {
	char* csvbuf = NULL;
	long fileSize = 0, bufSize = 0;
	FILE* fp = NULL;

	if (!ptTileset) {
		fprintf(stderr, "TilemapLayer::load(): ptTileset is not initialized\n");
		return false;
	}
	int tileWidth = ptTileset->getTileWidth();
	int tileHeight = ptTileset->getTileHeight();

	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "E: failed to load %s\n", filename);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	bufSize = fileSize + 1;
	fseek(fp, 0, SEEK_SET);
	csvbuf = (char*)malloc(bufSize);
	if (!csvbuf) {
		fprintf(stderr, "csvbuf: failed to allocate memory\n");
		return false;
	}

	memset(csvbuf, 0, bufSize);

	fread(csvbuf, 1, bufSize, fp);
	fclose(fp);

	printf("file name: %s\n", filename);
	printf("buffer size: %ld\n", bufSize);
	printf("csvbuf[bufSize-1]: %d\n", csvbuf[bufSize - 1]);

	mapWidth = 0;
	mapHeight = 0;
	int tileCnt = 0;
	int tileId = 0;
	char* ptr = csvbuf;
	while ((tileId = tok(&ptr)) != -1) {
		if (tileId == -2) {
			mapHeight++;
			//printf("\n");
			continue;
		}
		tileCnt++;
		if (mapHeight < 1)
			mapWidth++;
		//printf("%03d ", tileId);
		map.push_back(Tile(tileId, 0));
	}
	printf("\n");
	printf("total: %d tiles\n", tileCnt);
	printf("map width: %d, map height: %d\n", mapWidth, mapHeight);
	printf("csvbuf:\t%p\n", csvbuf);
	printf("ptr:\t%p\n", ptr);
	printf("map.size(): %zu\n", map.size());

	free(csvbuf);
	csvbuf = nullptr;

	if (layerBitmap) {
		al_destroy_bitmap(layerBitmap);
	}

	layerBitmap = al_create_bitmap(tileWidth * mapWidth, tileHeight * mapHeight);

	return true;
}

//bool TilemapLayer::loadMapSprite(const char* filename) {
//	
//}

void TilemapLayer::setTileset(Tileset* tsPtr) {
	ptTileset = tsPtr;
}

void TilemapLayer::draw() {


	ALLEGRO_BITMAP* targetOrigin = al_get_target_bitmap();
	al_set_target_bitmap(layerBitmap);

	al_clear_to_color(al_map_rgb(0, 0, 0));
	int tileWidth = ptTileset->getTileWidth();		// 타일의 가로 픽셀 길이
	int tileHeight = ptTileset->getTileHeight();	// 타일의 세로 픽셀 길이
	int width = ptTileset->getTilesetWidth();		// 타일셋의 가로 타일 개수
	int height = ptTileset->getTilesetHeight();		// 타일셋의 세로 타일 개수

	int i, j;
	int x = 0, y = 0;	// 맵 상에 출력될 위치
	int idx = 0;
	int tileId = 0;
	Sprite* tilesetSpr = ptTileset->getSprite();

	for (i = 0; i < mapHeight; i++) {
		x = 0;
		for (j = 0; j < mapWidth; j++) {
			tileId = getTileId(idx);
			// 칩셋에서 출력될 영역 선택
			Rect rc = ptTileset->toRect(tileId);
			
			// 맵 상에 출력될 위치 선택
			//printf("rc.x: %d, rc.y: %d\n", rc.x, rc.y);
			//printf("x: %d, y: %d\n", x, y);
			
			tilesetSpr->setXY(x, y);
			tilesetSpr->draw(&rc);
			x += tileWidth;
			idx++;
		}
		y += tileHeight;
	}

	al_set_target_bitmap(targetOrigin);
}

int TilemapLayer::getTileId(int idx) {
	int maxIdx = (mapWidth * mapHeight) - 1;
	if (idx > maxIdx || idx < 0) // illegal index
		return -1;
	return map[idx].id;
}

int TilemapLayer::getTileId(int x, int y) {
	return getTileId((mapWidth * y) + x);
}

int TilemapLayer::getTileType(int idx) {
	int maxIdx = (mapWidth * mapHeight) - 1;
	if (idx > maxIdx || idx < 0)
		return -1;
	return map[idx].type;
}

int TilemapLayer::getTileType(int x, int y) {
	return getTileType((mapWidth * y) + x);
}

ALLEGRO_BITMAP* TilemapLayer::getLayerBitmap() const {
	return layerBitmap;
}

int TilemapLayer::getMapWidth() const {
	return mapWidth;
}

int TilemapLayer::getMapHeight() const {
	return mapHeight;
}

int TilemapLayer::getTileWidth() const {
	//return ptTileset->tileWidth;
	return ptTileset->getTileWidth();
}

int TilemapLayer::getTileHeight() const {
	//return tileHeight;
	return ptTileset->getTileHeight();
}

int TilemapLayer::getMapWidthPx() const {
	return mapWidth * ptTileset->getTileWidth();
}

int TilemapLayer::getMapHeightPx() const {
	//return mapHeight * tileHeight;
	return mapHeight * ptTileset->getTileHeight();
}

Tilemap::Tilemap(int tw, int th) 
	: tileWidth(tw), tileHeight(th) {
	printf("Tilemap::Tilemap()\n");
	layers.clear();
}

void Tilemap::draw() {
	// (1). 하위 레이어부터 상위 레이어까지 각 레이어의 비트맵을 화면(target bitmap)에 그린다.
	// (2). 플레이어와 NPC 등의 게임 오브젝트를 target bitmap에 그린다.
	// (3). (2)보다 우선순위가 높은(즉, 위로 올라가야 하는) 높은 타일들을 다시 target bitmap에 그린다.
	// (3)의 기능은 draw대신 다른 멤버 함수로 분리할 것

	// 반복 시 foreach나 iterator를 사용할 것
}
