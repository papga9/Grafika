#pragma once

#include "Snake.h"
#include "Food.h"

/*
Egyszerre kezeli a különböző játékelemeket
*/

class GameMaster
{
private:
	Snake snake;
	Food goodFood;
	Food badFood;
public:
	GameMaster();
	void handleKeyEvent(int eventCode);
	void draw();
};

