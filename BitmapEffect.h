#pragma once
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

struct ALLEGRO_BITMAP;

// 화면(Bitmap) 효과의 조상 클래스
// 다음 클래스를 상속해서 구현할 것
class BitmapEffect
{
private:
	ALLEGRO_BITMAP* target;

public:
	bool load(const ALLEGRO_BITMAP* t);

};