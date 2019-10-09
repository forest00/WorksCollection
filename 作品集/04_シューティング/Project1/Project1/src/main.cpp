#include "game/game.h"

int Game()
{
	if (InitGame())
	{
		EndGame();
		return -1;
	}
	while (ContinueGame())
	{
		UpdateGame();
		DrawGame();
	}
	EndGame();
	return 0;
}

int main()
{
	Game();
	return 0;
}