#include "GameMaster.h"


/*
* A játéklogika kezelése
*/

const std::string GameMaster::wormPath = "Worm.png";	//Worm clipart with creative commons license: https://cdn.pixabay.com/photo/2019/02/15/12/00/earth-worm-3998335_1280.png
const std::string GameMaster::bombPath = "Bomb.png";	//Bomb clipart with creative commons license: https://www.pngall.com/bomb-vector-png/download/54358


GameMaster::GameMaster(unsigned int _screenW, unsigned int _screenH) :
	Drawable("4.0.shader.vs", "4.0.shader.fs"),
	ground(dotSize, detonationR),
	screenW(_screenW),
	screenH(_screenH),
	isGameFinished(false)
{
	std::cout << "USER MANUAL" << std::endl << std::endl;

	std::cout << "GENERAL SETTINGS" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << "Add new point: " << "Left click" << std::endl;
	//std::cout << "Drop Bomb: " << "Right click" << std::endl;
	std::cout << "CatmulClark addition: " << "J" << std::endl;
	std::cout << "CatmulClark removal: " << "N" << std::endl;
	std::cout << "Start: " << "ENTER" << std::endl << std::endl;
	std::cout << "Close: " << "Esc" << std::endl << std::endl;


	std::cout << "PLAYER#1" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << "Move left (Press or Hold for quicker movement): " << "<-" << std::endl;
	std::cout << "Move right (Press or Hold for quicker movement): " << "->" << std::endl;
	std::cout << "Drop bomb: " << "B" << std::endl;
	std::cout << "JUMP: " << "SPACE" << std::endl << std::endl;

	std::cout << "PLAYER#2" << std::endl;
	std::cout << "_______________________________" << std::endl;
	std::cout << "Move left (Press or Hold for quicker movement): " << "A" << std::endl;
	std::cout << "Move right (Press or Hold for quicker movement): " << "D" << std::endl;
	std::cout << "Drop bomb: " << "Q" << std::endl;
	std::cout << "JUMP: " << "W" << std::endl;
}

void GameMaster::finalyzeGame()
{
	ground.finalyzeGame();
	std::vector<std::pair<double, double>> gp = ground.getGroundPoints();
	unsigned int rand1 = rand() % (gp.size()-2)+1, rand2 = rand() % (gp.size() - 2) + 1;
	worms.push_back(Worm(wormPath, 2 * wormRatio, gp[rand1], rand1, GLFW_KEY_B, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_SPACE));
	worms.push_back(Worm(wormPath, 2 * wormRatio, gp[rand2], rand2, GLFW_KEY_Q, GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_W));
	//TODO: lehetséges további játékosok hozzáadása
}

void GameMaster::handleKeyEvent(int keyCode) {

	if (isGameFinished)
	{
		return;
	}

	switch (keyCode) {
	case GLFW_KEY_ENTER:
		if (!ground.checkIfFinalized())
		{
			this->finalyzeGame();
		}
		break;
	default:
		break;
	}

	ground.handleKeyEvent(keyCode);

	for (int i = 0; i < worms.size(); i++)
	{
		if (worms[i].handleKeyEvent(keyCode, ground.getGroundPoints()))
		{
			bombs.push_back(Bomb(bombPath, 2 * bombRatio, worms[i].getPosition(), 5000000));
		}
	}

	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].handleKeyEvent(keyCode);
	}
}

void GameMaster::handleMouseEvent(double xPos, double yPos, bool type)
{
	if (isGameFinished)
	{
		return;
	}

	ground.handleMouseEvent(xPos, yPos, type);

	for (int i = 0; i < worms.size(); i++)
	{
		worms[i].handleMouseEvent(xPos, yPos, type);
	}

	for (int i = 0; i < bombs.size(); i++)
	{
		bombs[i].handleMouseEvent(xPos, yPos, type);
	}
}

void GameMaster::finishGame()
{
	if (isGameFinished)
	{
		return;
	}

	int playerId = -1;
	int maxHP = 0;

	for (int i = 0; i < worms.size(); i++)
	{
		if (worms[i].getHP() > maxHP)
		{
			maxHP = worms[i].getHP();
			playerId = i;
		}
	}

	std::cout << "GAME OVER" << std::endl;

	if (playerId > -1 && maxHP > 0)
	{
		std::cout << "Player #" << playerId << " is the winer!" << std::endl;
	}
	else
	{
		std::cout << "EVERYONE IS A LOSER" << std::endl;
	}
	isGameFinished = true;
}

void GameMaster::draw() {

	std::vector<std::pair<double, double>> detonatedBombs;

	for (int i = 0; i < bombs.size(); i++)
	{
		if (bombs[i].shoudlDetonate())
		{
			if (rand() % 100 >= failureChance)
			{
				detonatedBombs.push_back(bombs[i].getPosition());
				std::cout << "A bomb has been detonated at: " << bombs[i].getPosition().first << " " << bombs[i].getPosition().second << std::endl;
			}
			else {
				std::cout << "Failed bomb at: " << bombs[i].getPosition().first << " " << bombs[i].getPosition().second << std::endl;
			}

			bombs.erase(bombs.begin() + i);
			i--;
		}
		else
		{
			bombs[i].draw();
		}
	}

	ground.draw(detonatedBombs);

	for (int i = 0; i < worms.size(); i++)
	{
		for (int j = 0; j < detonatedBombs.size(); j++)
		{
			if (distance(worms[i].getPosition(), detonatedBombs[j]) <= detonationR)
			{
				worms[i].decreaseHP(detonationDamage);
				std::cout << "Player #" << i << " Health: " << worms[i].getHP() << std::endl;
				if (worms[i].getHP() == 0)
				{
					finishGame();
				}
			}
		}

		worms[i].draw();
	}

}
