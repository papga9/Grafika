#pragma once

#include <algorithm>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Drawable
{
public:
	Drawable(std::string vs, std::string fs);
	~Drawable();
	virtual void handleKeyEvent(int eventCode) = 0;
	//type: true for left, false for right
	virtual void handleMouseEvent(double xPos, double yPos, bool type) = 0;
	virtual void draw() = 0;

protected:
	Shader ourShader;
	unsigned int VBO, VAO, EBO;

	static const double distance(std::pair<double, double> a, std::pair<double, double> b);
	bool isBetween(double start, double stop, double check);
};

