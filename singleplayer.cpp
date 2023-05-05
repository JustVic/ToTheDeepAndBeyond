#include "singleplayer.h"

#include <algorithm>


extern int draw;

void Singleplayer::Shoot()
{


}

void Singleplayer::ButtonShootEvent(kiss_button *button, SDL_Event *e, int *draw, std::function<void()> callback)
{
	

	//if(e->type == SDL_KEYUP)
	//{
		//if(e->key.repeat == 0)
		//{
			if (kiss_button_event(button, e, draw)) {
					press_end = SDL_GetTicks();
					press_duration = press_end - press_start;
					map->Shoot(&player, press_duration);
							//distance = press_duration/2;
					std::cout << "DURATION: " << press_duration << "\n";

				callback();
    			}
		//}
	//}
}

bool Singleplayer::Init(GameEngine* game)
{
	//kiss_window* window = game->GetWindow();//&(game->GetRend()->window1);
	blue_texture = game->GetRend()->createTexture("data/quadb.bmp");

	orange_texture = game->GetRend()->createTexture("data/quado.bmp");

	kiss_window* window = (game->GetRend()->GetWindow());
	width = game->GetRend()->GetWidth();
	height = game->GetRend()->GetHeight();
	cellWidth = game->GetRend()->GetCellWidth();
	std::cout << "Width " << width << std::endl;
	std::cout << "Height " << height << std::endl;

	int centerW = game->GetRend()->GetWidth()/2;
	int centerH = game->GetRend()->GetHeight()/2;

	char gmt[9] = "GameOver";

	int tWidth = kiss_textwidth(kiss_textfont, gmt, NULL); 

	kiss_label_new(&GameOver_Text, window, "GameOver!", centerW-tWidth/2,
		centerH);

	GameOver_Text.textcolor = kiss_black;

	GameOver_Text.font.lineheight = 400;

	mapCenterX = 0;
	mapCenterY = 0;

	srand ( time(NULL) );

	stop = false;

	end_save_flag = false;

	frame_start = SDL_GetTicks();

	SDL_FRect rec = {cellWidth+cellWidth/4, cellWidth+cellWidth/4, cellWidth/2, cellWidth/2};

	player.rect = rec;
	player.health = 10;
	player.sprite = 0;
	player.speed = cellWidth/26;
	if(player.speed<1)
		player.speed = 1;
	player.direction = 1;
	player.moveSpeed.x = 0;
	player.	moveSpeed.y = 0;

	map = new Map(game, &player);

	scoreStr = "Health";

	scoreStr += std::to_string(this->GetPlayerHealth());

	kiss_label_new(&scoreLabel, window, (char*)scoreStr.c_str(),
		       game->GetRend()->GetWidth()-scoreLabel.rect.w-8,
		0);

	scoreLabel.textcolor = kiss_white;

	kiss_button_new(&shootButton, window, "Shoot",
                        width-(width/8), height-(height/8));
	
	return true;
}


void Singleplayer::GameOver()
{
	stop = true;

	std::cout << "\nGameOver";
}

void Singleplayer::CheckForGameOver()
{

	if(this->PlayerDie())
	{
		this->GameOver();
	}
}

bool Singleplayer::UpdateScore()
{
	int w,h;

	strcpy(scoreC, "Health ");

	scoreStr = std::to_string(this->GetPlayerHealth());

	char * hh = strcat(scoreC, scoreStr.c_str());

	TTF_SizeText(kiss_textfont.font, hh, &w, &h);

	int diff_width = (scoreLabel.rect.w - w);

	kiss_string_copy(scoreLabel.text, KISS_MAX_LABEL,
                                                 hh, NULL);

	scoreLabel.rect.x=scoreLabel.rect.x + diff_width;
	scoreLabel.rect.w=w;
}

/*void Singleplayer::Shoot(float press_duration)//+
{
	std::cout << "SHOOT" << std::endl;
	Bullet bullet_block;

	bullet_block.rect.x = player.rect.x+cellWidth/8;
	bullet_block.rect.y = player.rect.y+cellWidth/8;
	bullet_block.startPoint.x = bullet_block.rect.x;
	bullet_block.startPoint.y = bullet_block.rect.y;

	bullet_block.rect.w = cellWidth/4;
	bullet_block.rect.h = cellWidth/4;
	bullet_block.direction = player.direction;
	//if(press_duration<1)
		//press_duration = 1;
	bullet_block.distance =  (press_duration/8)*cellWidth/20;
	
	bullet_block.z = 0;

	switch(player.direction)
	{
		case 0:
			bullet_block.rect.x -= cellWidth/8;
			break;
		case 1:
			bullet_block.rect.x += cellWidth/8;
			break;
		case 2:
			bullet_block.rect.y -= cellWidth/8;
			break;
		case 3:
			bullet_block.rect.y += cellWidth/8;
			break;
	}

	pBullets.push_back(bullet_block);
}*/


void Singleplayer::Update(GameEngine* game, float dt)
{
	this->CheckForGameOver();

	if(!map->WallCollision(player.rect.x+player.moveSpeed.x*(dt*100), player.rect.y, cellWidth/2, cellWidth/2))
	{
		player.rect.x += player.moveSpeed.x*(int)(dt*100);
		map->UpdateMapCenterX(player.rect.x);
	}

	if(!map->WallCollision(player.rect.x, player.rect.y+player.moveSpeed.y*(dt*100), cellWidth/2, cellWidth/2))
	{
		player.rect.y += player.moveSpeed.y*(int)(dt*100);
		map->UpdateMapCenterY(player.rect.y);
	}

	map->MonsterHit(&player, dt, 1);
	if(map->SimpleEnemyCollision(player.rect.x, player.rect.y, player.rect.w, player.rect.h))
		player.health = 0;


	map->Update(game, &player, dt);
	this->UpdateScore();
}


int Singleplayer::GetPlayerHealth()//+
{
	return player.health;
}

bool Singleplayer::PlayerDie()//+
{
	if(player.health <= 0)
	{
		//std::cout << "YOU LOST" << std::endl;
		return true;
	}

	return false;
}

void Singleplayer::PlayerStopX()
{
	player.moveSpeed.x = 0;
}

void Singleplayer::PlayerStopY()
{
	player.moveSpeed.y = 0;
}

void Singleplayer::PlayerMoveDown()
{
	player.direction = 3;
	player.moveSpeed.y = +player.speed;
}

void Singleplayer::PlayerMoveUp()
{
	player.direction = 2;
	player.moveSpeed.y = -player.speed;
}

void Singleplayer::PlayerMoveLeft()
{
	player.direction = 0;
	player.moveSpeed.x = -player.speed;
}

void Singleplayer::PlayerMoveRight()
{
	player.direction = 1;
	player.moveSpeed.x = +player.speed;
}


void Singleplayer::HandleEvents(GameEngine* game)
{
	//SDL_PollEvent(&event);
	//Touch variables
        SDL_Point touchLocation = { width/2, height/2 };

	ButtonShootEvent(&shootButton, &event, &draw,
                         std::bind(&Singleplayer::Shoot, this));
	//if(!stop)
	//{
		if(SDL_PollEvent(&event) == 1)
		{

			switch(event.type)
			{

				case SDL_KEYUP:

					if( event.key.keysym.sym == SDLK_SPACE)
					{
						//	break;
					}

					if( event.key.keysym.sym == SDLK_LEFT)
					{
						this->PlayerStopX();
					//break;
					}

					if( event.key.keysym.sym == SDLK_RIGHT)
					{
						this->PlayerStopX();
					//break;
					}
					if( event.key.keysym.sym == SDLK_UP)
					{
						this->PlayerStopY();
					//break;
					}

					if( event.key.keysym.sym == SDLK_DOWN)
					{
						this->PlayerStopY();
					//break;
					}
 
					if(event.key.keysym.sym == SDLK_z)
					{
						if(event.key.repeat == 0)
						{
							press_end = SDL_GetTicks();
							press_duration = press_end - press_start;
							map->Shoot(&player, press_duration);
							//distance = press_duration/2;
							break;
						}
						break;
					}
					break;

				case SDL_KEYDOWN:
					if( event.key.keysym.sym == SDLK_BACKSPACE)
					{
						game->Quit();
						break;
					}

					if( event.key.keysym.sym == SDLK_ESCAPE)
					{
						game->PopState();
					//game->Quit();
						break;
					}

					if( event.key.keysym.sym == SDLK_SPACE)
					{
						break;
					}

					if( event.key.keysym.sym == SDLK_LEFT)
					{
						if(event.key.repeat == 1)
						{
						//direction = 0;
						//snk[0].x -= width/200;

						}

						if(!stop)
							this->PlayerMoveLeft();

						//break;
					}

					if( event.key.keysym.sym == SDLK_RIGHT)
					{
						if(event.key.repeat == 1)
						{
						}
						if(!stop)
							this->PlayerMoveRight();

						//break;
					}

					if( event.key.keysym.sym == SDLK_UP)
					{
						if(event.key.repeat == 1)
						{
						}
						if(!stop)
							this->PlayerMoveUp();
						//break;
					}

					if( event.key.keysym.sym == SDLK_DOWN)
					{

						if(event.key.repeat == 1)
						{
						}
						if(!stop)
							this->PlayerMoveDown();
						//break;
					}

					if( event.key.keysym.sym == SDLK_z)
					{
						if(event.key.repeat == 0)
						{
						//this->Shoot();
							press_start = SDL_GetTicks();
						}
					//	break;
					}

					if( event.key.keysym.sym == SDLK_x)
					{
					}

				break;

			case SDL_MOUSEBUTTONDOWN:
				if(event.key.repeat == 0)
				{
					//this->Shoot();
					press_start = SDL_GetTicks();
				}
				break;

			case SDL_TEXTINPUT:
				break;

			case SDL_QUIT:
				game->Quit();
				break;
			default:
				break;
			}
		}

		SDL_PumpEvents();
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

    	//continmapCenterX+uous-response keys
    		if(keystate[SDL_SCANCODE_LEFT])
    		{
			if(!stop)
				this->PlayerMoveLeft();
    		}
    		if(keystate[SDL_SCANCODE_RIGHT])
    		{
			if(!stop)
				this->PlayerMoveRight();
    		}
   	 	if(keystate[SDL_SCANCODE_UP])
    		{
			if(!stop)
				this->PlayerMoveUp();
    		}
    		if(keystate[SDL_SCANCODE_DOWN])
    		{
			if(!stop)
				this->PlayerMoveDown();
    		}
    //Touch down
                if( event.type == SDL_FINGERDOWN )
                {
			if(!stop)
			{

                        	touchLocation.x = event.tfinger.x * width;
                        	touchLocation.y = event.tfinger.y * height;
				if((touchLocation.x<width/2)&&(touchLocation.x>height/2)&&(touchLocation.y<(3*height)/4))
					this->PlayerMoveLeft();
				if((touchLocation.x>width/2)&&(touchLocation.x>height/2)&&(touchLocation.y<(3*height)/4))
					this->PlayerMoveRight();
				if(touchLocation.y>(3*height)/4)
					this->PlayerMoveDown();
				if(touchLocation.y<height/2)
					this->PlayerMoveUp();
			}

                }
                    //Touch motion
                else if( event.type == SDL_FINGERMOTION )
                {
                        touchLocation.x = event.tfinger.x * width;
                        touchLocation.y = event.tfinger.y * height;
                }
                    //Touch release
                else if( event.type == SDL_FINGERUP )
                {
                        touchLocation.x = event.tfinger.x * width;
                        touchLocation.y = event.tfinger.y * height;
			
			this->PlayerStopX();
			this->PlayerStopY();
        	}
	//}
}

void Singleplayer::DrawPlayer(std::shared_ptr<RenderManager>& rend)
{
	if(player.sprite == 0)
    		rend->DrawTexture(blue_texture.get(), map->GetMapCenterX()+player.rect.x, map->GetMapCenterY()+player.rect.y);
	else if(player.sprite == 1)
    		rend->DrawTexture(orange_texture.get(), map->GetMapCenterX()+player.rect.x, map->GetMapCenterY()+player.rect.y);
}

void Singleplayer::Draw(GameEngine* game)
{
	this->DrawPlayer(game->rend);

	map->Draw(game->rend, &player);

	kiss_label_draw(&scoreLabel, game->rend->GetSDLRenderer());
	
	kiss_button_draw(&shootButton, game->GetRend()->GetSDLRenderer());

	if(stop)
		kiss_label_draw(&GameOver_Text, game->rend->GetSDLRenderer());

    	frame_end = SDL_GetTicks();
    	frame_duration = frame_end - frame_start;
}

