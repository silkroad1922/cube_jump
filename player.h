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
	void setFrame() { frame = { 0.0f,0.0f,(float)texture.width,(float)texture.height }; }
	void setTexture(const char* filename) { texture = LoadTexture(filename); }
	void setVisible(bool v) { visible = v; }
	Rectangle& getFrame() { return frame; }
	const Rectangle& getFrame() const  { return frame; } 
	Texture2D& getTexture() { return texture; }
	bool getVisible() { return visible; }
	void setBonus(bool b) { bonus = b; }
	bool getBonus() { return bonus; }

	float Velocity = 500.0f;
private:
	Rectangle body;
	Rectangle frame;
	Texture2D texture;
	bool visible;
	bool bonus;

};


#endif // !PLAYER_H
