#pragma once
#include "../lib_Point/Point.h"
#include <random>
class Wall
{
private :
	SDL_Rect wall;
	Color color;


public:
	Wall(SDL_Rect wall);
	void draw(SDL_Renderer* renderer);

	SDL_Rect getWall();
};

