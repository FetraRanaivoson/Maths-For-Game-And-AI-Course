#include "Timer.h"

#include <time.h>

Timer::Timer(clock_t duration) {
	this->duration = duration;
	this->nextEvent = clock() + duration;
}

bool Timer::isReady() {
	bool ready = false;

	clock_t now = clock();

	if (now >= nextEvent) {
		nextEvent = now + duration;
		ready = true;
	}

	return ready;
}


Ticker::Ticker() {
	this->reset();
}

void Ticker::reset() {
	this->last = clock();
}

double Ticker::getEllapsedTime() {
	clock_t now = clock();
	clock_t ticks = now - this->last;
	this->last = now;
	return (double)ticks / CLOCKS_PER_SEC;
}