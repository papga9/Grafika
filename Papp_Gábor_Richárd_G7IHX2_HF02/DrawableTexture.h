#pragma once

#include <algorithm>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Drawable.h"

class DrawableTexture : public Drawable
{
public:
	DrawableTexture(std::string vs, std::string fs, std::string _path, double _rw, std::pair<double, double> spawnPoint);
	~DrawableTexture();
	void draw()override;
	std::pair<double, double> getPosition() const;

protected:
	std::string path;
	unsigned int texture;
	const static unsigned int VERTICES_SIZE = 32U;
	double renderWidth;
	bool shouldFlip = false;
	std::pair<double, double> currentPosition;

	float vertices[VERTICES_SIZE] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};

	void refreshVertices();
};

