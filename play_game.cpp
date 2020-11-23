#include <windows.h>
#include <stdbool.h>
#include <ctime>
#include "pacman.h"

int main()
{
	system("chcp 437>null");
	system("cls");

	Game game;
	game.StartGame();
	return 0;
}
