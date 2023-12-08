#include "Game.h"
void main()
{
	Game* game = new Game();
	game->play();
	if (game) delete game;
}