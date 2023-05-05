#ifndef singleplayer_h
#define singleplayer_h

#include "gamestate.h"
#include <time.h>
#include <map>

#include "map.h"

//enum bulletType { SIMPLE, ARMOR, ROCKET };



class Singleplayer : public GameState
{
	private:
		int label_x;
		int label_y;
		int label_width;
		int label_height;

		int mapCenterX;
		int mapCenterY;

		char scoreC[100] = "Score ";

		kiss_label scoreLabel = {0};

	    	kiss_label Score_Num = {0};
	    	    	
		kiss_button shootButton = {0};

		kiss_button backToMenuButton = {0};

	    	std::string scoreStr;

	    	kiss_label GameOver_Text = {0};

	    	SDL_RWops* save_file;

		Map* map;

		bool end_save_flag;

	    	bool stop;

		int width;
		int height;
		int widthInCells;
		int heightInCells;

		kiss_button button_ok2;

		SDL_Event event;

		Player player;
		int cellWidth;

	    Uint32 frame_start;
	    Uint32 frame_end;
	    Uint32 frame_duration;


	    Uint32 press_start;
	    Uint32 press_end;
	    Uint32 press_duration;


	    Uint32 phase_start;
	    Uint32 phase_end;
	    Uint32 phase_duration;

	    std::function<void()> nextStateCallback;

	    std::function<void()> quitCallback;

	    //std::function<void()> nextCallback;
	    
		std::shared_ptr<RenderManager> rend;
		
		std::shared_ptr<SDL_Texture> blue_texture;
	    	std::shared_ptr<SDL_Texture> orange_texture;

	public:
		Singleplayer();
		~Singleplayer();

		bool Init(GameEngine* game);

		void Update(GameEngine* game, float dt);

		void PlayerMoveDown();
		void PlayerMoveUp();
		void PlayerMoveLeft();
		void PlayerMoveRight();
		void PlayerStopY();
		void PlayerStopX();
		
		bool PlayerDie();
		int GetPlayerHealth();

		//void Cleanup();
		void HandleEvents(GameEngine* game);
		void Restart();
		bool UpdateScore();
		void setNextStateCallback(std::function<void()> callback)
	    	{nextStateCallback = callback;};
		void GameOver();
		void CheckForGameOver();

		void Shoot();
		void ButtonShootEvent(kiss_button *button, SDL_Event *e, int *draw, std::function<void()> callback);
		void BackToMenuButtonEvent(kiss_button *button, SDL_Event *e, int *draw, std::function<void()> callback);
		void DrawPlayer(std::shared_ptr<RenderManager>& rend);
		void Draw(GameEngine* game);
};

#endif
