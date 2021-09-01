#include "SimplePoint.h"

void SimplePoint::lerp(Vector initialPosition, Vector destinationPosition, double alpha)
{
	if (alpha >= 0 && alpha <= 1) {
		position = (1 - alpha) * initialPosition + alpha * destinationPosition;
	}
}

SimplePoint::SimplePoint(Vector position, int size, double alpha, Color pointColor)
	: position(position), pointColor(pointColor), vitesse(0, 0), acceleration(0, 0)
{
	//this->position = position;
	this->size = size;
	this->alpha = alpha;


	this->maxSpeed = 6;
	this->targetSpeed = 8;
}


void SimplePoint::followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY)
{
	Vector destination(clickPosX, clickPosY);
	this->lerp(position, destination, alpha);
}

void SimplePoint::followRealistic(SDL_Renderer* renderer, int clickPosX, int clickPosY)
{
	currentTime = SDL_GetTicks();

	if (currentTime - latUpdate > deltaTime) {

		if (vitesse.magnitude() > 100) {
			vitesse = { 0,0 };
		}

		
		Vector destinationPosition(clickPosX, clickPosY);
		if (alpha >= 0 && alpha <= 1) {
			position = (1 - alpha) * position + alpha * destinationPosition;
			position += vitesse;
			vitesse += acceleration;
		}
		Vector currentVitesse = vitesse;


		acceleration = { position.x - currentVitesse.x , position.y - currentVitesse.y };

		/*if (acceleration.magnitude() > 500)
			acceleration = { position.x - currentVitesse.x , position.y - currentVitesse.y };*/

		latUpdate = currentTime;
	}


}


void SimplePoint::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	position.draw(renderer, pointColor, size);
}

Point& SimplePoint::getPosition()
{
	return position;
}
