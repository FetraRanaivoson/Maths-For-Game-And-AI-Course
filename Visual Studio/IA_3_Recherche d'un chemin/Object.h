#pragma once
#include "../lib_Point/Point.h"
#include <time.h>
#include <random>
#include <cstdlib>

class Object
{
private:
	Point position;
	double radius;
	Color color;

	int windowW, windowH;

	Uint32 currentTime = 0;
	Uint32 lastUpdate = 0;
	int refreshTime = 1000;

public:
	Object(Point position, double radius, Color color, int windowW, int windowH);
	void draw(SDL_Renderer* renderer);
	void drawRandom(SDL_Renderer* renderer);

	Point& getPosition();
	double getRadius();

	void setPosition(Point newPosition);
};

