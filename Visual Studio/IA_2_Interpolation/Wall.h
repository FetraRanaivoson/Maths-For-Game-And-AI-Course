#pragma once
#include "../lib_Point/Point.h"
class Wall
{
private :
	SDL_Rect wall;


public:
	Wall(SDL_Rect wall);
	void draw(SDL_Renderer* renderer);

	SDL_Rect getWall();
};

