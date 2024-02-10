#pragma once

#include "Ground.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>
#include <vector>
#include "Worm.h"
#include "Bomb.h"

/*
Egyszerre kezeli a különböző játékelemeket
*/

class GameMaster : public Drawable
{
private:
	unsigned int screenW, screenH;

	double wormRatio = 0.05;
	double bombRatio = 0.03;
	int failureChance = 20;
	int detonationDamage = 20;
	double detonationR = 0.3;
	int dotSize = 0;
	bool isGameFinished;

	static const std::string wormPath;
	static const std::string bombPath;

	Ground ground;
	std::vector<Worm> worms;
	std::vector<Bomb> bombs;

public:
	GameMaster(unsigned int screenW, unsigned int  screenH);
	void finalyzeGame();
	void handleKeyEvent(int eventCode)override;
	void handleMouseEvent(double xPos, double yPos, bool type) override;
	void draw() override;
	void finishGame();
};

