#include "Wall2.h"

Wall::Wall(SDL_Rect wall)
	: wall(wall), color(105 - rand() % 20, 105 - rand() % 20, 105 + rand() % 20, SDL_ALPHA_OPAQUE)
{
}

void Wall::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &wall);
}

SDL_Rect Wall::getWall()
{
	return this->wall;
}
