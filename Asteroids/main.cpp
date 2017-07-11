#include "Game.h"

#include <Windows.h>
#include <exception>

int main(int argc, char* argv[])
{
	try
	{
		Asteroids::Game game;
		game.Run();
	}
	// Game failed to load resources
	catch(...)
	{
		MessageBox(
					NULL,
					"Failed to load one or more resources.\nPlease try re-installing the game to restore missing resources.",
					"Fatal Error",
					MB_ICONERROR
				  );

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}