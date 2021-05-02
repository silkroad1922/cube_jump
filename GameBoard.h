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
#include "Button.h"
#include "TextureHolder.h"



class GameBoard
{
public:

	enum class Scene {MENU,MAIN};

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
	void handlerBlack_hole();
	void retry();

private:
	std::shared_ptr<TextureHolder> steps_texture;
	std::unique_ptr<Player> player;
	std::vector<std::unique_ptr<Steps>> steps;
	std::unique_ptr<Bonus> rocket;
	std::unique_ptr<Bonus> black_hole;
	std::unique_ptr<Button> button_start;
	std::unique_ptr<Button> button_retry;
	std::unique_ptr<Button> button_exit;
	Camera2D camera;
	bool jump;
	bool gameOver; 
	bool game;
	Rectangle prev;
	float low_point;
	float low_point_prev;
	float y_random;
	float x_random;
	float background_scroll;
	Texture2D background;
	Texture2D background_menu;
	int pixel;
	int score;
	float dt;
	Scene scene;
};


#endif // !GAME_BOARD_H
