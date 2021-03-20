#include "GameBoard.h"

void GameBoard::Init()
{
	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "doodle_jump");
	SetTargetFPS(60);

	jump = true;
	gameOver = false;

	count_steps = 0;

	player = std::make_unique<Player>(Rectangle{ 150,150,PLAYER_WIDHT,PLAYER_HEIGHT });


	for (int i = 0; i < NUMBER_OF_STEPS; i++)
	{
		if (count_steps % 2 == 0)
		{
			steps.push_back(std::make_unique<Steps>(Rectangle{ (float)GetRandomValue(199,550),(float)WINDOW_HEIGHT / 2 + count_steps * -100,STEPS_WIDTH,STEPS_HEIGHT }));
			count_steps++;
		}
		else
		{
			steps.push_back(std::make_unique<Steps>(Rectangle{ (float)GetRandomValue(199,550),(float)WINDOW_HEIGHT / 2 + count_steps * -100,STEPS_WIDTH,STEPS_HEIGHT }));
			count_steps++;
		}
	}
	
	player->setY(steps[0]->getStep().y-PLAYER_HEIGHT);
	player->setX(steps[0]->getStep().x);

	low_point = player->getBody().y + 200;

	camera.offset = { WINDOW_WIDTH/2,WINDOW_HEIGHT/2 };
	camera.rotation = 0.0f;
	camera.target = { player->GetXY() };
	camera.zoom = 1.0f;

	prev = player->getBody();


}

void GameBoard::handlerKeyboard()
{
	if (IsKeyDown(KEY_RIGHT))player->moveX(4.0f);
	if (IsKeyDown(KEY_LEFT))player->moveX(-4.0f);
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
	if (player->getBody().x < PLAY_ZONE.x)
	{
		player->setX(PLAY_ZONE.x);
	}else if ( player->getBody().x > PLAY_ZONE.x + PLAY_ZONE.width -PLAYER_WIDHT)
	{
		player->setX(PLAY_ZONE.x + PLAY_ZONE.width - PLAYER_WIDHT);
	}

	if(player->getBody().y > low_point)
	{
		gameOver = true;
	}

}


void GameBoard::update()
{
	while (!WindowShouldClose())
	{
		handlerKeyboard();

		checkPlayZone();

		jumping();

		if(low_point > player->getBody().y + 200)low_point = player->getBody().y + 200;

		for (int i = 0; i < steps.size(); i++)
		{
			if(low_point < steps[i]->getStep().y )
			{
				steps[i]->setY((float)WINDOW_HEIGHT / 2 + count_steps * -100);
				count_steps++;
			}
			if (myCheckCollision(player->getBody(), prev, steps[i]->getStep()))
			{
				DrawRectangle(100, 100, 500, 500, BLACK);
				player->setY(steps[i]->getStep().y - 50);
				player->Velocity = 9.0f;
				jump = true;
			}
		}

		cameraUpdate();

		prev = player->getBody();

		draw();
	}
}

void GameBoard::cameraUpdate()
{
	if(camera.target.y > player->getBody().y)
	camera.target.y = player->getBody().y;
	camera.target.x = 400;
}


void GameBoard::draw()
{
	BeginDrawing();

	ClearBackground(BLACK);
		
		BeginMode2D(camera);
			
		for (int i = 0; i < NUMBER_OF_STEPS; i++)
		{
			DrawRectangleRec(steps[i]->getStep(), PURPLE);
		}

		DrawRectangleRec(player->getBody(), RED);

		EndMode2D();

		if(gameOver)DrawText("GameOver",120,200,30,GRAY);

	EndDrawing();


}

