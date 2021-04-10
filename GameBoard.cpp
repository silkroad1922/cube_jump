#include "GameBoard.h"

void GameBoard::Init()
{

	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "doodle_jump");


	monster = LoadTexture("resource/pixel_5.png");
	background = LoadTexture("resource/space.png");
	cloud = LoadTexture("resource/test6.png");
	rocket = LoadTexture("resource/rocket.png");
	black_hole = LoadTexture("resource/black_hole.png");

	rocket.height = 50;
	rocket.width = 50;

	monster.height = 50;
	monster.width = 50;

	background.height = 700;

	cloud.height = 10;
	cloud.width = 50;

	black_hole.width = 100;
	black_hole.height = 50;

	framePlayer = { 0.0f, 0.0f, (float)monster.width ,(float)monster.height };

	frameRocket = { 0.0f,0.0f,(float)rocket.width , (float)rocket.height };

	frameBlack_hole = { 0.0f,0.0f,(float)black_hole.width,(float)black_hole.height };

	for (size_t i = 0; i < NUMBER_OF_STEPS; i++)
	{
		frameStep.push_back(Rectangle{ 0.0f,0.0f,(float)cloud.width,(float)cloud.height });
	}

	SetTargetFPS(60);
	jump = true;
	gameOver = false;
	visible_player = true;

	//int a = 10;

	//constexpr void foo(const int& a)



	float dt = GetFrameTime();

	//velocity* dt;

	pixel = 5;
	score = 0;

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
	}
	
	player->setY(steps[0]->getStep().y-PLAYER_HEIGHT);
	player->setX(steps[0]->getStep().x);

	rocket_rectangle = { steps[19]->getStep().x , steps[19]->getStep().y - 50,50,50 };
	black_hole_rectangle = { 0,-800,100,100 };

	low_point = player->getBody().y + 350;
	low_point_prev = player->getBody().y + 350;
	background_scroll = 0.0f;

	camera.offset = { WINDOW_WIDTH/2,WINDOW_HEIGHT/2 };
	camera.rotation = 0.0f;
	camera.target = { player->GetXY() };
	camera.zoom = 1.0f;

	prev = player->getBody();


}

void GameBoard::handlerKeyboard()    
{
	if (IsKeyDown(KEY_RIGHT))player->moveX(250.0f * dt);
	if (IsKeyDown(KEY_LEFT))player->moveX(-250.0f * dt);
}

void GameBoard::jumping()
{
	
	if (jump)
	{
		player->moveY( -player->Velocity * dt );
		player->Velocity -= GRAVITY * dt;
		if (player->Velocity < 0)
		{
			player->Velocity = 0.0f;
			jump = false;
			visible_player = true;
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
	while (!WindowShouldClose())
	{
		//dt = 0.015 * 1.2;
		dt = GetFrameTime();

		//dt = 0.015 * 1.2;

		//dt = 1.0f / 60 * 1.2;

		handlerKeyboard();

		checkPlayZone();

		jumping();
		
		if ((int)GetTime() % 30 == 0)//как генерировать бонус?
		{
			rocket_rectangle.x = steps[19]->getStep().x;
			rocket_rectangle.y = steps[19]->getStep().y - 50;
		}

		score += fabs(low_point - low_point_prev);

		if(low_point > player->getBody().y + 350)low_point = player->getBody().y + 350;

		score += fabs(low_point - low_point_prev);

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
				player->Velocity = 500.0f;
				jump = true;
			}
		}

		if (CheckCollisionRecs(player->getBody(), rocket_rectangle))player->Velocity = 1000.0f;

		if (CheckCollisionRecs(player->getBody(), black_hole_rectangle) && visible_player)handlerBlack_hole();

		if (low_point < black_hole_rectangle.y) { black_hole_rectangle.y -= GetRandomValue(5000,10000); black_hole_rectangle.x = GetRandomValue(0, 300); }
	


		if(low_point < background_scroll)background_scroll-=700;
		

		cameraUpdate();

		prev = player->getBody();
		low_point_prev = low_point;
		


		draw();

	
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
	
	visible_player = false;

	player->Velocity = 1000.0f;



}


void GameBoard::draw()
{
	BeginDrawing();

	DrawText(TextFormat("low_point_ : %02.02f",score),0,100,15,GREEN);

	ClearBackground(GetColor(0x052c46ff));

		
		BeginMode2D(camera);

		DrawTexture(background,0, background_scroll,WHITE);
		DrawTexture(background,0,background_scroll-700,WHITE);
			

		for (size_t i = 0; i < NUMBER_OF_STEPS; i++)
		{
			DrawTextureRec(cloud, frameStep.at(i), steps[i]->getPosition(), WHITE);
		}


		DrawTextureRec(black_hole, frameBlack_hole, Vector2{ black_hole_rectangle.x,black_hole_rectangle.y }, WHITE);
		DrawTextureRec(rocket, frameRocket, Vector2{ rocket_rectangle.x,rocket_rectangle.y }, WHITE);
		if(visible_player)DrawTextureRec(monster, framePlayer, player->GetXY(),WHITE);



		EndMode2D();

		if(gameOver)DrawText("GameOver",120,200,30,GRAY);
		DrawFPS(0,200);
		DrawText(TextFormat("Score : %02i", score/10), 0, 0,15, GREEN);
		DrawText(TextFormat("position_y : %02.02f", player->getBody().y),0,100,15,GREEN);
		DrawText(TextFormat("low_point : %02.02f", low_point), 0, 150, 15, GREEN);

		

	EndDrawing();

}

