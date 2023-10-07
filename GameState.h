#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/keycodes.h>

//static const char* GAME_TITLE = "Goat Simulator";

static const char* GAME_STATE_SYSTEM_FONT_PATH_SANS = "font/Dotum-03.ttf";
static const char* GAME_STATE_SYSTEM_FONT_PATH_SERIF = "font/Batang-01.ttf";

struct ALLEGRO_DISPLAY;
struct ALLEGRO_EVENT_QUEUE;
struct ALLEGRO_TIMER;
struct ALLEGRO_FONT;
struct ALLEGRO_BITMAP;
class Player;
class Scroller;

enum {
	GAME_STATE_SYSTEM_FONT_SANS = 0,
	GAME_STATE_SYSTEM_FONT_SERIF
};

enum {
	GAME_STATE_INIT = 0,
	GAME_STATE_SPLASH,
	GAME_STATE_RUNNING,
	GAME_STATE_PAUSE,
	GAME_STATE_UI_TITLE,
//	GAME_STATE_UI_MENU,
	GAME_STATE_UNKNOWN = -1
};

class GameState {
private:
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* systemTargetBitmap;		// 게임 화면을 렌더링할 버퍼; 게임 화면 렌더링 시 해당 비트맵을 target으로 전환할 것.
	ALLEGRO_BITMAP* displayBitmap;			// 원래의 target bitmap; display의 backbuffer와 동일하다. 화면상에 보이는 모습을 렌더링한다.
	ALLEGRO_EVENT_QUEUE* eventQueue;
	ALLEGRO_TIMER* timer;
	ALLEGRO_FONT* builtinFont;
	ALLEGRO_FONT* sans12;
	ALLEGRO_FONT* sans14;
	ALLEGRO_FONT* sans16;
	ALLEGRO_FONT* sans18;
	ALLEGRO_FONT* serif12;
	ALLEGRO_FONT* serif14;
	ALLEGRO_FONT* serif16;
	ALLEGRO_FONT* serif18;

	int curGameState;

	int bitmapWidth, bitmapHeight;
	int displayWidth, displayHeight;
	int oldDisplayFlags, newDisplayFlags;
	bool fullscreen;
	char gameTitle[256];
	bool pressedKeys[ALLEGRO_KEY_MAX];
	bool prevPressedKeys[ALLEGRO_KEY_MAX];

	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_KEYBOARD_STATE prevKeyState;

	Scroller* scroller;
	Player* ptPlayer;			// 현재 컨트롤 중인 플레이어 객체
	int mapWidth, mapHeight;	// 현재 맵의 크기

private:
	void registerEventSources();
	void destoryAllegroObjects();

private:
	GameState();
	~GameState();
	static GameState* instance;
public:
	static GameState* getInstance();
	static void destroyInstance();

	bool init();
	bool init(int w, int h);	// initialize with display width and height
	void initMap();
	void initMap(int w, int h);
	void update();

	int getCurrentGameState() const;
	void setCurrentGameState(int state);

	ALLEGRO_EVENT_QUEUE* getEventQueue() const;
	ALLEGRO_FONT* getBuiltinFont() const;

	ALLEGRO_FONT* getSystemFont(int type, int size) const;

	ALLEGRO_BITMAP* setTargetBitmap(ALLEGRO_BITMAP* target);
	ALLEGRO_BITMAP* getCurrentTargetBitmap() const;
	ALLEGRO_BITMAP* getSystemTargetBitmap() const;
	ALLEGRO_BITMAP* setNewTargetBitmap(ALLEGRO_BITMAP* bitmap);
	//ALLEGRO_BITMAP* getDisplayBitmap() const;
	int getTargetBitmapWidth() const;
	int getTargetBitmapHeight() const;
	void beginSystemBitmapContext();
	void beginDisplayBitmapContext();

	int getDisplayWidth() const;
	int getDisplayHeight() const;
	const char* getGameTitle() const;
	Scroller* getScroller() const;
	Player* getCurrentPlayer() const;
	void setCurrentPlayer(Player* pt);

	bool setDisplayWidth(int w);
	bool setDisplayHeight(int h);
	bool setDisplaySize(int w, int h);

	bool isFullscreen() const;
	void toggleFullscreen();

	int getMapWidth() const;
	int getMapHeight() const;
	void setMapSize(int w, int h);
	
	void setGameTitle(const char* t);
	void setDisplayFlag(int f);

	void startTimer();
	void stopTimer();

	// I/O Routines
	const bool* getKeyState() const;
	void setKeyDown(int keycode);
	void setKeyUp(int keycode);
	bool isKeyDown(int keycode) const;
	bool isKeyUp(int keycode) const;
	void setPrevKeyDown(int keycode);
	void setPrevKeyUp(int keycode);
	bool getPrevKeyDown(int keycode) const;
	bool getPrevKeyUp(int keycode) const;

	void saveToPrevKeyState();

	void screenshot(const char* path);

	// Event Handlers
	void onDisplayResize();
};
