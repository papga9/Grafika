#pragma once
#include <chrono>

using namespace std::chrono;
class MoveElement
{
protected:
	long referenceDuration = 60000;
	steady_clock::time_point clockStart = high_resolution_clock::now();				//Konstans kígyó sebesség elérésére időzítők lértehozása
	steady_clock::time_point clockEnd = high_resolution_clock::now();
	void startTimer();
	void endTimer();
	long getDifference();
};

