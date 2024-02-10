#include "MoveElement.h"


void MoveElement::startTimer() {
	clockStart = high_resolution_clock::now();
}

void MoveElement::endTimer() {
	clockEnd = high_resolution_clock::now();
}

long MoveElement::getDifference() {
	return duration_cast<microseconds>(clockEnd - clockStart).count();
}