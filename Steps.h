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
private:
	Rectangle step;
	int dir;
};

#endif // !STEPS_H
