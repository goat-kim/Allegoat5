#pragma once
#include "Util.h"
struct ALLEGRO_BITMAP;

class Sprite {
private:
	ALLEGRO_BITMAP* bitmap;
	int bitmapWidth;
	int bitmapHeight;
	char* filename;
	float cx, cy;			// centerX, centerY (0.0~1.0)
	float x, y;
	float angle;			// radian clockwise
	float scaleX, scaleY;
	int flag;				// ALLEGRO_FLIP_HORIZONTAL, ALLEGRO_FLIP_VERTICAL
	bool visible;
	unsigned char alpha;
public:
	Sprite();
	~Sprite();

	bool load(const char* filename);
	bool isOpen() const;

	void draw();
	void draw(const Rect* region);
	
	ALLEGRO_BITMAP* getBitmap() const;
	const char* getFilename() const;
	int getBitmapWidth() const;
	int getBitmapHeight() const;
	int getWidth() const;
	int getHeight() const;
	float getX() const;
	float getY() const;
	float getCenterX() const;
	float getCenterY() const;
	float getAngle() const;
	float getScaleX() const;
	float getScaleY() const;
	unsigned char getAlpha() const;
	bool isFlipHorizontal() const;
	bool isFlipVertical() const;
	bool isVisible() const;

	void setColorKey(int r, int g, int b);

	void setX(float x);
	void setY(float y);
	void setCenterX(float cx);
	void setCenterY(float cy);
	void setScaleX(float sx);
	void setScaleY(float sy);
	void setAngle(float a);
	void setFlipHorizontal(bool f);
	void setFlipVertical(bool f);

	void setXY(float x, float y);
	void setCenter(float cx, float cy);
	void setScale(float s);
	void setScale(float sx, float sy);
	void setAlpha(unsigned char a);
	void setVisible(bool v);
};