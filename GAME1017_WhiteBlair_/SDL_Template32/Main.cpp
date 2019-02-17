#include <iostream>
#include "Game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
using namespace std;

int main(int argc, char* args[])
{
	// We have the same basic structure as before but no more Game object
	// as we can now access everything through the static Game instance.
	Game::Instance()->Init("Lecture 2 Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, 0);
	while (Game::Instance()->Running())
	{
		Game::Instance()->HandleEvents();
		if (Game::Instance()->Tick())
		{
			Game::Instance()->Update();
			Game::Instance()->Render();
		}
	}
	Game::Instance()->Clean();
	return 0;
}