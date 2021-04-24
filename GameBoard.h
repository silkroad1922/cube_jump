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
#include <cmath>
#include "Bonus.h"



class GameBoard
{
public:
	GameBoard() { Init(); }
	~GameBoard() { CloseWindow(); UnloadTexture(background); UnloadTexture(player->getTexture()); }
	void Init();
	void update();
	void handlerKeyboard();
	void draw();
	void jumping();
	bool myCheckCollision(Rectangle, Rectangle, Rectangle);
	void checkPlayZone();
	void cameraUpdate();
	void handlerBlack_hole();

private:
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Steps>> steps;
	std::unique_ptr<Bonus> rocket;
	std::unique_ptr<Bonus> black_hole;
	Camera2D camera;
	bool jump;
	bool gameOver;
	Rectangle prev;
	float low_point;
	float low_point_prev;
	float y_random;
	float x_random;
	float background_scroll;
	Texture2D background;
	int pixel;//отступ от леовй стороны до ноги персонажа
	int score;
	float dt;
};


#endif // !GAME_BOARD_H
