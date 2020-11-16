#include <windows.h>
#include <stdbool.h>
#include <ctime>
#include "pacman.h"

int main()
{
	system("chcp 437>null");
	system("cls");
	srand(time(NULL));
	Field field;
	Cursor cursor;

	Pacman pacman;
	pacman.InitPacman(field);

	Ghost* a_mas_ghost_;
	a_mas_ghost_ = new Ghost[COUNT_GHOST];
	for (int i = 0; i < COUNT_GHOST; i++)
		a_mas_ghost_[i].InitGhost(field);

	cursor.HideCursor();
	while (1)
	{
		pacman.Move(field);

		for (int i = 0; i < COUNT_GHOST; i++)
			a_mas_ghost_[i].Move(field);

		field.CheckCollision(a_mas_ghost_, pacman);

		for (int i = 0; i < MAX_COUNT_COIN; i++)
			field.CheckCoin((field.GetMasCoin())[i], pacman);

		field.Show();
		if (!pacman.GetChase())
			printf("SCORE: %.3d      COIN: %d       LIVE: %d\n", pacman.GetFood(), pacman.GetCoin(), pacman.GetLives());
		else
			printf("CHASE! KILL THE GHOST! TIME LEFT: %.3d\n", pacman.GetChase());

		if (!pacman.CheckLives())
		{
			system("cls");
			printf("GAME OVER\n");
			printf("SCORE: %d\n", pacman.GetFood());
			system("pause");
			break;
		}

		if (!field.CheckFood())
		{
			system("cls");
			printf("YOU WIN!\n");
			break;
		}

		Sleep(TIME_SLEEP);
		cursor.SetCursor(0, 0);
	}

	delete[] a_mas_ghost_;
	return 0;
}
