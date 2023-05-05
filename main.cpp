//Using SDL and standard IO
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <memory>
//#include <functional>
#include <string>
#include <vector>
#include <iostream>

#include "kiss_sdl/kiss_sdl.h"

#include "intro.h"
#include "menu.h"
#include "highscore.h"
#include "singleplayer.h"

int main( int argc, char* args[] )
{
	GameEngine game;
	double dt;
	Uint64 currentTime;
	Uint64 oldTime;

	bool quit = false;


	std::cout << "Game starts" << "\n";

	if( !game.Init() ){
		printf( "Failed to initialize!\n" );
	}
	else
	{
		
		currentTime =  SDL_GetTicks();

		while ( game.Running() )
		{
			oldTime = currentTime;
        		currentTime =  SDL_GetTicks();
        		dt = (currentTime - oldTime) / 1000.0f;

			game.HandleEvents();
			game.Draw();
			game.Update(dt);
		}
	}

	//Free resources and close SDL
	game.Cleanup();

	return 0;
}
