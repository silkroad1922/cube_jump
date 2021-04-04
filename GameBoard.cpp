#include "GameBoard.h"

void GameBoard::Init()
{

	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "doodle_jump");



	monster = LoadTexture("resource/pixel_5.png");
	background = LoadTexture("resource/space.png");


	monster.height = 50;
	monster.width = 50;

	background.height = 800;

	frameRec = { 0.0f, 0.0f, (float)monster.height ,(float)monster.width };//?

	SetTargetFPS(60);
	jump = true;
	gameOver = false;

	count_steps = 0;


	player = std::make_unique<Player>(Rectangle{ 0,0,PLAYER_WIDHT,PLAYER_HEIGHT });


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
		count_steps++;
	}
	
	player->setY(steps[0]->getStep().y-PLAYER_HEIGHT);
	player->setX(steps[0]->getStep().x);

	low_point = player->getBody().y + 350;
	background_scroll = 0.0f;

	camera.offset = { WINDOW_WIDTH/2,WINDOW_HEIGHT/2 };
	camera.rotation = 0.0f;
	camera.target = { player->GetXY() };
	camera.zoom = 1.0f;

	prev = player->getBody();


}

void GameBoard::handlerKeyboard()
{
	if (IsKeyDown(KEY_RIGHT))player->moveX(6.0f);
	if (IsKeyDown(KEY_LEFT))player->moveX(-6.0f);
}

void GameBoard::jumping()
{
	if (jump)
	{
		player->moveY(-player->Velocity);
		player->Velocity -= GRAVITY;
		if (player->Velocity < 0)
		{
			player->Velocity = 0.0f;
			jump = false;
		}
	}
	else
	{
		player->moveY(player->Velocity);
		player->Velocity += GRAVITY;
	}
}

bool GameBoard::myCheckCollision(Rectangle current, Rectangle prev, Rectangle step)
{
	if ((int)prev.y + 50 <= (int)step.y && (int)current.y + 50 >= (int)step.y
		&&
		((prev.x > step.x && prev.x < step.x + 50)
			||
			(prev.x + 50 > step.x && prev.x < step.x + 50))
		&&
		((current.x > step.x && current.x < step.x + 50)
			||
			(current.x + 50 > step.x && current.x < step.x + 50))
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
	while (!WindowShouldClose())
	{
		handlerKeyboard();

		checkPlayZone();

		jumping();

		if(low_point > player->getBody().y + 350)low_point = player->getBody().y + 350;

		for (int i = 0; i < steps.size(); i++)
		{
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
				player->Velocity = 10.0f;
				jump = true;
			}
		}

		if(low_point < background_scroll)background_scroll-=700;

		cameraUpdate();

		prev = player->getBody();

		draw();
	}
}

void GameBoard::cameraUpdate()
{
	if(camera.target.y > player->getBody().y)
	camera.target.y = player->getBody().y;
	camera.target.x = 200;
}


void GameBoard::draw()
{
	BeginDrawing();

	DrawText(TextFormat("low_point_ : %02.02f",low_point),0,100,15,GREEN);

	ClearBackground(GetColor(0x052c46ff));

		
		BeginMode2D(camera);

		DrawTexture(background,0, background_scroll,WHITE);
		DrawTexture(background,0,background_scroll-700,WHITE);
			
		for (int i = 0; i < NUMBER_OF_STEPS; i++)
		{
			DrawRectangleRec(steps[i]->getStep(), PURPLE);
		}

		//DrawRectangleRec(player->getBody(), RED);


		DrawTextureRec(monster, frameRec, player->GetXY(),WHITE);


		EndMode2D();

		if(gameOver)DrawText("GameOver",120,200,30,GRAY);
		DrawFPS(0,200);

	EndDrawing();


}

