#pragma once

#include <algorithm>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "DrawableTexture.h"

class Worm: public DrawableTexture
{
public:
	Worm(std::string _path, double _rw, std::pair<double, double> sp, unsigned int sId, int _keyBomb, int _keyRight, int _keyLeft, int _keyJump);
	~Worm();

	void handleKeyEvent(int eventCode)override;
	bool handleKeyEvent(int eventCode, std::vector<std::pair<double, double>>);

	void handleMouseEvent(double xPos, double yPos, bool type) override;

	unsigned int getHP();
	void decreaseHP(int damage);

protected:
	unsigned int currentId;
	const int keyBomb, keyRight, keyLeft, keyJump;
	unsigned int pointLimit;
	unsigned int hp;
	double speed;
	double currentDist = 0;
	bool jumpStarted = false;

	std::pair<double, double> calculateNewPosition(std::pair<double, double> p1, std::pair<double, double> p2, double d);
	void startJump();
	void jump();
};

