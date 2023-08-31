#pragma once
#include <vector>
#include <cstdint>
#include "Util.h"

const int DEFAULT_TILE_WIDTH = 16;
const int DEFAULT_TILE_HEIGHT = 16;

struct ALLEGRO_BITMAP;
class Sprite;

typedef struct Tile {
	int16_t id;
	int8_t type;
	Tile(int16_t i, int8_t t) : id(i), type(t) {}
} Tile;

class Tileset {
private:
	int tileWidth, tileHeight;			// 단일 타일의 가로와 세로 길이 (픽셀 단위)
	int tilesetWidth, tilesetHeight;	// 맵의 가로와 세로 길이 (타일 단위)
	Sprite* mapspr;

public:
	Tileset(int tw = DEFAULT_TILE_WIDTH, int th = DEFAULT_TILE_HEIGHT);
	~Tileset();
	bool load(const char* filename);
	int getTileWidth() const;
	int getTileHeight() const;
	int getTilesetWidth() const;
	int getTilesetHeight() const;
	int getTilesetSpriteWidth() const;
	int getTilesetSpriteHeight() const;
	Sprite* getSprite() const;
	Rect toRect(int tidx) const; // 최적화: 참조를 반환하는 방법으로 대체할 수 없는지 테스트해볼 것
};

// 각 레이어에 타일맵을 일단 한 번 draw해두면 업데이트가 발생하지 않는 이상 매 프레임마다 다시 그릴 필요가 없다.
class TilemapLayer {
private:
	int mapWidth, mapHeight;		// 맵의 가로/세로 타일 개수
	//int tileWidth, tileHeight;		// 타일의 가로/세로 픽셀 길이
	ALLEGRO_BITMAP* layerBitmap;
	std::vector<Tile> map;
	Tileset* ptTileset;				// 
	//char* filename;

private:
	int tok(char** buf); // null: -1, LF: -2, otherwise: tileID
public:
	//TilemapLayer();
	TilemapLayer(Tileset* tsPtr);
	~TilemapLayer();
	bool load(const char* filename);
	//bool loadMapSprite(const char* filename);
	void setTileset(Tileset* tsPtr);
	void draw();
	int getTileId(int idx);
	int getTileId(int x, int y);
	int getTileType(int idx);
	int getTileType(int x, int y);

	int getMapWidth() const;
	int getMapHeight() const;
	int getTileWidth() const;
	int getTileHeight() const;
	int getMapWidthPx() const;
	int getMapHeightPx() const;
	ALLEGRO_BITMAP* getLayerBitmap() const;
	
	//const Tile& getTile(int idx);
	//const Tile& getTile(int x, int y);
};

class Tilemap {
private:
	//int nlayers;
	//TilemapLayer* layers;
	std::vector<TilemapLayer*> layers;
	int tileWidth, tileHeight;
public:
	Tilemap(int tw = DEFAULT_TILE_WIDTH, int th = DEFAULT_TILE_HEIGHT);
	void draw();
};
