#pragma once
//#include <ctime>
//#include <cstdio>
#include <vector>
#include <allegro5/utf8.h>

//#define BUF_SIZE	256
#define BUF_SIZE	512
//const int DEFAULT_DISPLAY_WIDTH = 1280;
//const int DEFAULT_DISPLAY_HEIGHT = 1024;
const int DEFAULT_DISPLAY_WIDTH = 320;
const int DEFAULT_DISPLAY_HEIGHT = 240;

const int TARGET_BITMAP_WIDTH = 320;
const int TARGET_BITMAP_HEIGHT = 240;

typedef struct Rect {
	int x, y;
	int width, height;
	Rect(int _x = 0, int _y = 0, int _w = 0, int _h = 0)
		: x(_x), y(_y), width(_w), height(_h) {	}
	Rect(const Rect& rc)
		: Rect(rc.x, rc.y, rc.width, rc.height) {}
	//Rect() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {  }
} Rect;

typedef struct ColorRGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	ColorRGB(unsigned char _r = 0, unsigned char _g = 0, unsigned char _b = 0)
		: r(_r), g(_g), b(_b) {}
} ColorRGB;

// 시간, 타임스탬프 관련 루틴
// 스크린샷이나 게임 세이브, 로드
const char* getCurrentTimestamp(void);
void ustrDisplayCodes(ALLEGRO_USTR* str);

inline int ustrGetChar(ALLEGRO_USTR* str, int idx) {
	return al_ustr_get(str, idx);
}

inline int ustrGetNextChar(ALLEGRO_USTR* str, int* idx) {
	return al_ustr_get_next(str, idx);
}

inline void ustrSetChar(ALLEGRO_USTR* str, int idx, int32_t code) {
	al_ustr_set_chr(str, idx, code);
}

// https://liballeg.org/a5docs/trunk/utf8.html
// ALLEGRO_USTR
// ALLEGRO_USTR_INFO
// al_ref_buffer
// al_draw_ustr


// ALLEGRO_USTR *al_ustr_new(const char *s)
// ALLEGRO_USTR *al_ustr_new_from_buffer(const char *s, size_t size)
// ALLEGRO_USTR* al_ustr_newf(const char* fmt, ...)
// void al_ustr_free(ALLEGRO_USTR *us)
// const char* al_cstr(const ALLEGRO_USTR* us)
// void al_ustr_to_buffer(const ALLEGRO_USTR *us, char *buffer, int size)

// std::list
// std::vector


// ustr list from file
// 
class UstrVector {
private:
	std::vector<ALLEGRO_USTR*> vec;
public:
	UstrVector();
	~UstrVector();
	bool load(const char* pathname);
	ALLEGRO_USTR* ustrAt(int idx) const;
	ALLEGRO_USTR* ustrFront() const;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	ALLEGRO_USTR* ustrBack() const;
	size_t length() const;
};