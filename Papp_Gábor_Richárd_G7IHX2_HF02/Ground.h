#pragma once

#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "CatmullClark.hpp"
#include <glad/glad.h>
#include<algorithm>

#include "shader.h"

#include <iostream>
#include <cmath>
#include <vector>
#include "Drawable.h"

class Ground: public Drawable
{
private:
	static const unsigned int MAX_POINTS = 1000;
	const unsigned int pointSize;
	float detonationR;
	std::vector<std::pair<double, double>> coords;
	bool isFinalized;

	void transformCoordsToFloatArray(float* arrayRef);
	static const std::pair<float, float> normalize(double x, double y, double distance);
	std::vector<std::pair<double, double>> getCirclePointsWithXLimit(std::pair<double, double> mouseP, std::pair<double, double> start, std::pair<double, double> stop, double radius);

public:
	Ground(unsigned int ps, float detR);
	~Ground();

	void handleKeyEvent(int eventCode) override;
	void handleMouseEvent(double xPos, double yPos, bool type) override;
	void draw() override;

	void detonate(std::pair<double, double> mouseP);
	void draw(std::vector<std::pair<double, double>> detonatedBombs);
	void finalyzeGame();
	bool checkIfFinalized()const;
	std::vector<std::pair<double, double>> getGroundPoints()const;
};

