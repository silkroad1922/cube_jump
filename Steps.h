#pragma once
#ifndef STEPS_H
#define STEPS_H

#include <raylib.h>
#include "TextureHolder.h"
#include <memory>

class Steps
{
public:
	Steps(Rectangle s) : step(s) {}
	Rectangle getStep() { return step; }
	void setX(float x) { step.x = x; }
	void setY(float y) { step.y = y; }
	Vector2 getPosition() { return Vector2{ step.x,step.y }; }

	void setTexture(const std::shared_ptr<TextureHolder>& t) { texture = t; }
	void setFrame() { frame = { 0.0f,0.0f,(float)texture->getTexture().width,(float)texture->getTexture().height }; }

	Rectangle& getFrame() { return frame; }
	Texture2D getTexture() { return texture->getTexture(); }

private:
	Rectangle step;
	std::shared_ptr<TextureHolder> texture;
	Rectangle frame;
};

#endif // !STEPS_H
