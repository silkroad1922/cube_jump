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
#include <fstream>

class GameBoard
{
public:
	GameBoard() { Init(); }
	~GameBoard() { CloseWindow(); UnloadTexture(background); UnloadTexture(monster); UnloadTexture(cloud); }
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
	Camera2D camera;
	bool jump;
	bool gameOver;
	bool visible_player;
	Rectangle prev;
	Rectangle rocket_rectangle;
	Rectangle black_hole_rectangle;
	float low_point;
	float low_point_prev;
	float y_random;
	float x_random;
	float background_scroll;
	Texture2D monster;
	Texture2D background;
	Texture2D cloud;
	Texture2D rocket;
	Texture2D black_hole;
	Rectangle frameRocket;		
	Rectangle framePlayer; //?
	Rectangle frameBlack_hole;
	std::vector<Rectangle> frameStep;
	int pixel;//отступ от леовй стороны до ноги персонажа
	int score;
	float dt;
};


#endif // !GAME_BOARD_H
