#include "Object.h"

Object::Object(Point position, double radius, Color color, int windowW, int windowH)
	: position(position), radius(radius), color(color)
{
	this->windowW = windowW;
	this->windowH = windowH;
}

void Object::draw(SDL_Renderer* renderer)
{
	position.drawCircle(renderer, radius, color, true);
}

void Object::drawRandom(SDL_Renderer* renderer)
{
	currentTime = SDL_GetTicks();
	if (currentTime - lastUpdate > rand() % refreshTime + 5000) {
		position.x = rand() % windowW;
		position.y = rand() % windowH;
		position.drawCircle(renderer, radius, color, true);

		lastUpdate = currentTime;
	}
	else {
		position.drawCircle(renderer, radius, color, true);
		position.drawCircle(renderer, radius - 2, color, true);
		position.drawCircle(renderer, radius - 4, color, true);
		position.drawCircle(renderer, radius - 6, color, true);
		position.drawCircle(renderer, radius - 8, color, true);
		position.drawCircle(renderer, radius - 9, color, true);
	}
	
}

Point& Object::getPosition()
{
	return position;
}

double Object::getRadius()
{
	return this->radius;
}

void Object::setPosition(Point newPosition)
{
	this->position = position;
}
