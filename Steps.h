#pragma once
#ifndef STEPS_H
#define STEPS_H

#include <raylib.h>

class Steps
{
public:
	Steps(Rectangle s, int d) : step(s), dir(d) {}
	void setSpeed(int d) { dir = d; }
	Rectangle getStep() { return step; }
	void setX(float x) {step.x = x;}
	void setY(float y) {step.y = y;}
private:
	Rectangle step;
	int dir;
};

#endif // !STEPS_H
