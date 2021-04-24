#include "GameBoard.h"

void GameBoard::Init()
{
	
	srand(time(NULL));

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "doodle_jump");

	background = LoadTexture("resource/space.png");


	SetTargetFPS(60);
	jump = true;
	gameOver = false;

	pixel = 15;
	score = 0;
	dt = GetFrameTime();

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
		steps[i]->setTexture("resource/test6.png");
		steps[i]->setFrame();
	}
	//

	//player
	player = std::make_unique<Player>(Rectangle{ 0,0,PLAYER_WIDHT,PLAYER_HEIGHT });
	player->setTexture("resource/pixel_8.png");
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
	while (!WindowShouldClose())
	{
		dt = GetFrameTime() * 1.2;


		handlerKeyboard();

		checkPlayZone();

		jumping();
		
		if ((int)GetTime() % 30 == 0 && GetTime() > 1.0)//как генерировать бонус?
		{
			rocket->getBody().x = steps[0]->getStep().x;
			rocket->getBody().y = steps[0]->getStep().y - 50;
		}


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

		if (CheckCollisionRecs(player->getBody(), rocket->getBody())) { player->Velocity = 1000.0f; player->setBonus(true); }

		if (CheckCollisionRecs(player->getBody(), black_hole->getBody()) && player->getVisible())handlerBlack_hole();

		if (low_point < black_hole->getBody().y) { black_hole->minusY(GetRandomValue(5000,10000)); black_hole->setX(GetRandomValue(0, 300)); }
	


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
	
	player->setVisible(false);

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
			DrawTextureRec(steps[i]->getTexture(), steps[i]->getFrame(), steps[i]->getPosition(), WHITE);
		}


		DrawTextureRec(black_hole->getTexture(), black_hole->getFrame(), Vector2{ black_hole->getBody().x,black_hole->getBody().y}, WHITE);
		DrawTextureRec(rocket->getTexture(), rocket->getFrame(), Vector2{ rocket->getBody().x,rocket->getBody().y }, WHITE);
		if (player->getVisible())
		{
			if (!player->getBonus())
			{
				player->getFrame().x = (float)0 * (float)player->getTexture().width / 2;
				DrawTextureRec(player->getTexture(), player->getFrame(), player->GetXY(), WHITE);
			}
			else
			{
				player->getFrame().x = (float)1 * (float)player->getTexture().width / 2;
				DrawTextureRec(player->getTexture(), player->getFrame(), player->GetXY(), WHITE);
			}
		}



		EndMode2D();

		if(gameOver)DrawText("GameOver",120,200,30,GRAY);
		DrawFPS(0,200);
		DrawText(TextFormat("Score : %02i", score/10), 0, 0,15, GREEN);
		DrawText(TextFormat("position_y : %02.02f", player->getBody().y),0,100,15,GREEN);
		DrawText(TextFormat("low_point : %02.02f", low_point), 0, 150, 15, GREEN);

		

	EndDrawing();

}

