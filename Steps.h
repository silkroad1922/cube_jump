#pragma once
#ifndef STEPS_H
#define STEPS_H

#include <raylib.h>

class Steps
{
public:
	Steps(Rectangle s) : step(s) {}
	Rectangle getStep() { return step; }
	void setX(float x) { step.x = x; }
	void setY(float y) { step.y = y; }
	Vector2 getPosition() { return Vector2{ step.x,step.y }; }

	void setTexture(const char* filename) { texture = LoadTexture(filename); }
	void setFrame() { frame = { 0.0f,0.0f,(float)texture.width,(float)texture.height }; }

	Rectangle& getFrame() { return frame; }
	Texture2D& getTexture() { return texture; }

private:
	Rectangle step;
	Texture2D texture;
	Rectangle frame;
};

#endif // !STEPS_H
