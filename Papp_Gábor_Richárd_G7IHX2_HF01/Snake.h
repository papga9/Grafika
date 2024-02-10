#pragma once
#include "DrawElement.h"
#include "MoveElement.h"


enum class Facing { UP, DOWN, LEFT, RIGHT };
class Snake:public DrawElement,public MoveElement
{
private:
	Facing facing = Facing::RIGHT;									//Default irány
	void setFacing(Facing newFacing);								//Haladási irány átállítása
	void createSnake();												//Kezdeti kígyó generálása
public:
	Snake(float r, float g, float b);
	virtual void draw() override;
	void move(Facing newFacing);									//Kígyó mozgatása, DrawElement-ben létrehozott időzítő használata
	void eat();														//Kígyó növekedése
	std::deque<std::pair<float, float>> getBodyParts() const;
};

