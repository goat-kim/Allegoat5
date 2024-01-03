#include "Sprite.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <cstdio>
#include <cstring>

Sprite::Sprite()
	: bitmap(nullptr), bitmapWidth(0), bitmapHeight(0), filename(nullptr),
	cx(0.0f), cy(0.0f), x(0.0f), y(0.0f),
	angle(0.0f), scaleX(1.0f), scaleY(1.0f),
	flag(0), visible(true), alpha(0xff) {
}

Sprite::~Sprite() {
	printf("~Sprite(): %s\n", getFilename());
	if (filename)
		delete[] filename;
	//printf("string deleted\n");
	al_destroy_bitmap(bitmap);
	//printf("bitmap destroyed\n");
}

bool Sprite::load(const char* filename) {
	this->filename = new char[strlen(filename) + 1];
	memset(this->filename, 0, strlen(filename) + 1);
	strcpy(this->filename, filename);
	bitmap = al_load_bitmap(filename);
	if (!bitmap) {
		fprintf(stderr, "Sprite::open(): failed to load bitmap [%s]\n", filename);
		return false;
	}
	printf("Sprite::open(): %s\n", filename);
	bitmapWidth = al_get_bitmap_width(bitmap);
	bitmapHeight = al_get_bitmap_height(bitmap);
	return true;
}

void Sprite::draw() {
	//printf("draw() ");
	//al_draw_scaled_rotated_bitmap(bitmap, cx * bitmapWidth, cy * bitmapHeight, x, y, scaleX, scaleY, angle, flag);
	al_draw_tinted_scaled_rotated_bitmap(
		bitmap, 
		al_map_rgba(255, 255, 255, alpha), 
		cx * bitmapWidth, cy * bitmapHeight, 
		x, y, 
		scaleX, scaleY, 
		angle, 
		flag);
}

/* void al_draw_scaled_bitmap(ALLEGRO_BITMAP *bitmap,
   float sx, float sy, float sw, float sh,
   float dx, float dy, float dw, float dh, int flags) */
/* void al_draw_bitmap_region(ALLEGRO_BITMAP *bitmap,
   float sx, float sy, float sw, float sh, float dx, float dy, int flags) */
/* void al_draw_tinted_scaled_rotated_bitmap_region(ALLEGRO_BITMAP *bitmap,
   float sx, float sy, float sw, float sh,
   ALLEGRO_COLOR tint,
   float cx, float cy, float dx, float dy, float xscale, float yscale,
   float angle, int flags) */
void Sprite::draw(const Rect* region) {
	al_draw_tinted_scaled_rotated_bitmap_region(
		bitmap, 
		region->x, region->y,
		region->width, region->height,
		al_map_rgba(255, 255, 255, alpha),
		//cx * bitmapWidth, cy * bitmapHeight,
		cx * region->width, cy * region->height,
		x, y,
		scaleX, scaleY,
		angle,
		flag);
}

bool Sprite::isOpen() const {
	return (bool)bitmap;
}

const char* Sprite::getFilename() const {
	return filename;
}

int Sprite::getBitmapWidth() const {
	return bitmapWidth;
}

int Sprite::getBitmapHeight() const {
	return bitmapHeight;
}

int Sprite::getWidth() const {
	return bitmapWidth * scaleX;
}

int Sprite::getHeight() const {
	return bitmapHeight * scaleY;
}

ALLEGRO_BITMAP* Sprite::getBitmap() const {
	return bitmap;
}

float Sprite::getX() const {
	return x;
}

float Sprite::getY() const {
	return y;
}

float Sprite::getCenterX() const {
	return cx;
}

float Sprite::getCenterY() const {
	return cy;
}

float Sprite::getAngle() const {
	return angle;
}

float Sprite::getScaleX() const {
	return scaleX;
}

float Sprite::getScaleY() const {
	return scaleY;
}

unsigned char Sprite::getAlpha() const {
	return alpha;
}

bool Sprite::isFlipHorizontal() const {
	return flag & ALLEGRO_FLIP_HORIZONTAL;
}

bool Sprite::isFlipVertical() const {
	return flag & ALLEGRO_FLIP_VERTICAL;
}

bool Sprite::isVisible() const {
	return visible;
}

void Sprite::setColorKey(int r, int g, int b) {
	al_convert_mask_to_alpha(bitmap, al_map_rgb(r, g, b));
}

void Sprite::setX(float x) {
	this->x = x;
}

void Sprite::setY(float y) {
	this->y = y;
}

void Sprite::setCenterX(float cx) {
	this->cx = cx;
}

void Sprite::setCenterY(float cy) {
	this->cy = cy;
}

void Sprite::setAngle(float a) {
	this->angle = a;
}

void Sprite::setScaleX(float sx) {
	this->scaleX = sx;
}

void Sprite::setScaleY(float sy) {
	this->scaleY = sy;
}

void Sprite::setXY(float x, float y) {
	this->x = x;
	this->y = y;
}

void Sprite::setCenter(float cx, float cy) {
	setCenterX(cx);
	setCenterY(cy);
}

void Sprite::setScale(float s) {
	this->scaleX = this->scaleY = s;
}

void Sprite::setScale(float sx, float sy) {
	this->scaleX = sx;
	this->scaleY = sy;
}

void Sprite::setFlipHorizontal(bool f) {
	if (f)
		flag |= ALLEGRO_FLIP_HORIZONTAL;
	else
		flag &= ~ALLEGRO_FLIP_HORIZONTAL;
}

void Sprite::setFlipVertical(bool f) {
	if (f)
		flag |= ALLEGRO_FLIP_VERTICAL;
	else
		flag &= ~ALLEGRO_FLIP_VERTICAL;
}

void Sprite::setAlpha(unsigned char a) {
	alpha = a;
}

void Sprite::setVisible(bool v) {
	visible = v;
}