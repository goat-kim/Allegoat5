#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

//#include "Util.h"
#include "GameState.h"
#include "SpriteAnimation.h"
#include "Player.h"
#include "Npc1.h"
#include "Sound.h"
#include "Scroller.h"
#include "Tilemap.h"
#include "UI.h"

bool allegroInit() {
	printf("allegroInit()\n");

	if (!al_init()) {
		fprintf(stderr, "E: failed to initialize allegro\n");
		return false;
	}

	if (!al_init_primitives_addon()) {
		fprintf(stderr, "E: failed to initialize primitive addon\n");
		return false;
	}

	if (!al_init_image_addon()) {
		fprintf(stderr, "E: failed to initialize image addon\n");
		return false;
	}
	if (!al_init_font_addon()) {
		fprintf(stderr, "E: failed to initialize font addon\n");
		return false;
	}
	if (!al_init_ttf_addon()) {
		fprintf(stderr, "E: failed to initialize ttf addon\n");
		return false;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "E: failed to initialize keyboard\n");
		return false;
	}
	if (!al_install_mouse()) {
		fprintf(stderr, "E: failed to initialize mouse\n");
		return false;
	}
	if (!al_install_audio()) {
		fprintf(stderr, "E: failed to initialize audio\n");
		return false;
	}
	if (!al_init_acodec_addon()) {
		fprintf(stderr, "E: failed to initialize acodec addon\n");
		return false;
	}

	return true;
}

void cleanUp(void) {
	GameState::destroyInstance();
}

bool loadResources(void) {
	// 
	return true;
}

int main(int argc, char* argv[])
{
	if (!allegroInit()) {
		fprintf(stderr, "Failed to initialize allegro system: program aborted\n");
		return -1;
	}

	GameState* gameState = GameState::getInstance();
	//if (!gameState->init()) {
	if (!gameState->init(640, 480)) {
		fprintf(stderr, "Failed to initialize game state: program aborted\n");
		//delete gameState;
		return -1;
	}
	//gameState->setDisplayFlag(ALLEGRO_FRAMELESS);
	gameState->setGameTitle("Goat Simulator");

	const char* chrFilename = "img/chr1.png";
	const char* bgFilename = "img/bg1.png";
	const char* splashFilename = "img/splash-white.png";
	
	bool mainLoop = true;
	bool redraw = false;

	ALLEGRO_EVENT ev;

	ALLEGRO_FONT* font1 = al_load_ttf_font("font/consola.ttf", 12, 0);
	char buf[256];		//
	char vpLog[256];	// current view port
	char vxyLog[256];	// current vx, vy

	al_reserve_samples(10);
	//Sound *sound1 = new Sound();
	//sound1->load("bgm/Town2.ogg");

	Sprite *sprSplashScreen = new Sprite();
	sprSplashScreen->load(splashFilename);

	Sprite* sprBG1 = new Sprite();
	float bgScale = 1.0f;
	sprBG1->load(bgFilename);
	sprBG1->setScale(bgScale);

	Player* player = new Player();
	if (!player->init("img/Animal.png")) {
		fprintf(stderr, "could not initialize object 'player'\n");
		return -1;
	}

	//player->setScale(5.0f);
	player->setColorKey(0x20, 0x9c, 0x00);
	player->setAnimationSpeed(ANIM_NORMAL);
	player->setXY(200, 130);
	player->setDirect(DIRECT_RIGHT);
	player->setSpeed(2.0f);
	//player->setBoundaryBoxVisible(true);
	//player->setScrollMode(false);
	//player->setXY(gameState->getDisplayWidth() / 2, gameState->getDisplayHeight() / 2);

	Npc1* npc1 = new Npc1();
	if (!npc1->init("img/Actor2.png")) {
		fprintf(stderr, "could not initialize object 'npc1'\n");
		return -1;
	}
	npc1->setScrollMode(false);
	//npc1->setScale(5.0f);
	npc1->setColorKey(0x20, 0x9c, 0x00);
	npc1->setAnimationSpeed(ANIM_NORMAL);
	npc1->setXY(200, 200);
	printf("npc1.x=%f, npc1.y=%f\n", npc1->getX(), npc1->getY());

	GameObject* npc2 = new GameObject();
	if (!npc2->init("img/Actor2.png")) {
		fprintf(stderr, "could not initialize object 'npc2'\n");
		return -1;
	}
	npc2->setScrollMode(false);
	npc2->setScale(3.0f);
	npc2->setColorKey(0x20, 0x9c, 0x00);
	npc2->setAnimationSpeed(ANIM_NORMAL);
	npc2->setXY(500, 500);
	npc2->setDirect(DIRECT_RIGHT);


	gameState->setCurrentPlayer(player);
	// 배경 사진(sprBG1)의 크기를 기준으로 맵 크기 설정
	//gameState->setMapSize(sprBG1->getWidth(), sprBG1->getHeight());
	//gameState->initMap();
	//

	Tileset* tilesetMap1 = new Tileset();
	tilesetMap1->load("img/Interior.png");

	TilemapLayer* layer1 = new TilemapLayer(tilesetMap1);
	//map1Layer1->load("map/map1.csv");
	//layer1->setTileset(tilesetMap1);
	layer1->load("map/kemo.csv");

	layer1->draw();

	gameState->setMapSize(layer1->getMapWidthPx(), layer1->getMapHeightPx());
	printf("map size: [%d, %d]\n", gameState->getMapWidth(), gameState->getMapHeight());
	gameState->initMap();

	gameState->startTimer();
	//sound1->play(true);

	Scroller* ptScroll = gameState->getScroller();
	printf("scroller fixed: %d %d\n", ptScroll->isFixX(), ptScroll->isFixY());
	const Rect* vp = nullptr;

	//gameState->setDisplaySize(320, 240);
	int displayWidth = gameState->getDisplayWidth();
	int displayHeight = gameState->getDisplayHeight();

	bool onDisplayTest = false;

	int keycode;

	bool ustrDisp = false;
	int rateCnt = 0;
	int rate = 3;
	int ustrIdx = 0;

	DialogBox* dbox = new DialogBox();
	dbox->loadScript("text/text2.txt");
	dbox->loadUISprite("img/System.png");
	//FF9C00
	dbox->setUISpriteColorKey(0xff, 0x9c, 0x00);
	dbox->setMargin(5, 5, 10, 10);
	dbox->setPadding(5, 5, 10, 5);
	dbox->setFrameVisible(true);
	dbox->setAlpha(128);
	bool textAreaVisible = false;

	bool boundBoxVisible = false;

	printf("dbox.width: %d\n", dbox->getWidth());

	// On test ========================================
	//gameState->setCurrentGameState(GAME_STATE_INIT); // init 시점이나 스플래시 화면에 필요 데이터 메모리에 로드
	gameState->setCurrentGameState(GAME_STATE_SPLASH); // 스플래시 화면 출력
	int curGameState = gameState->getCurrentGameState();
	int splashCnt = 60;
	Sprite *sprTitle = new Sprite();
	sprTitle->load("img/title-sample3.png");
	// ================================================

	// Test Variables =================================
	int uiDockStateIdx = 0;
	int retKeyCount = -1;
	const int BLINK_RATE = 40;
	int blinkCnt = BLINK_RATE;
	bool drawEn = true;
	// ================================================

	while (mainLoop) {
		al_wait_for_event(gameState->getEventQueue(), &ev);
		//al_get_keyboard_state(keyState);

		switch (ev.type) {
		case ALLEGRO_EVENT_TIMER:
			/* Update routine ======================================================================== */
			redraw = true;

			gameState->update();

			curGameState = gameState->getCurrentGameState();
			if (curGameState == GAME_STATE_RUNNING) {
				player->update();
				npc1->update();
				npc2->update();
				dbox->update();

				if (player->aabbIntersection(npc1->getBoundaryBox())) {
					npc1->setCollision(true);
					player->setCollision(true);
				}
				else {
					npc1->setCollision(false);
					player->setCollision(false);
				}

				vp = ptScroll->getViewPort();
				sprintf(buf, "position: [%f, %f]", player->getX(), player->getY());
				sprintf(vpLog, "view port: [%d, %d, %d, %d]", vp->x, vp->y, vp->width, vp->height);
				sprintf(vxyLog, "velocity: [%f, %f]", player->getVx(), player->getVy());
				//sprintf(gameState->getScroller()->getScaledViewPort(10.0f));
				//scroll->update();
				//printf("vp.diff: [%d, %d]\n", scroll->getDx(), scroll->getDy());
			}
			else if (curGameState == GAME_STATE_SPLASH) {
				if (splashCnt <= 0) {
					gameState->setCurrentGameState(GAME_STATE_TITLE);
					printf("Set current game state to GAME_STATE_TITLE\n");
				}
				//printf("splashCnt: %d\n", splashCnt);
				splashCnt--;
			}

			else if (curGameState == GAME_STATE_TITLE) {
				if (gameState->isKeyDown(ALLEGRO_KEY_ENTER)) {
					// 키를 떼고 있으면 -1, 누르면 프레임 당 1씩 감소
					if (retKeyCount == -1) {
						retKeyCount = 1;
					}
					else if (retKeyCount == 0) {
						gameState->setCurrentGameState(GAME_STATE_RUNNING);
					}
					retKeyCount--;
				}

				if (gameState->isKeyUp(ALLEGRO_KEY_ENTER)) {
					retKeyCount = -1;
				}
				blinkCnt--;
				if (blinkCnt == 0) {
					drawEn = !drawEn;
					blinkCnt = BLINK_RATE;
				}
			}

			if (gameState->isKeyDown(ALLEGRO_KEY_ESCAPE))
				mainLoop = false;
			break;
			/* ======================================================================================= */

		case ALLEGRO_EVENT_KEY_DOWN:
			//if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			//	mainLoop = false;
			//g_pressedKeys[ev.keyboard.keycode] = true;
			
			keycode = ev.keyboard.keycode;
			gameState->setKeyDown(ev.keyboard.keycode);
			
			if (keycode == ALLEGRO_KEY_SPACE) {
				//printf("sound1: %d\n", sound1->play());
				//printf("A sample [id=%d, idx=%d] is played\n", sid._id, sid._index);
			}
			else if (keycode == ALLEGRO_KEY_BACKSPACE) {
				//Sound::stopAll();
			}
			else if (keycode == ALLEGRO_KEY_F1) {
				gameState->setDisplaySize(640, 480);
				printf("Set display size to 640x480\n");
			}
			else if (keycode == ALLEGRO_KEY_F2) {
				gameState->setDisplaySize(320, 240);
				printf("Set display size to 320x240\n");
			}
			else if (keycode == ALLEGRO_KEY_F3) {
				gameState->setDisplaySize(1280, 1024);
				printf("Set display size to 1280x1024\n");
			}
			else if (keycode == ALLEGRO_KEY_F4) {
				gameState->toggleFullscreen();
				if (gameState->isFullscreen())
					printf("Fullscreen on\n");
				else
					printf("Fullscreen off\n");
			}
			else if (keycode == ALLEGRO_KEY_F5) {
				char pathname[50];
				sprintf(pathname, "screenshots/screenshot_%s.png", getCurrentTimestamp());
				gameState->screenshot(pathname);
				printf("screenshot is saved in %s\n", pathname);
				
			}
			else if (keycode == ALLEGRO_KEY_F6) {
				onDisplayTest = !onDisplayTest;
			}
			// F7: DialogBox Test
			else if (keycode == ALLEGRO_KEY_F7) {
				dbox->show(0);
			}
			// F8: Draw Text Area (=DialogBox's Content Area)
			else if (keycode == ALLEGRO_KEY_F8) {
				textAreaVisible = !textAreaVisible;
			}
			else if (keycode == ALLEGRO_KEY_F9) {
				dbox->setRate(5);
				printf("Set message rate to 5\n");
			}
			else if (keycode == ALLEGRO_KEY_F10) {
				dbox->setRate(10);
				printf("Set message rate to 10\n");
			}
			else if (keycode == ALLEGRO_KEY_F11) {
				dbox->setRate(0);
				printf("Set message rate to 0\n");
			}
			else if (keycode == ALLEGRO_KEY_F12) {
				boundBoxVisible = !boundBoxVisible;
			}
			else if (keycode == ALLEGRO_KEY_0) {
				ustrDisp = true;
				ustrIdx = 0;
				rateCnt = 0;
			}
			/* UI Frame Test */
			else if (keycode == ALLEGRO_KEY_Q) {
				uiDockStateIdx--;
			}
			else if (keycode == ALLEGRO_KEY_E) {
				uiDockStateIdx++;
			}

			if (keycode == ALLEGRO_KEY_Q || keycode == ALLEGRO_KEY_E) {
				if (uiDockStateIdx < 0)
					uiDockStateIdx = 6;
				else if (uiDockStateIdx > 6)
					uiDockStateIdx = 0;
				switch (uiDockStateIdx) {
				case 0:
					dbox->dockBottom();
					printf("dbox->dockBottom()\n");
					break;
				case 1:
					dbox->dockTop();
					printf("dbox->dockTop()\n");
					break;
				case 2:
					dbox->dockMiddle();
					printf("dbox->dockMiddle()\n");
					break;
				case 3:
					dbox->dockBottomHalf();
					printf("dbox->dockBottomHalf()\n");
					break;
				case 4:
					dbox->dockTopHalf();
					printf("dbox->dockTopHalf()\n");
					break;
				case 5:
					dbox->dockLeftHalf();
					printf("dbox->dockLeftHalf()\n");
					break;
				case 6:
					dbox->dockRightHalf();
					printf("dbox->dockRightHalf()\n");
					break;
				}
				dbox->reallocUITargetBitmap();
			}

			displayWidth = gameState->getDisplayWidth();
			displayHeight = gameState->getDisplayHeight();
			break;
		case ALLEGRO_EVENT_KEY_UP:
			//g_pressedKeys[ev.keyboard.keycode] = false;
			gameState->setKeyUp(ev.keyboard.keycode);
			break;
		case ALLEGRO_EVENT_DISPLAY_RESIZE:
			// issue: 화면 크기 재조정해도 display width, height 값 변하지 않음
			gameState->onDisplayResize();
			displayWidth = gameState->getDisplayWidth();
			displayHeight = gameState->getDisplayHeight();
			printf("Display is resized to %d, %d\n", displayWidth, displayHeight);
			break;
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			mainLoop = false;
			break;
		}

		if (!mainLoop)
			break;

		if (redraw && al_is_event_queue_empty(gameState->getEventQueue())) {
			curGameState = gameState->getCurrentGameState();
			
			if (curGameState == GAME_STATE_RUNNING) {
				/* targetBitmap에 먼저 게임 화면을 draw한다. */
				gameState->beginSystemBitmapContext();
				al_clear_to_color(al_map_rgb(0, 0, 255));

				//sprBG1->draw();
				//sprBG1->draw(gameState->getScroller()->getScaledViewPort(bgScale));
				//sprBG1->draw(scroll->getViewPort());

				/* 레이어 별 맵 출력 */
				const Rect* rc = gameState->getScroller()->getScaledViewPort(bgScale);
				al_draw_bitmap_region(layer1->getLayerBitmap(), rc->x, rc->y, rc->width, rc->height, 0, 0, 0);
				//al_draw_bitmap(map1Layer1->getLayerBitmap(), 0, 0, 0);

				/* 플레이어 및 게임 오브젝트 출력 */
				player->draw();
				npc1->draw();
				npc2->draw();

				if (boundBoxVisible) {
					player->drawBoundaryBox();
					npc1->drawBoundaryBox();
					npc2->drawBoundaryBox();
				}
				//sprintf(buf, "[%f, %f]", spr1->getX(), spr1->getY());
				//al_draw_text(gameState->getBuiltinFont(), al_map_rgb(255, 255, 255), 10.0f, 10.0f, ALLEGRO_ALIGN_LEFT, buf);

				/* UI 출력 */
				// UI 출력 우선순위는 맵과 플레이어, 게임 오브젝트 다음으로
				//dbox->drawUIArea();
				dbox->draw();

				if (textAreaVisible) {
					Rect rcText = dbox->getContentArea();
					al_draw_rectangle(rcText.x, rcText.y, rcText.x + rcText.width, rcText.y + rcText.height, al_map_rgb(0xff, 0, 0), 1.0f);
				}

				// Draw in-game text =====================================================================================
				//al_draw_ustr(gameState->getSystemFont(GAME_STATE_SYSTEM_FONT_SANS, 12), al_map_rgb(255, 255, 0), 10, 40, 0, usMsg2);
				//al_draw_ustr(gameState->getSystemFont(GAME_STATE_SYSTEM_FONT_SANS, 14), al_map_rgb(255, 255, 0), 10, 60, 0, usMsg2);
				//al_draw_text(gameState->getBuiltinFont(), al_map_rgb(255, 255, 0), 10, 80, 0, "Builtin Font Test");
				// =======================================================================================================
				
				/* display bitmap에 앞서 렌더링된 게임 화면(targetBitmap)을 크기에 맞게 draw한다. */
				gameState->beginDisplayBitmapContext();
				al_clear_to_color(al_map_rgb(0, 0, 0));

				if (onDisplayTest) {
					al_draw_bitmap(gameState->getSystemTargetBitmap(), 0, 0, 0);
					al_draw_scaled_bitmap(gameState->getSystemTargetBitmap(), 0, 0,
						gameState->getTargetBitmapWidth(), gameState->getTargetBitmapHeight(),
						100, 300, 640, 480, 0);
					al_draw_scaled_bitmap(gameState->getSystemTargetBitmap(), 0, 0,
						gameState->getTargetBitmapWidth(), gameState->getTargetBitmapHeight(),
						800, 200, 200, 700, 0);
					al_draw_scaled_bitmap(gameState->getSystemTargetBitmap(), 0, 0,
						gameState->getTargetBitmapWidth(), gameState->getTargetBitmapHeight(),
						150, 800, 600, 100, 0);
				}
				else {
					al_draw_scaled_bitmap(gameState->getSystemTargetBitmap(),
						0, 0,
						gameState->getTargetBitmapWidth(), gameState->getTargetBitmapHeight(),
						0, 0,
						displayWidth, displayHeight, 0);
				}

				/* 화면에 출력되는 로그는 화면 크기와 독립적으로 출력한다. */
				al_draw_text(font1, al_map_rgb(255, 255, 255), 10, 10, 0, buf);
				al_draw_text(font1, al_map_rgb(255, 255, 255), 10, 30, 0, vpLog);
				al_draw_text(font1, al_map_rgb(255, 255, 255), 10, 50, 0, vxyLog);
				al_flip_display();
			}
			else if (curGameState == GAME_STATE_SPLASH) {
				gameState->beginSystemBitmapContext();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				sprSplashScreen->draw();

				gameState->beginDisplayBitmapContext();
				//al_draw_bitmap(gameState->getSystemTargetBitmap(), 0, 0, 0);
				al_draw_scaled_bitmap(gameState->getSystemTargetBitmap(),
					0, 0,
					gameState->getTargetBitmapWidth(), gameState->getTargetBitmapHeight(),
					0, 0,
					displayWidth, displayHeight, 0);
				al_flip_display();
			}
			else if (curGameState == GAME_STATE_TITLE) {
				gameState->beginSystemBitmapContext();
				al_clear_to_color(al_map_rgb(0, 0, 0));
				sprTitle->draw();
				if (drawEn) {
					al_draw_text(gameState->getBuiltinFont(), 
						al_map_rgb(128, 128, 0), 160, 150, ALLEGRO_ALIGN_CENTRE, 
						"Press Enter to start game");
				}
				
				gameState->beginDisplayBitmapContext();
				al_draw_scaled_bitmap(
					gameState->getSystemTargetBitmap(),
					0,
					0,
					gameState->getTargetBitmapWidth(),
					gameState->getTargetBitmapHeight(),
					0,
					0,
					displayWidth,
					displayHeight,
					0);
				al_flip_display();
			}

			/* End Drawing */
			redraw = false;
		}
	}

	al_destroy_font(font1);

	delete sprTitle;

	delete dbox;

	delete npc2;
	delete npc1;
	delete player;

	delete layer1;
	delete tilesetMap1;

	delete sprBG1;
	cleanUp();

	return 0;
}
