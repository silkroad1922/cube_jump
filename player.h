#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include "Constants.h"

class Player
{
public:
	Player(Rectangle b) :body(b) {}
	Vector2 GetXY() { return { body.x,body.y }; }
	void moveX(float x) { body.x += x; }
	void moveY(float y) { body.y += y; }
	Rectangle getBody() { return body; }
	void setY(float y) { body.y = y; }
	void setX(float x) { body.x = x; }
	
	float Velocity = 500.0f;

private:
	Rectangle body;
};


#endif // !PLAYER_H
