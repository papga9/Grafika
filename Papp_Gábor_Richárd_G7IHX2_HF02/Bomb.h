#pragma once

#include <algorithm>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <chrono>
#include "DrawableTexture.h"

using namespace std::chrono;

class Bomb : public DrawableTexture
{
public:
	Bomb(std::string path, double _renderWidth, std::pair<double, double> spawnPoint, unsigned int _t);
	~Bomb();
	bool shoudlDetonate();
	void handleKeyEvent(int eventCode) override;
	void handleMouseEvent(double xPos, double yPos, bool type) override;
protected:
	int timeout = 5000000;
	steady_clock::time_point clockStart = high_resolution_clock::now();
};
