#include "Util.h"
#include "GameState.h"
#include "Scroller.h"
#include "Player.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cstdio>
#include <cstring>

GameState* GameState::instance = nullptr;

/* Constructors and Destructor */
GameState::GameState()
	: display(nullptr), eventQueue(nullptr), timer(nullptr), builtinFont(nullptr),
	displayWidth(DEFAULT_DISPLAY_WIDTH), displayHeight(DEFAULT_DISPLAY_HEIGHT),
	oldDisplayFlags(0), newDisplayFlags(0), fullscreen(false),
	scroller(nullptr),
	ptPlayer(nullptr),
	mapWidth(TARGET_BITMAP_WIDTH), mapHeight(TARGET_BITMAP_HEIGHT),
	curGameState(GAME_STATE_INIT)
{
	printf("GameState()\n");
	memset(gameTitle, 0, sizeof(gameTitle));
	memset(pressedKeys, 0, sizeof(pressedKeys));
	memset(prevPressedKeys, 0, sizeof(prevPressedKeys));
}

GameState::~GameState() {
	printf("~GameState()\n");
	if (scroller)
		delete scroller;
	destroyAllegroObjects();
}

/* Private member functions */
void GameState::registerEventSources() {
	printf("GameState::registerEventSources()\n");
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_mouse_event_source());
	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
}

bool GameState::loadBuiltinFont() {
	if (!(builtinFont = al_create_builtin_font())) {
		fprintf(stderr, "E: failed to create builtin font\n");
		return false;
	}
	return true;
}

bool GameState::loadSystemFonts() {
	if (!(sans12 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SANS, 12, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SANS);
		return false;
	}
	if (!(sans14 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SANS, 14, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SANS);
		return false;
	}
	if (!(sans16 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SANS, 16, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SANS);
		return false;
	}
	if (!(sans18 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SANS, 18, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SANS);
		return false;
	}

	if (!(serif12 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SERIF, 12, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SERIF);
		return false;
	}
	if (!(serif14 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SERIF, 14, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SERIF);
		return false;
	}
	if (!(serif16 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SERIF, 16, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SERIF);
		return false;
	}
	if (!(serif18 = al_load_ttf_font(GAME_STATE_SYSTEM_FONT_PATH_SERIF, 18, ALLEGRO_TTF_MONOCHROME))) {
		fprintf(stderr, "E: failed to load ttf file %s\n", GAME_STATE_SYSTEM_FONT_PATH_SERIF);
		return false;
	}

	return true;
}

void GameState::destroyAllegroObjects() {
	if (systemTargetBitmap)
		al_destroy_bitmap(systemTargetBitmap);
	if (display)
		al_destroy_display(display);
	if (builtinFont)
		al_destroy_font(builtinFont);
	if (sans12)
		al_destroy_font(sans12);
	if (sans14)
		al_destroy_font(sans14);
	if (sans16)
		al_destroy_font(sans16);
	if (sans18)
		al_destroy_font(sans18);
	if (serif12)
		al_destroy_font(serif12);
	if (serif14)
		al_destroy_font(serif14);
	if (serif16)
		al_destroy_font(serif16);
	if (serif18)
		al_destroy_font(serif18);
	if (timer)
		al_destroy_timer(timer);
	if (eventQueue)
		al_destroy_event_queue(eventQueue);
}

GameState* GameState::getInstance() {
	if (!instance)
		instance = new GameState();
	return instance;
}

void GameState::destroyInstance() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

bool GameState::init() {
	return init(DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_HEIGHT);
}

bool GameState::init(int w, int h) {
	printf("GameState::init(%d, %d)\n", w, h);
	curGameState = GAME_STATE_INIT;
	displayWidth = w;
	displayHeight = h;
	//bool ret = false;
	if (!(eventQueue = al_create_event_queue())) {
		fprintf(stderr, "GameState::init(): failed to create event queue\n");
		return false;
	}
	// 60fps
	if (!(timer = al_create_timer(1.0 / 60.0))) {
		fprintf(stderr, "GameState::init(): failed to create main timer\n");
		return false;
	}

	/* Load fonts */
	if (loadBuiltinFont() == false) {
		fprintf(stderr, "GameState::loadBuiltinFont(): failed to load the builtin font\n");
		return false;
	}

	if (loadSystemFonts() == false) {
		fprintf(stderr, "GameState::loadSystemFonts(): failed to load system fonts\n");
		return false;
	}

	//al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	oldDisplayFlags = al_get_new_display_flags();

	if (!(display = al_create_display(displayWidth, displayHeight))) {
		fprintf(stderr, "GameState::init(): failed to create display\n");
		return false;
	}

	//displayBackbuffer = al_get_backbuffer(display);
	displayBitmap = al_get_target_bitmap();
	systemTargetBitmap = al_create_bitmap(TARGET_BITMAP_WIDTH, TARGET_BITMAP_HEIGHT);
	if (!systemTargetBitmap) {
		fprintf(stderr, "GameState::init(): failed to create target bitmap\n");
		return false;
	}
	bitmapWidth = TARGET_BITMAP_WIDTH;
	bitmapHeight = TARGET_BITMAP_HEIGHT;

	registerEventSources();

	return true;
}

void GameState::initMap() {
	initMap(mapWidth, mapHeight);
}

void GameState::initMap(int w, int h) {
	mapWidth = w;
	mapHeight = h;
	if (!scroller)
		scroller = new Scroller();
	scroller->init();
}

void GameState::update() {
	// update routine
	al_get_keyboard_state(&keyState);
	scroller->update();
}

int GameState::getCurrentGameState() const {
	return curGameState;
}

void GameState::setCurrentGameState(int state) {
	curGameState = state;
}

ALLEGRO_EVENT_QUEUE* GameState::getEventQueue() const {
	return eventQueue;
}

ALLEGRO_FONT* GameState::getBuiltinFont() const {
	return builtinFont;
}

ALLEGRO_FONT* GameState::getSystemFont(int type, int size) const {
	if (type == GAME_STATE_SYSTEM_FONT_SANS) {
		switch (size) {
		case 12:
			return sans12;
		case 14:
			return sans14;
		case 16:
			return sans16;
		case 18:
			return sans18;
		default:
			fprintf(stderr, "E: Illegal font - [type: %d, size: %d]\n", type, size);
			break;
		}
	}
	else if (type == GAME_STATE_SYSTEM_FONT_SERIF) {
		switch (size) {
		case 12:
			return serif12;
		case 14:
			return serif14;
		case 16:
			return serif16;
		case 18:
			return serif18;
		default:
			fprintf(stderr, "E: Illegal font - [type: %d, size: %d]\n", type, size);
			break;
		}
	}
	else {
		fprintf(stderr, "E: Illegal font type %d", type);
		return nullptr;
	}

	return nullptr;
}

ALLEGRO_BITMAP* GameState::setTargetBitmap(ALLEGRO_BITMAP* target) {
	ALLEGRO_BITMAP* targetOrigin = getCurrentTargetBitmap();
	al_set_target_bitmap(target);
	return targetOrigin;
}

ALLEGRO_BITMAP* GameState::getCurrentTargetBitmap() const {
	return al_get_target_bitmap();
}

ALLEGRO_BITMAP* GameState::getSystemTargetBitmap() const {
	return systemTargetBitmap;
}

ALLEGRO_BITMAP* GameState::getDisplayBitmap() const {
	return displayBitmap;
}

ALLEGRO_BITMAP* GameState::setNewTargetBitmap(ALLEGRO_BITMAP* bitmap) {
	ALLEGRO_BITMAP* origin = al_get_target_bitmap();
	al_set_target_bitmap(bitmap);
	return origin;
}

int GameState::getTargetBitmapWidth() const {
	return bitmapWidth;
}

int GameState::getTargetBitmapHeight() const {
	return bitmapHeight;
}

// targetBitmap을 target bitmap으로 한다.
void GameState::beginSystemBitmapContext() {
	al_set_target_bitmap(systemTargetBitmap);
}

// Display의 backbuffer를 target bitmap으로 한다.
void GameState::beginDisplayBitmapContext() {
	al_set_target_bitmap(displayBitmap);
}

int GameState::getDisplayWidth() const {
	return displayWidth;
}

int GameState::getDisplayHeight() const {
	return displayHeight;
}

bool GameState::setDisplayWidth(int w) {
	return setDisplaySize(w, displayHeight);
}

bool GameState::setDisplayHeight(int h) {
	return setDisplaySize(displayWidth, h);
}

bool GameState::setDisplaySize(int w, int h) {
	displayWidth = w;
	displayHeight = h;
	return al_resize_display(display, w, h);
}

int GameState::getMapWidth() const {
	return mapWidth;
}

int GameState::getMapHeight() const {
	return mapHeight;
}

void GameState::setMapSize(int w, int h) {
	mapWidth = w;
	mapHeight = h;
}

void GameState::setGameTitle(const char* t) {
	strcpy(gameTitle, t);
	al_set_window_title(display, gameTitle);
}

Scroller* GameState::getScroller() const {
	return scroller;
}

Player* GameState::getCurrentPlayer() const {
	return ptPlayer;
}

void GameState::setCurrentPlayer(Player* pt) {
	ptPlayer = pt;
}

void GameState::startTimer() {
	al_start_timer(timer);
}

void GameState::stopTimer() {
	al_stop_timer(timer);
}

void GameState::setDisplayFlag(int f) {
	newDisplayFlags = f;
	if (!al_set_display_flag(display, newDisplayFlags, true))
		fprintf(stderr, "E: could not update new display flags\n");
}

const char* GameState::getGameTitle() const {
	return gameTitle;
}

// I/O Routines
const bool* GameState::getKeyState() const {
	return pressedKeys;
}

bool GameState::isKeyDown(int keycode) const {
	//return pressedKeys[keycode];
	return al_key_down(&keyState, keycode);
}

bool GameState::isKeyUp(int keycode) const {
	//return !pressedKeys[keycode];
	return !al_key_down(&keyState, keycode);
}


void GameState::setKeyDown(int keycode) {
	pressedKeys[keycode] = true;
}

void GameState::setKeyUp(int keycode) {
	pressedKeys[keycode] = false;
}

void GameState::setPrevKeyDown(int keycode) {
	prevPressedKeys[keycode] = true;
}

void GameState::setPrevKeyUp(int keycode) {
	prevPressedKeys[keycode] = false;
}

bool GameState::getPrevKeyDown(int keycode) const {
	return prevPressedKeys[keycode];
}

bool GameState::getPrevKeyUp(int keycode) const {
	return !prevPressedKeys[keycode];
}

void GameState::saveToPrevKeyState() {
	prevKeyState = keyState;
}

void GameState::onDisplayResize() {
	displayWidth = al_get_bitmap_width(displayBitmap);
	displayHeight = al_get_bitmap_height(displayBitmap);
	printf("dw, dh: %d, %d\n", displayWidth, displayHeight);
}

bool GameState::isFullscreen() const {
	return fullscreen;
}

void GameState::toggleFullscreen() {
	fullscreen = !fullscreen;

	al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, fullscreen);
	displayWidth = al_get_display_width(display);
	displayHeight = al_get_display_height(display);
	printf("width: %d, height: %d\n", displayWidth, displayHeight);
}

void GameState::screenshot(const char *path) {
	al_save_bitmap(path, systemTargetBitmap);
}
