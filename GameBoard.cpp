#include "GameBoard.h"

void GameBoard::Init()
{
	
	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "doodle_jump");


	background = LoadTexture("resource/space.png");
	background_menu = LoadTexture("resource/background_menu3.png");
	
	steps_texture = std::make_shared<TextureHolder>();
	steps_texture->setTexture("resource/test6.png");

	SetTargetFPS(60);
	jump = true;
	gameOver = false;
	game = true;

	pixel = 15;
	score = 0;
	dt = GetFrameTime();

	//login
	letterCount = 0;
	loginBox = { 85,450,225,50 };
	//

	//leader
	leader = { 340,680,50,10 };
	statistic.open("resource/statistic.txt" , std::fstream::in);
	statistic >> leader_score;
	statistic >> leader_name;
	statistic.close();
	//

	//step	
	for (size_t i = 0; i < NUMBER_OF_STEPS; i++)
	{
		x_random = GetRandomValue(0,350);
		if(i==0)
		{
			y_random = WINDOW_HEIGHT / 2 + i * GetRandomValue(-100,-50);
		}else
		{
			y_random = steps[i-1]->getStep().y - GetRandomValue(30,100);
		}

		steps.push_back(std::make_unique<Steps>(Rectangle{ x_random,y_random,STEPS_WIDTH,STEPS_HEIGHT }));
		steps[i]->setTexture(steps_texture);
		steps[i]->setFrame();
	}
	//

	//player
	player = std::make_unique<Player>(Rectangle{ 0,0,PLAYER_WIDHT,PLAYER_HEIGHT });
	player->setTexture("resource/pixel_5.png");
	player->setFrame();
	player->setVisible(true);
	player->setY(steps[0]->getStep().y-PLAYER_HEIGHT);
	player->setX(steps[0]->getStep().x);
	player->setBonus(false);
	//

	//rocket
	rocket = std::make_unique<Bonus>(Rectangle{ steps[10]->getStep().x,steps[10]->getStep().y-50,50,50 });
	rocket->setTexture("resource/rocket.png");
	rocket->setFrame();
	//

	//black_hole
	black_hole = std::make_unique<Bonus>(Rectangle{ 0,-800,50,50 });
	black_hole->setTexture("resource/black_hole.png");
	black_hole->setFrame();
	//

	//Button_start
	button_start = std::make_unique<Button>(Rectangle{150,500,100,50});
	button_start->setTexture("resource/button.png");
	button_start->setFrame();
	//

	//Button_rerty
	button_retry = std::make_unique<Button>(Rectangle{0,500,100,50});
	button_retry->setTexture("resource/button_retry.png");
	button_retry->setFrame();
	//

	//Button_exit
	button_exit = std::make_unique<Button>(Rectangle{300,500,100,50});
	button_exit->setTexture("resource/button_exit.png");
	button_exit->setFrame();
	//

	//monster
	monster = std::make_unique<Enemy>(Rectangle{300,-2000,50,50});
	monster->setTexture("resource/monster_1.png");
	monster->setFrame();
	monster->bullet = {monster->getBody().x/2,monster->getBody().y,5,5};
	//

	low_point = player->getBody().y + 350;
	low_point_prev = player->getBody().y + 350;
	background_scroll = 0.0f;

	camera.offset = { WINDOW_WIDTH/2,WINDOW_HEIGHT/2 };
	camera.rotation = 0.0f;
	camera.target = { player->GetXY() };
	camera.zoom = 1.0f;

	prev = player->getBody();
	scene = Scene::MENU;


}

void GameBoard::handlerKeyboard()    
{
	if (IsKeyDown(KEY_RIGHT))player->moveX(250.0f * dt);
	if (IsKeyDown(KEY_LEFT))player->moveX(-250.0f * dt);
}

void GameBoard::jumping()
{
	if (dt > 0.2)dt = 0;
	if (jump)
	{
		player->moveY( -player->Velocity * dt );
		player->Velocity -= GRAVITY * dt;
		if (player->Velocity < 0)
		{
			player->Velocity = 0.0f;
			jump = false;
			player->setVisible(true);
			player->setBonus(false);
		}
	}
	else
	{
		player->moveY(player->Velocity * dt);
		player->Velocity += GRAVITY * dt;
	} 
}//delta

bool GameBoard::myCheckCollision(Rectangle current, Rectangle prev, Rectangle step)
{
	if ((int)prev.y + PLAYER_HEIGHT <= (int)step.y && (int)current.y + PLAYER_HEIGHT >= (int)step.y
		&&
		((prev.x + pixel > step.x && prev.x + pixel < step.x + STEPS_WIDTH)
			||
			(prev.x + PLAYER_WIDHT > step.x && prev.x + pixel < step.x + STEPS_WIDTH))
		&&
		((current.x + pixel > step.x && current.x + pixel < step.x + STEPS_WIDTH)
			||
			(current.x + PLAYER_WIDHT > step.x && current.x + pixel < step.x + STEPS_WIDTH))
		)return true;

	return false;
}

void GameBoard::checkPlayZone()
{
	if (player->getBody().x + PLAYER_WIDHT < PLAY_ZONE.x)
	{
		player->setX(PLAY_ZONE.x + PLAY_ZONE.width);
	}else if ( player->getBody().x > PLAY_ZONE.x + PLAY_ZONE.width)
	{
		player->setX(PLAY_ZONE.x - player->getBody().width);
	}

	if(player->getBody().y > low_point)
	{
		gameOver = true;
	}

}

//class 
void GameBoard::update()
{
	while (!WindowShouldClose() && game)
	{
		dt = GetFrameTime() * 1.2;

		switch (scene)
		{
		case Scene::MAIN:
		{

		if(!gameOver)
		{
		handlerKeyboard();

		

		checkPlayZone();

		jumping();

		if(low_point > player->getBody().y + 350)low_point = player->getBody().y + 350;

		score += fabs(low_point - low_point_prev);

		for (int i = 0; i < steps.size(); i++)
		{

			//bonus generation
			if ((int)GetTime() % 30 == 0 && GetTime() > 1.0 && low_point < steps[i]->getStep().y && low_point < rocket->getBody().y)
			{
				if(i==0)
				{
					rocket->getBody().x = steps[steps.size()-1]->getStep().x;
					rocket->getBody().y = steps[steps.size()-1]->getStep().y - 50;
				}else
				{
					rocket->getBody().x = steps[i-1]->getStep().x;
					rocket->getBody().y = steps[i-1]->getStep().y - 50;
				}
			}
			//

			if(low_point < steps[i]->getStep().y )
			{
				x_random = GetRandomValue(0,350);
				if(i==0)
				{
				y_random = steps[steps.size()-1]->getStep().y - GetRandomValue(30,100);
				steps[i]->setY(y_random);
				}else
				{
				y_random = steps[i-1]->getStep().y - GetRandomValue(30,100);
				steps[i]->setY(y_random);
				}
				steps[i]->setX(x_random);
			}
			if (myCheckCollision(player->getBody(), prev, steps[i]->getStep()))
			{
				player->setY(steps[i]->getStep().y - 50);
				player->Velocity = 500.0f;
				jump = true;
			}
		}


		if (CheckCollisionRecs(player->getBody(), rocket->getBody())) { player->Velocity = 1000.0f; player->setBonus(true);jump = true; }

		if (CheckCollisionRecs(player->getBody(), black_hole->getBody()) && player->getVisible())handlerBlack_hole();

		if (low_point < black_hole->getBody().y) { black_hole->minusY(GetRandomValue(5000,10000)); black_hole->setX(GetRandomValue(0, 300)); }
	


		if(low_point < background_scroll)background_scroll-=700;
		

		cameraUpdate();

		//enemy handler(monster)
		{
		if(monster->getBody().x+50 > PLAY_ZONE.x + PLAY_ZONE.width)monster->setDir(Enemy::Dir::LEFT);
		else if(monster->getBody().x < PLAY_ZONE.x )monster->setDir(Enemy::Dir::RIGHT);
		switch (monster->getDir())
		{
		case Enemy::Dir::LEFT:
			monster->minusX(200.0f * dt);
			break;
		case Enemy::Dir::RIGHT:
			monster->plusX(200.0f * dt);
			break;
		}
		if(CheckCollisionRecs(monster->getBody(),player->getBody()))
		{
			player->setVisible(false);
			gameOver = true;
		}

		}
		//

		prev = player->getBody();
		low_point_prev = low_point;
		}else
		{
			if(IsMouseButtonPressed(0) && 
			CheckCollisionPointRec(GetMousePosition(),button_exit->getBound()))game = false;
			if(IsMouseButtonPressed(0) && 
			CheckCollisionPointRec(GetMousePosition(),button_retry->getBound()))retry();
		}
		draw();
		break;
		}
		
		case Scene::MENU:
		{

			if (CheckCollisionPointRec(GetMousePosition(), loginBox))mouseOnText = true;
			else mouseOnText = false;

			if (mouseOnText)
			{

				SetMouseCursor(MOUSE_CURSOR_IBEAM);
				int key = GetCharPressed();
				while (key>0)
				{
					if ((key >= 32) && (key <= 125) && letterCount < 9)
					{
						login += (char)key;
						letterCount++;
					}
					key = GetCharPressed();
				}

				if (IsKeyPressed(KEY_BACKSPACE))
				{
					letterCount--;
					if (letterCount < 0)letterCount = 0;
					if(!login.empty())login.pop_back();
				}

			}else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			if(IsMouseButtonPressed(0) &&
			CheckCollisionPointRec(GetMousePosition(),button_start->getBound()) && letterCount > 0 )scene = Scene::MAIN;


			draw();
			break;
		}
		}
	
	}
}

void GameBoard::cameraUpdate()
{
	if(camera.target.y > player->getBody().y)
	camera.target.y = player->getBody().y;
	camera.target.x = 200;
}

void GameBoard::handlerBlack_hole()
{
	
	player->setVisible(false);

	player->Velocity = 1000.0f;



}


void GameBoard::draw()
{
	switch (scene)
	{
	case Scene::MAIN:
	{

	BeginDrawing();

	ClearBackground(GetColor(0x052c46ff));
	DrawText(TextFormat("low_point_ : %02.02f",score),0,100,15,GREEN);

		
		BeginMode2D(camera);

		DrawTexture(background,0, background_scroll,WHITE);
		DrawTexture(background,0,background_scroll-700,WHITE);
			

		for (size_t i = 0; i < NUMBER_OF_STEPS; i++)
		{
			DrawTextureRec(steps[i]->getTexture(), steps[i]->getFrame(), steps[i]->getPosition(), WHITE);
		}


		DrawTextureRec(black_hole->getTexture(), black_hole->getFrame(), Vector2{ black_hole->getBody().x,black_hole->getBody().y}, WHITE);
		DrawTextureRec(rocket->getTexture(), rocket->getFrame(), Vector2{ rocket->getBody().x,rocket->getBody().y }, WHITE);
		DrawTextureRec(monster->getTexture(), monster->getFrame(), Vector2{ monster->getBody().x,monster->getBody().y }, WHITE);
		if (player->getVisible())
		{
			DrawTextureRec(player->getTexture(), player->getFrame(), player->GetXY(), WHITE);
		}

		EndMode2D();

		if(gameOver)
		{
			DrawText("GameOver",120,200,30,GRAY);
			DrawTextureRec(button_retry->getTexture(),button_retry->getFrame(),Vector2{button_retry->getBound().x,button_retry->getBound().y},WHITE);
			DrawTextureRec(button_exit->getTexture(),button_exit->getFrame(),Vector2{button_exit->getBound().x,button_exit->getBound().y},WHITE);

		}
		else
		{
		DrawFPS(0,200);
		DrawText(TextFormat("Score : %02i", score/10), 0, 0,15, GREEN);
		DrawText(TextFormat("position_y : %02.02f", player->getBody().y),0,100,15,GREEN);
		DrawText(TextFormat("low_point : %02.02f", low_point), 0, 150, 15, GREEN);
		}
		EndDrawing();
		break;
	}

	case Scene::MENU:
	{
		BeginDrawing();

		ClearBackground(GRAY);

		DrawRectangleRec(loginBox, LIGHTGRAY);
		DrawRectangleRec(leader, GRAY);
	
		DrawTexture(background_menu,0,0,WHITE);

		if (CheckCollisionPointRec(GetMousePosition(), leader))  DrawText(TextFormat("%s : %i", leader_name.c_str(), leader_score, 10), 220, 650, 20, BLACK);

		DrawText(login.c_str(), loginBox.x + 5, loginBox.y + 8, 40, MAROON);
		DrawText(TextFormat("YOU`R NICKNAME: %i/%i", letterCount, 9), 85, 430, 20, BLACK);
		DrawText("leader", 350, 680, 10, RED);

		if (mouseOnText)
		{
			DrawRectangleLines(loginBox.x, loginBox.y, loginBox.width, loginBox.height, RED);

			if (letterCount < 9)
			{
				if((int)GetTime()%2 == 0)DrawText("_", loginBox.x + 8 + MeasureText(login.c_str(), 40), loginBox.y + 12, 40, MAROON);
			}
		}

		else DrawRectangleLines(loginBox.x, loginBox.y, loginBox.width, loginBox.height, DARKGRAY);

		if(letterCount == 0)	DrawTextureRec(button_start->getTexture(),button_start->getFrame(),Vector2{button_start->getBound().x,button_start->getBound().y},ColorAlpha(WHITE,0.5f));
		else DrawTextureRec(button_start->getTexture(), button_start->getFrame(), Vector2{ button_start->getBound().x,button_start->getBound().y }, ColorAlpha(WHITE, 1.0f));


		EndDrawing();
		break;
	}
}
}
void GameBoard::retry()
{
	jump = true;
	gameOver = false;

	for (size_t i = 0; i < NUMBER_OF_STEPS; i++)
	{
		x_random = GetRandomValue(0,350);
		if(i==0)
		{
			y_random = WINDOW_HEIGHT / 2 + i * GetRandomValue(-100,-50);
		}else
		{
			y_random = steps[i-1]->getStep().y - GetRandomValue(30,100);
		}
		steps[i]->setX(x_random);
		steps[i]->setY(y_random);
	}

	player->setVisible(true);
	player->setY(steps[0]->getStep().y-PLAYER_HEIGHT);
	player->setX(steps[0]->getStep().x);
	player->Velocity = 500.0f;

	rocket->setX(steps[10]->getStep().x);
	rocket->setY(steps[10]->getStep().y-50);

	black_hole->setX(0);
	black_hole->setY(-800);

	prev = player->getBody();

	score = 0;

	camera.offset = { WINDOW_WIDTH/2,WINDOW_HEIGHT/2 };
	camera.rotation = 0.0f;
	camera.target = { player->GetXY() };
	camera.zoom = 1.0f;

	low_point = player->getBody().y + 350;
	low_point_prev = player->getBody().y + 350;
	background_scroll = 0.0f;
}

void GameBoard::updateStatistic()
{
	statistic.open("resource/statistic.txt",std::fstream::in | std::fstream::out );

	int s;

	statistic >> s;

	if ((score/10) > s)
	{
		statistic.close();
		statistic.open("resource/statistic.txt", std::fstream::out | std::fstream::trunc);
		statistic << (score/10);
		statistic << ' ' + login;
	}
	

	statistic.close();


}
