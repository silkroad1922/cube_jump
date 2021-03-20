#pragma once
#ifndef GAME_BOARD_H
#define GAME_BOARD_H
#include <raylib.h>
#include "player.h"
#include "Steps.h"
#include <vector>
#include "Constants.h"
#include <memory>
#include <time.h>
#include <cstdlib>


class GameBoard
{
public:
	GameBoard() { Init(); }
	~GameBoard() { CloseWindow(); }
	void Init();
	void update();
	void handlerKeyboard();
	void draw();
	void jumping();
	bool myCheckCollision(Rectangle, Rectangle, Rectangle);
	void checkPlayZone();
	void cameraUpdate();

private:
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Steps>> steps;
	Camera2D camera;
	bool jump;
	bool gameOver;
	Rectangle prev;
	float low_point;
	int count_steps;
};


#endif // !GAME_BOARD_H
