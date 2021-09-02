#include "SimplePoint.h"

void SimplePoint::lerp(Vector initialPosition, Vector destinationPosition, double alpha)
{
	if (alpha >= 0 && alpha <= 1) {
		position = (1 - alpha) * initialPosition + alpha * destinationPosition;
	}
}

SimplePoint::SimplePoint(Vector position, int size, double alpha, double maxSpeed, double maxAcceleration, Color pointColor)
	: position(position), pointColor(pointColor), speed(0, 0), acceleration(0, 0)
{
	this->size = size;
	this->alpha = alpha;

	this->maxSpeed = maxSpeed;
	this->maxAcceleration = maxAcceleration;

	//this->maxSpeed = 300;
	//this->maxAcceleration = 175;
}


void SimplePoint::followSimple(SDL_Renderer* renderer, int clickPosX, int clickPosY)
{
	Vector destination(clickPosX, clickPosY);
	this->lerp(position, destination, alpha);
}

void SimplePoint::update()
{
	currentTime = SDL_GetTicks();
	if (currentTime - latUpdate > deltaTime) {
		position += speed * (double) (currentTime -latUpdate) / 1000;
		speed += acceleration * (double) (currentTime - latUpdate) / 1000;
		latUpdate = currentTime;
	}
}

void SimplePoint::followRealistic(SDL_Renderer* renderer, Point target)
{
	//Step 1: get the direction
	Vector AB(position, target);
	//Step 2: limit this direction because it's too big
	Vector targetSpeed = AB / AB.magnitude() * maxSpeed; // =======> vecteur v * L / norme(v)  where L is the new magnitude
	//Step 3: desiredAcceleration = targetSpeed - currentSpeed 
	Vector desiredAcceleration = targetSpeed - speed;
	//Step 4: limit it because it's too big
	this->acceleration = desiredAcceleration / desiredAcceleration.magnitude() * maxAcceleration;
}


void SimplePoint::draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	position.draw(renderer, pointColor, size);

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, position.x, position.y, position.x + speed.x / 3, position.y + speed.y / 3);
}

Point& SimplePoint::getPosition()
{
	return position;
}
