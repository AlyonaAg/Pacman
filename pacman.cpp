#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <iostream> 
#include <algorithm> 
#include <ctime>
#include "pacman.h"

Field::Field()
{
	unsigned char field[HEIGH][WIDTH] =
	{
		{ "##################################" },
		{ "#               ##               #" },
		{ "#  ####  #####  ##  #####  ####  #" },
		{ "#  ####  #####  ##  #####  ####  #" },
		{ "#                                #" },
		{ "#  ####  ##  ########  ##  ####  #" },
		{ "#        ##     ##     ##        #" },
		{ "#######  #####  ##  #####  #######" },
		{ "#######  ##            ##  #######" },
		{ "#######  ##  ###  ###  ##  #######" },
		{ "#            #      #            #" },
		{ "#            ########            #" },
		{ "#######  ##            ##  #######" },
		{ "#######  ##  ########  ##  #######" },
		{ "#               ##               #" },
		{ "#  ####  #####  ##  #####  ####  #" },
		{ "#    ##                    ##    #" },
		{ "###  ##  ##  ########  ##  ##  ###" },
		{ "#        ##     ##     ##        #" },
		{ "#  ###########  ##  ###########  #" },
		{ "#                                #" },
		{ "##################################" }
	};

	a_count_food_ = 0;

	a_play_field_ = new unsigned char*[HEIGH];
	for (size_t i = 0; i < HEIGH; i++)
		a_play_field_[i] = new unsigned char[WIDTH];

	for (size_t i = 0; i < HEIGH; i++)
		for (size_t j = 0; j < WIDTH; j++)
			a_play_field_[i][j] = field[i][j];

	for (size_t i = 0; i < HEIGH; i++)
	{
		for (size_t j = 0; j < WIDTH; j++)
		{
			if (a_play_field_[i][j] == ' ')
			{
				a_play_field_[i][j] = '.';
				a_count_food_++;
			}
		}
	}

	a_mas_coin = new Coin[MAX_COUNT_COIN];
	for (int i = 0; i < MAX_COUNT_COIN;i++)
		a_mas_coin[i].InitCoin(*this);
}

void Field::Show()const
{
	for (size_t i = 0; i < HEIGH; i++)
	{
		for (size_t j = 0; j < WIDTH; j++)
			printf("%c", a_play_field_[i][j]);
		printf("\n");
	}
}

unsigned char Field::GetCell(size_t x, size_t y)const
{
	return a_play_field_[x][y];
}

void Field::SetCell(size_t x, size_t y, unsigned char symbol)
{
	a_play_field_[x][y] = symbol;
}

void Field::CheckCollision(Ghost* mas_ghost, Pacman &pacman)
{
	size_t pac_x = pacman.GetPos()->GetPosX();
	size_t pac_y = pacman.GetPos()->GetPosY();

	for (int i = 0; i < COUNT_GHOST; i++)
	{
		size_t ghost_x = mas_ghost[i].GetPos()->GetPosX();
		size_t ghost_y = mas_ghost[i].GetPos()->GetPosY();
		
		if (mas_ghost[i].CheckLives())
			if (pac_x == ghost_x && pac_y == ghost_y)
			{
				if (!pacman.GetChase())
				{
					if (!pacman.GetTime())
						pacman.Wound();
				}
				else
					mas_ghost[i].Wound();
			}
	}
}

bool Field::CheckFood()const
{
	for (size_t i = 0; i < HEIGH; i++)
		for (size_t j = 0; j < WIDTH; j++)
			if (a_play_field_[i][j] == '.')
				return true;
	return false;
}

size_t Field::GetFood()const
{
	return a_count_food_;
}

Coin* Field::GetMasCoin()const
{
	return a_mas_coin;
}

void Field::CheckCoin(Coin &coin, Pacman &pacman)
{
	Position* pos = coin.GetPos();

	coin.ChangeLifetime();

	if (!coin.GetLifetime()
		|| GetCell(pos->GetPosX(), pos->GetPosY()) == ' ')
	{
		// if pacman took the coin
		if (GetCell(pos->GetPosX(), pos->GetPosY()) == ' ')
		{
			if (coin.GetPrevSymbol(*this) == '.')
				pacman.NewFood();

			SetCell(pos->GetPosX(), pos->GetPosY(), ' ');
		}
		else
			SetCell(pos->GetPosX(), pos->GetPosY(), coin.GetPrevSymbol(*this));
		coin.InitCoin(*this);
	}
}

Field::~Field()
{
	delete[] a_mas_coin;
	for (size_t i = 0; i < HEIGH; i++)
		delete[] a_play_field_[i];
	delete[] a_play_field_;
}


void Position::SetPosition(size_t x, size_t y)
{
	a_pos_x_ = x;
	a_pos_y_ = y;
}

size_t Position::GetPosX()const
{
	return a_pos_x_;
}

size_t Position::GetPosY()const
{
	return a_pos_y_;
}


Character::Character()
{
	a_position_ = new Position;
	a_input_x_ = 0;
	a_input_y_ = 0;
}

Position* Character::GetPos()const
{
	return a_position_;
}

size_t Character::GetLives()const
{
	return a_lives_;
}

bool Character::CheckLives()const
{
	if (a_lives_)
		return true;
	return false;
}


void Ghost::InitGhost(Field &field)
{
	int x, y;
	do
	{
		x = 1 + rand() % (HEIGH - 1);
		y = 1 + rand() % (WIDTH - 1);
	} while (field.GetCell(x,y) != '.');
	field.SetCell(x, y, GHOST);

	a_position_->SetPosition(x, y);
	a_prev_symbol_ = '.';

	a_lives_ = 1;
}

void Ghost::Move(Field &field)
{
	if (a_lives_)
	{
		size_t new_pos = rand() % 4;
		int vx, vy;
		switch (new_pos)
		{
		case 0:
			vx = 0; vy = 1;
			break;
		case 1:
			vx = 0; vy = -1;
			break;
		case 2:
			vx = 1; vy = 0;
			break;
		case 3:
			vx = -1; vy = 0;
			break;
		}

		size_t nx, ny;
		nx = a_position_->GetPosX() + vx;
		ny = a_position_->GetPosY() + vy;

		if (field.GetCell(nx, ny) == '#')
			return;
		if (field.GetCell(nx, ny) == GHOST)
			return;

		field.SetCell(a_position_->GetPosX(), a_position_->GetPosY(), a_prev_symbol_);

		if (field.GetCell(nx, ny) != PACMAN)
			a_prev_symbol_ = field.GetCell(nx, ny);
		else
			a_prev_symbol_ = ' ';

		a_position_->SetPosition(nx, ny);
		field.SetCell(nx, ny, GHOST);
	}
}

void Ghost::Wound()
{
	a_lives_--;
}

char Ghost::GetPrevSymbol()const
{
	return a_prev_symbol_;
}


void Cursor::HideCursor()const
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void Cursor::SetCursor(size_t x, size_t y)const
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void Pacman::InitPacman(Field &field)
{
	a_position_->SetPosition(1, 1);
	a_lives_ = 3;
	a_food_collected_ = 0;
	a_coin_ = 0;
	a_time_without_death_ = 0;
	a_chase_ = 0;
	field.SetCell(1, 1, PACMAN);
}

void Pacman::Wound()
{
	a_lives_--;
	ChangeTime(TIME_WITHOUT_DEATH);
}

void Pacman::ChangeTime(int count)
{
	a_time_without_death_ += count;
}

void Pacman::NewFood()
{
	a_food_collected_++;
}

void Pacman::NewCoin()
{
	a_coin_++;
	if (a_coin_ == 3)
	{
		a_chase_ = TIME_CHASE;
		a_coin_ = 0;
	}
}

size_t Pacman::GetFood()const
{
	return a_food_collected_;
}

size_t Pacman::GetCoin()const
{
	return a_coin_;
}

size_t Pacman::GetTime()const
{
	return a_time_without_death_;
}

void Pacman::Move(Field &field)
{
	if (GetTime())
		ChangeTime(-1);

	if (GetChase())
		ChangeChase(-1);

	if (_kbhit())
	{
		char c1 = _getch();
		if (c1 == -32)
		{
			char c2 = _getch();
			a_input_x_ = 0;
			a_input_y_ = 0;


			switch (c2)
			{
			case 72: a_input_x_ = -1; break; // cursor up
			case 80: a_input_x_ = +1; break; // cursor down
			case 75: a_input_y_ = -1; break; // cursor left
			case 77: a_input_y_ = +1; break; // cursor right            
			}
			if (!a_input_x_ && !a_input_y_)
				return;
		}
	}
	// 1. compute new desired coordinate (nx,ny)
	int nx = a_input_x_ + a_position_->GetPosX();
	int ny = a_input_y_ + a_position_->GetPosY();

	// 2. test whether there is a wall at (nx,ny) 
	if (field.GetCell(nx, ny) == '#')
	{
		a_input_x_ = 0;
		a_input_y_ = 0;
		return;
	}

	if (field.GetCell(nx, ny) == GHOST)
	{
	}

	// 3. delete PacMan from old position
	field.SetCell(a_position_->GetPosX(), a_position_->GetPosY(), ' ');

	// 4. update PacMan's coordinate
	a_position_->SetPosition(nx, ny);

	// 5. is there a food piece at the new location?
	if (field.GetCell(nx, ny) == '.')
		NewFood();


	if (field.GetCell(nx, ny) == COIN)
	{
		NewCoin();
	}

	// 6. put PacMan back again to playfield
	field.SetCell(nx, ny, PACMAN);

}

size_t Pacman::GetChase()const
{
	return a_chase_;
}

void Pacman::ChangeChase(int count)
{
	a_chase_ += count;
}


void Coin::InitCoin(Field &field)
{
	a_position_ = new Position;
	a_lifetime_ = MIN_TIMELIFE_COIN + rand() % (MAX_TIMELIFE_COIN - MIN_TIMELIFE_COIN);

	int x = 0, y = 0;
	do
	{
		x = 1 + rand() % (HEIGH - 1);
		y = 1 + rand() % (WIDTH - 1);
	} while (field.GetCell(x, y) != '.' && field.GetCell(x, y) != ' ');
	a_prev_symbol_ = field.GetCell(x, y);
	field.SetCell(x, y, COIN);

	a_position_->SetPosition(x, y);
}

void Coin::ChangeLifetime()
{
	a_lifetime_--;
}

Position* Coin::GetPos()const
{
	return a_position_;
}

size_t Coin::GetLifetime()const
{
	return a_lifetime_;
}

void Coin::SetPrevSymbol(char symbol)
{
	a_prev_symbol_ = symbol;
}

char Coin::GetPrevSymbol(const Field &field)const
{
	return a_prev_symbol_;
}