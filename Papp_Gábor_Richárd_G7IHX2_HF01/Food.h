#pragma once

#include "DrawElement.h"



class Food:public DrawElement
{
private:
	bool poisoned;																				//Egy osztályba tartozik az ehető és mérgező bogyók, ez a változóval lehet különbséget tenni
	void generateColor();
public:
	Food(bool _poisoned);
	bool isPoisoned() const;
	void regenerate(std::deque<std::pair<float,float>> excludePositions, float tolerance);		//Evés után új bogyó generálása
	virtual void draw() override;

	
};

