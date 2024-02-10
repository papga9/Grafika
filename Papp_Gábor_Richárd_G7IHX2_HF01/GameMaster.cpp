#include "GameMaster.h"
#include <GLFW/glfw3.h>


GameMaster::GameMaster():snake(0.9f,0.9f,0.9f), goodFood(false), badFood(true)
{
}

/*
* A játéklogika kezelése
*/

void GameMaster::handleKeyEvent(int keyCode) {
	switch (keyCode) {
	case GLFW_KEY_UP:
		snake.move(Facing::UP);
		break;
	case GLFW_KEY_DOWN:
		snake.move(Facing::DOWN);
		break;
	case GLFW_KEY_RIGHT:
		snake.move(Facing::RIGHT);
		break;
	case GLFW_KEY_LEFT:
		snake.move(Facing::LEFT);
		break;
	}


	std::pair<float, float> snakeHead = snake.getPosition();
	std::pair<float, float> goodFoodPosition = goodFood.getPosition();
	std::pair<float, float> badFoodPosition = badFood.getPosition();

	float distanceFromGoodFood = sqrt(pow(goodFoodPosition.first - snakeHead.first, 2) + pow(goodFoodPosition.second - snakeHead.second, 2));
	float distanceFromBadFood = sqrt(pow(badFoodPosition.first - snakeHead.first, 2) + pow(badFoodPosition.second - snakeHead.second, 2));

	if (distanceFromBadFood < snake.getScale()) {
		exit(0);
	}
	if (distanceFromGoodFood < snake.getScale()) {
		snake.eat();

		std::deque<std::pair<float, float>> excludePoints(snake.getBodyParts());
		excludePoints.push_back(badFoodPosition);
		goodFood.regenerate(excludePoints, snake.getScale());
	}
}

void GameMaster::draw() {
	snake.draw();
	goodFood.draw();
	badFood.draw();
}