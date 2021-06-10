#include "Laser.h"
#include <time.h>

__time64_t Laser::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

Laser::Laser(double mass, Point position, Vector speed, int width, int height)
	: position(position), speed(speed), acceleration(0, 0) {
	this->mass = mass;
	this->lastUpdate = this->getTimeInNanoSeconds();
	this->width = width;
	this->height = height;

}

void Laser::update(SDL_Event& event)
{
	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	this->position.update(event);

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;

	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->position.x = this->position.x + this->speed.x * deltaT;
	this->position.y = this->position.y + this->speed.y * deltaT;
	//	OR this->position = this->position + this->speed * deltaT;		//	using library operators

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;
	//	OR this->speed = this->speed + this->acceleration * deltaT;		//	using library operators

	//	compute sum of forces pallyed on object
	Vector gravity(0, 0 * this->mass);
	Vector sum = gravity;

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;
}

Point Laser::getPosition()
{
	return this->position;
}

Vector Laser::getSpeed()
{
	return this->speed;
}

void Laser::setSpeed(Vector newSpeed)
{
	this->speed = newSpeed;
}

void Laser::setAcceleration(Vector newAcceleration)
{
	this->acceleration = newAcceleration;
}

void Laser::draw(SDL_Renderer* renderer, Color color, SDL_Event& event)
{
	this->update(event);

	//	check borders of screen
	if ((this->position.x > width && this->speed.x > 0) ||
		(this->position.x < 0 && this->speed.x < 0)) {
		this->speed.x *= -0.9;
	}
	if ((this->position.y > height && this->speed.y > 0) ||
		(this->position.y < 0 && this->speed.y < 0)) {
		this->speed.y *= -0.9;
	}


	this->position.draw(renderer, color, 10);
	//this->position.drawCircle(renderer, 10, color, true);

	SDL_RenderDrawLine(renderer, this->position.x, this->position.y, this->position.x, this->position.y);
}
