#include "Ground.h"

#define _USE_MATH_DEFINES
#include <math.h>


const std::pair<float, float> Ground::normalize(double x, double y, double distance)
{
	return std::make_pair(x / distance, y / distance);
}

void Ground::transformCoordsToFloatArray(float* arrayRef)
{
	for (size_t i = 0; i < coords.size(); ++i) {
		std::pair<float, float> current_coord = coords.at(i);
		arrayRef[3 * i] = current_coord.first;
		arrayRef[(3 * i) + 1] = current_coord.second;
		arrayRef[(3 * i) + 2] = 0.0f;;
	}
}

Ground::Ground(unsigned int ps, float detR) :
	Drawable("4.0.shader.vs", "4.0.shader.fs"),
	pointSize(ps),
	detonationR(detR)
{
	coords.emplace_back(std::make_pair(-1.0f, 0.0f));
	coords.emplace_back(std::make_pair(1.0f, 0.0f));

	isFinalized = false;
}

Ground::~Ground()
{
}

void Ground::finalyzeGame()
{
	isFinalized = true;
}
bool Ground::checkIfFinalized() const
{
	return isFinalized;
}

void Ground::handleKeyEvent(int eventCode)
{
	if (!isFinalized)
	{
		switch (eventCode) {
		case GLFW_KEY_J:
			//std::cout << "coors: " << coords.size() << " ---- max:" << MAX_POINTS << std::endl;
			if (coords.size() * 2 - 1 < MAX_POINTS)
			{
				CatmullClark::subDivision(coords);
			}
			break;
		case GLFW_KEY_N:
			CatmullClark::invertSubDivision(coords);
			break;

		default:
			break;
		}
	}
}

void Ground::handleMouseEvent(double xPos, double yPos, bool type)
{
	if (!isFinalized && type)
	{
		coords.push_back(std::make_pair(xPos, yPos));
	}
	//else if (!isFinalized)
	//{
	//	detonate(std::make_pair(xPos, yPos));
	//}
}

void Ground::draw(std::vector<std::pair<double, double>> detonatedBombs)
{
	std::sort(coords.begin(), coords.end(), [](const std::pair<float, float>& a, const std::pair<float, float>& b) {return a.first < b.first; });

	for (int i = 0; i < detonatedBombs.size(); i++) {
		this->detonate(detonatedBombs.at(i));
	}

	this->draw();
}

void Ground::draw()
{
	float vertices[MAX_POINTS * 3];

	transformCoordsToFloatArray(vertices);

	ourShader.use();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPointSize(20);
	glDrawArrays(GL_LINE_STRIP, 0, coords.size());
	//glDrawArrays(GL_POINTS, 0, coords.size());
}

std::vector<std::pair<double, double>> Ground::getGroundPoints() const
{
	return coords;
}

//visszaad egy kor részletet ami két limit kozott van
std::vector<std::pair<double, double>> Ground::getCirclePointsWithXLimit(std::pair<double, double> mouseP, std::pair<double, double> start, std::pair<double, double> stop, double radius)
{
	std::vector<std::pair<double, double>> result;

	for (int i = 0; i < 360; i++)
	{
		//kor keplet
		auto temp = std::make_pair(mouseP.first + cos(i * M_PI / 180) * radius, mouseP.second + sin(i * M_PI / 180) * radius);

		double dist1 = distance(start, temp), dist2 = distance(stop, temp);
		//kivalasztjuk a kozelebb levo szelso pont Y erteket mert a felkorunknek ahhoz kell "csatlakoznia"
		double localYMin = dist1 > dist2 ? stop.second : start.second;

		if (isBetween(start.first, stop.first, temp.first) && (temp.second < localYMin))
		{
			result.push_back(temp);
		}
	}

	return result;
}

//ha a robbanás során egy "sziget" lebegve marad akkor arra úgy tekintünk mintha leesett volna.
void Ground::detonate(std::pair<double, double> mouseP)
{
	int firstId = -1, lastId = -1;

	//megkeressuk az elso es utolso olyan pontot amelyiket fel szeretnénk robbantani
	for (int i = 0; i < coords.size(); i++)
	{
		double dist = distance(mouseP, coords[i]);

		if ((dist < detonationR) ||
			(isBetween(mouseP.first - detonationR, mouseP.first + detonationR, coords[i].first) &&
				coords[i].second >= mouseP.second))
		{
			if (firstId < 0)
			{
				firstId = i;
			}
			else
			{
				lastId = i;
			}
		}
	}

	if (firstId >= 0 && lastId > 0 && lastId > firstId)
	{
		//ha a robbantani valo pont egy dombba ekelodik bele akkor a felkor legszelsobb pontja lesz az uj dombtetonk
		if (isBetween(mouseP.first - detonationR, mouseP.first + detonationR, coords[firstId].first) &&
			coords[firstId].second >= mouseP.second)
		{
			coords[firstId].second = mouseP.second;
		}


		if (isBetween(mouseP.first - detonationR, mouseP.first + detonationR, coords[lastId].first) &&
			coords[lastId].second >= mouseP.second)
		{
			coords[lastId].second = mouseP.second;
		}

		std::vector<std::pair<double, double>> circlePoints = getCirclePointsWithXLimit(mouseP, coords[firstId], coords[lastId], detonationR);

		double temp = circlePoints.size() / (float(lastId - firstId));

		if (temp > 0)
		{
			for (int i = firstId; i <= lastId; i++)
			{
				//meret checkolasa
				if (int((i - firstId) * temp) >= circlePoints.size())
					break;

				//ahelyett hogy torolnenk a coords elemeit felulirjuk a kor koordinataival
				coords[i] = circlePoints[int((i - firstId) * temp)];

				if (coords[i].second < -1)
				{
					coords[i].second = -1;
				}
			}
		}
	}
}

