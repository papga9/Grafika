#include "Worm.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "shader.h"

#include "stb_image.h"


Worm::Worm(std::string path, double _renderWidth, std::pair<double, double> spawnPoint, unsigned int spawnId, int _keyBomb, int _keyRight, int _keyLeft, int _keyJump) :
	DrawableTexture("4.0.shader_tex.vs", "4.0.shader_tex.fs", path, _renderWidth, spawnPoint),
	currentId(spawnId),
	keyBomb(_keyBomb),
	keyRight(_keyRight),
	keyLeft(_keyLeft),
	keyJump(_keyJump),
	hp(100)
{
	pointLimit = 0;
	speed = 0.01;
}

Worm::~Worm()
{
	//TODO fix
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
}

void Worm::handleKeyEvent(int eventCode)
{
}

std::pair<double, double> Worm::calculateNewPosition(std::pair<double, double> p1, std::pair<double, double> p2, double d)
{
	//kiszamoljuk a jelenlegi es a kovetkezo pont kozotti szakaszon levo d tavolsagra levo pontot
	//a jelenlegi es kovetkezo pont alapjan vett vector altal.
	double p12Dist = distance(p1, p2);
	std::pair<double, double> p3(std::make_pair<double, double>(
		p1.first + abs(d) * (p2.first - p1.first) / p12Dist,
		p1.second + abs(d) * (p2.second - p1.second) / p12Dist));

	return p3;
}

void Worm::startJump()
{
	//jumpStarted = true;
	//save start coordinate
	//set up jump vector
	//set up max dist
}

void Worm::jump()
{
	////refresh coordinate based on jump
	//if ()//
	//{
	//	jumpStarted = false;
	//}
}

bool Worm::handleKeyEvent(int eventCode, std::vector<std::pair<double, double>> groundPoints)
{
	bool result = false;
	double temp = 0;

	//if (jumpStarted)
	//{
	//	jump();
	//	return;
	//}

	if (eventCode == keyBomb)
	{
		result = true;
	}
	else if (eventCode == keyJump)
	{
		//startJump();
	}
	else if (eventCode == keyRight)
	{
		shouldFlip = true;
		currentDist += speed;
	}
	else if (eventCode == keyLeft)
	{
		shouldFlip = false;
		currentDist -= speed;
	}

	if (currentDist >= 0)
	{
		if (currentId + 2 < groundPoints.size())
		{
			temp = distance(groundPoints.at(currentId), groundPoints.at(currentId + 1));
			while (currentDist > temp && (groundPoints.size() > currentId + 2)) {
				currentDist -= temp;
				currentId++;
				temp = distance(groundPoints.at(currentId), groundPoints.at(currentId + 1));
				//addig kell levonnunk a tavolsagot es leptetnunk a kovetkezo pont utanig a kukacot ameddig a currentDist kissebb nem lesz mint a
				//jelenlegi es kovetkezo pont kozotti szakasz hossza.
				//ha ezt nem tennenk meg a vegtelensegig lepkednenk az adott szakasz altal meghatarozott egyenesen
				//ugyanez vonatkozik a kovetkezo blokkra
			}
			if (groundPoints.size() > currentId + 2) {
				//az utolso kett pont szukseges, hogy megfelelo poziciot es vectort tudjunk szamolni. 
				//Ezert illetve hogy ne logjunk ki a poalyarol az utolso pont tiltott.
				currentPosition = calculateNewPosition(groundPoints.at(currentId), groundPoints.at(currentId + 1), currentDist);
			}
		}
		else
		{
			//elertunk a palya szelehez le kell vonnunk a mar hozzaadott sebesseget amit nem tudtunk "vegrehajtani"
			currentDist -= speed;
		}
	}
	else if (currentDist < 0)
	{
		//ugyanaz mint az elozo blokk csak also limitekkel
		if (currentId > 1)
		{
			temp = distance(groundPoints.at(currentId), groundPoints.at(currentId - 1));
			while (abs(currentDist) > temp && (currentId > 1)) {
				currentDist += temp;
				currentId--;
				temp = distance(groundPoints.at(currentId), groundPoints.at(currentId - 1));
			}
			if (currentId > 1)
			{
				currentPosition = calculateNewPosition(groundPoints.at(currentId), groundPoints.at(currentId - 1), currentDist);
			}
		}
		else
		{
			currentDist += speed;
		}
	}

	return result;
}

void Worm::handleMouseEvent(double xPos, double yPos, bool type)
{
}
unsigned int Worm::getHP()
{
	return hp;
}

void Worm::decreaseHP(int damage)
{
	if (hp <= damage)
	{
		hp = 0;
	}
	else
	{
		hp -= damage;
	}
}
