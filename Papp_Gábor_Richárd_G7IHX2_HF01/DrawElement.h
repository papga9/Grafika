#pragma once

#include <algorithm>
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <deque>

/*
* Az étel és a kígyó is ebből az ősosztályból származik
*/

class DrawElement
{
public:
    DrawElement();
	DrawElement(float _x, float _y);
	DrawElement(float _x, float _y,float r, float g, float b);
	virtual void draw() =0; 
	std::pair<float, float> getPosition() const;
	float getScale() const;

protected:
	Shader ourShader;
	float r, g, b;
	std::deque<std::pair<float, float>> bodyParts;
	float pos_x, pos_y;
	float SCALE  = 0.01f;												//SCALE változóval egyszerre lehet a játék elemek és ablak relatív méretét változtatni
	unsigned int VBO, VAO, EBO;

	virtual void initialize();											//Inicializációs lépések egyszeri lefuttatása, memóriafolyás elkerülése végett
	void drawBodyParts() const;




};

