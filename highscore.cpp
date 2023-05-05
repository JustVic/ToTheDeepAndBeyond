#include "highscore.h"

extern int draw;

static void buttonBackEvent(kiss_button *button, SDL_Event *e, int *draw,
                                 std::function<void()> callback)
{
	if (kiss_button_event(button, e, draw)) {
		callback();
    }
}

bool Highscore::Init(GameEngine* game)
{
	buttonBack = {0};

	kiss_window* window = (game->GetRend()->GetWindow());

	centerW = game->GetRend()->GetWidth()/2;
	centerH = game->GetRend()->GetHeight()/2;

	screenWidth = game->GetRend()->GetWidth();
	screenHeight = game->GetRend()->GetHeight();
	cellWidth = game->GetRend()->GetCellWidth();


	kiss_button_new(&buttonBack, window, "Back",
                        centerW, centerH+screenHeight/8);
	
   	char high0[100] = "Highscore ";
   	char high1[100] = "Labyrinth Size ";

	Highlevel = game->ReadSave();

	HighlevelStr = std::to_string(Highlevel);

	SizeStr = std::to_string(Highlevel+20);

	char* highscore = strcat(high0, HighlevelStr.c_str());

	char* size = strcat(high1, SizeStr.c_str());

	int width0 = 0;

	int width1 = 0;

	width0 = kiss_textwidth(kiss_textfont, highscore, NULL);

	width1 = kiss_textwidth(kiss_textfont, size, NULL);

	kiss_label_new(&HighscoreLabel, window, highscore,
                           centerW-width0/2,
                centerH-buttonBack.rect.h*2);//screenHeight/8);

	kiss_label_new(&SizeLabel, window, size,
                           centerW-width1/2,
                centerH-buttonBack.rect.h);//screenHeight/16);

	cupTexture = game->GetRend()->createTexture("data/cup.bmp");

	return true;
}

void Highscore::Update(GameEngine* game, float dt)
{
}

void Highscore::HandleEvents(GameEngine* game)
{
	SDL_PollEvent(&event);

	buttonBackEvent(&buttonBack, &event, &draw,
                         std::bind(&GameEngine::PopState, game));

	switch(event.type)
    {
		case SDL_KEYDOWN:
			if( event.key.keysym.sym == SDLK_q){
				game->Quit();
            }
            break;

        case SDL_TEXTINPUT:
		break;

		case SDL_QUIT:
			game->Quit();
			break;
	}
}

void Highscore::DrawLabel(std::shared_ptr<RenderManager>& rend, int x, int y,
						  int w, int h)
{
	rend->DrawTextureReal(cupTexture.get(), x, y, w, h);
}
void Highscore::Draw(GameEngine* game)
{
	kiss_button_draw(&buttonBack, game->GetRend()->GetSDLRenderer());

	kiss_label_draw(&HighscoreLabel, game->GetRend()->GetSDLRenderer());
	
	kiss_label_draw(&SizeLabel, game->GetRend()->GetSDLRenderer());

	int picWidth = cellWidth;
	int picHeight = cellWidth;

	//this->DrawLabel(game->rend, centerW-buttonBack.rect.w-picWidth*2,
	//				HighscoreLabel.rect.y, picWidth, picHeight);
}
