#pragma once
#ifndef BONUS_H
#define BONUS_H

#include "raylib.h"

class Bonus
{
public:
	Bonus(Rectangle b) :body(b) {}
	void setTexture(const char* filename) { texture = LoadTexture(filename); }
	void setFrame() { frame = { 0.0f,0.0f,(float)texture.width,(float)texture.height }; }
	Texture2D& getTexture() { return texture; }
	Rectangle& getFrame() { return frame; }
	Rectangle& getBody() { return body; }
	void minusY(float y) { body.y -= y; }
	void setX(float x) { body.x = x; }

private:
	Rectangle frame;
	Rectangle body;
	Texture2D texture;
};


#endif // !

