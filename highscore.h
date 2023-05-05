#ifndef highscore_h
#define highscore_h

#include "gamestate.h"

class Highscore : public GameState
{
	private:
		int Highlevel;
		int Size;
		std::string HighlevelStr;
		std::string SizeStr;

		kiss_button buttonBack;
		kiss_label HighscoreLabel;
		kiss_label SizeLabel;

		SDL_RWops* saveFile;
		SDL_Event event;
		std::shared_ptr<SDL_Texture> cupTexture;
		std::shared_ptr<RenderManager> rend;
		int centerW;
		int centerH;
		int screenWidth;
		int screenHeight;
		int cellWidth;

	public:
		bool Init(GameEngine* game);

		void Update(GameEngine* game, float dt);

		void HandleEvents(GameEngine* game);

		void DrawLabel(std::shared_ptr<RenderManager>& rend, int x, int y, int
					   w, int h);

		void Draw(GameEngine* game);
};

#endif
