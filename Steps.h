#pragma once
#ifndef STEPS_H
#define STEPS_H

#include <raylib.h>

class Steps
{
public:
	Steps(Rectangle s) : step(s) {}
	Rectangle getStep() { return step; }
	void setX(float x) {step.x = x;}
	void setY(float y) {step.y = y;}
	Vector2 getPosition() { return Vector2{ step.x,step.y }; }
private:
	Rectangle step;
};

#endif // !STEPS_H
