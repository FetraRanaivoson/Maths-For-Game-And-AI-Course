#include "Wall.h"

Wall::Wall(SDL_Rect wall)
	: wall(wall)
{
}

void Wall::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 40, 255, 10, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &wall);
}

SDL_Rect Wall::getWall()
{
	return this->wall;
}
