#pragma once

#include <time.h>

class Timer {
private:
	clock_t duration;		//	duree en 1/1000ème de seconde
	clock_t nextEvent;		//	next timestamp for event

public:
	Timer(clock_t duration);
	bool isReady();
};

class Ticker {
private:
	clock_t last;

public:
	Ticker();
	void reset();
	double getEllapsedTime();
};