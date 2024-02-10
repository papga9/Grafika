#pragma once


#include <vector>
#include<iostream>
class CatmullClark
{
private:
public:
	static const void subDivision(std::vector<std::pair<double, double>>& coords);
	static const void invertSubDivision(std::vector<std::pair<double, double>>& coords);
};

