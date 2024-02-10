#include "Bomb.h"


Bomb::Bomb(std::string path, double _renderWidth, std::pair<double, double> spawnPoint, unsigned int _t) :
	DrawableTexture("4.0.shader_tex.vs", "4.0.shader_tex.fs", path, _renderWidth, spawnPoint),
	timeout(_t)
{
	clockStart = high_resolution_clock::now();
}

Bomb::~Bomb() {}

void Bomb::handleKeyEvent(int eventCode)
{
}

void Bomb::handleMouseEvent(double xPos, double yPos, bool type)
{
}

bool Bomb::shoudlDetonate()
{
	return duration_cast<microseconds>(high_resolution_clock::now() - clockStart).count() >= timeout;
}

