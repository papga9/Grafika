#include "Drawable.h"

Drawable::Drawable(std::string vs, std::string fs) :
	ourShader(vs.c_str(), fs.c_str())
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

Drawable::~Drawable()
{
	//TODO
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
}

const double Drawable::distance(std::pair<double, double> a, std::pair<double, double> b)
{
	return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

bool Drawable::isBetween(double start, double stop, double check)
{
	//TODO: fix this
	start += 10.0f;
	stop += 10.0f;
	check += 10.0f;

	return check > start && check < stop;
}
