#include "VaisseauObject.h"
#include <time.h>
#include <iostream>

__time64_t VaisseauObject::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

VaisseauObject::VaisseauObject(double mass, Point position, Vector speed, Vector laserSpeed, int width, int height)
	: position(position), speed(speed), laserSpeed(laserSpeed), acceleration(0, 0) {
	this->mass = mass;
	this->lastUpdate = this->getTimeInNanoSeconds();
	this->width = width;
	this->height = height;

}

void VaisseauObject::update(SDL_Event& event) {

	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	this->position.update(event);
	this->laser.update(event);

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;

	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->position.x = this->position.x + this->speed.x * deltaT;
	this->position.y = this->position.y + this->speed.y * deltaT;
	this->laser.x = this->laser.x + this->laserSpeed.x * deltaT;
	this->laser.y = this->laser.y + this->laserSpeed.y * deltaT;

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

Point VaisseauObject::getPosition() {
	return this->position;
}

Vector VaisseauObject::getSpeed() {
	return this->speed;
}


void VaisseauObject::setSpeed(Vector newSpeed) {
	this->speed = newSpeed;
}

void VaisseauObject::setAcceleration(Vector newAcceleration)
{
	this->acceleration = newAcceleration;
}

void VaisseauObject::fireLaser(SDL_Renderer* renderer, Color color, SDL_Event& event)
{
	this->laser.draw(renderer, color, 5);
}

void VaisseauObject::draw(SDL_Renderer* renderer, Color color, SDL_Event& event) {
	this->update(event);

	//	check borders of screen
	if ((this->position.x > width && this->speed.x > 0 ) ||
		(this->position.x < 0 && this->speed.x < 0 )) {
		this->speed.x *= -0.9;
	}
	if ((this->position.y > height && this->speed.y > 0) ||
		(this->position.y < 0 && this->speed.y < 0)) {
		this->speed.y *= -0.9;
	}


	this->position.draw(renderer, color, 50);
	//this->position.drawCircle(renderer, 10, color, true);

	SDL_RenderDrawLine(renderer, this->position.x, this->position.y, this->position.x, this->position.y);

}
