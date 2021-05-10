#pragma once
#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "Bonus.h"

class Enemy : public Bonus
{
public:
	enum class Dir {RIGHT,LEFT};

	Enemy(Rectangle r) : Bonus(r){}
	void setDir(Dir d){dir = d;}
	Dir getDir(){return dir;}
	void plusX(float x){getBody().x += x;}
	void minusX(float x) {getBody().x -= x;}

	Rectangle bullet;

private:
	Dir dir;
};


#endif