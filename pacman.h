#define COUNT_GHOST 4
#define HEIGH 22
#define WIDTH 35
#define GHOST 234 
#define PACMAN 233 
#define COIN 157
#define TIME_SLEEP 30
#define TIME_WITHOUT_DEATH 20
#define MAX_COUNT_COIN 4
#define MAX_TIMELIFE_COIN 60
#define MIN_TIMELIFE_COIN 20
#define TIME_CHASE 60

#include <stdio.h>

class Character;
class Ghost;
class Pacman;
class Field;


class Position
{
public:
	Position(){};
	void SetPosition(size_t x, size_t y);
	size_t GetPosX() const;
	size_t GetPosY() const;
	~Position(){};
private:
	size_t a_pos_x_, a_pos_y_;
};

class Coin
{
public:
	Coin(){};
	void InitCoin(Field &field); 
	void ChangeLifetime();
	Position* GetPos() const;
	size_t GetLifetime() const;
	void SetPrevSymbol(char symbol); 
	char GetPrevSymbol(const Field &field) const;
	~Coin(){ delete a_position_; };
private:
	char a_prev_symbol_;
	Position* a_position_;
	size_t a_lifetime_;
};

class Field
{
public:
	Field();
	void Show()const;
	void SetCell(size_t x, size_t y, unsigned char symbol);
	unsigned char GetCell(size_t x, size_t y) const;
	void CheckCollision(Ghost* mas_ghost, Pacman &pacman);
	bool CheckFood()const;
	size_t GetFood() const;
	Coin* GetMasCoin() const;
	void CheckCoin(Coin &mas_coin, Pacman &pacman);
	~Field();
private:	
	unsigned char** a_play_field_/*[HEIGH][WIDTH]*/;
	Coin* a_mas_coin;
	size_t a_count_food_;
};

class Character
{
public:
	Character();
	virtual void Move(Field &field) = 0;
	virtual void Wound() = 0;
	Position* GetPos()const;
	size_t GetLives()const;
	bool CheckLives()const;
	~Character(){ delete a_position_; };
protected:
	size_t a_lives_;
	Position* a_position_;
	int a_input_x_, a_input_y_;
};

class Pacman : public Character
{
public:
	Pacman(){}; 
	void InitPacman(Field &field); 
	void Wound(); 
	void NewFood(); 
	void NewCoin(); 
	void ChangeTime(int count); 
	size_t GetTime()const;
	size_t GetFood()const;
	size_t GetCoin()const;
	virtual void Move(Field &field);
	size_t GetChase()const;
	void ChangeChase(int count);
	~Pacman(){}; 
private:
	size_t a_food_collected_;
	size_t a_coin_;
	size_t a_time_without_death_;
	size_t a_chase_;
};

class Ghost : public Character
{
public:
	Ghost(){};
	void InitGhost(Field &field);
	void Move(Field &field);
	void Wound();
	char GetPrevSymbol()const;
	~Ghost(){};
private:
	char a_prev_symbol_;
};

class Cursor
{
public:
	void HideCursor()const;
	void SetCursor(size_t x, size_t y)const;
};


class Game
{
public: 
	void StartGame();
};
