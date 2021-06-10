#include "GameObject.h"
#include <time.h>

__time64_t GameObject::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}
	return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

GameObject::GameObject(double mass, Point position, Vector speed):positionA(position),speed(speed), acceleration(0,0)
{
	//this->update();
	this->mass = mass;
	this->lastUpdate = getTimeInNanoSeconds();

}

Point GameObject::getPosition()
{
	return positionA;
}

Vector GameObject::getSpeed()
{
	return this->speed;
}

void GameObject::setSpeed(Vector newSpeed)
{
	this->speed = speed;
}

void GameObject::update()
{
	this->lastUpdate = getTimeInNanoSeconds();

	__time64_t currentUpdate = getTimeInNanoSeconds();

	double deltaT = (currentUpdate - this->lastUpdate)/1e9;

	this->lastUpdate = currentUpdate;

	// compute new position of object now
	positionA.x = positionA.x + speed.x * deltaT;
	positionA.y = positionA.y + speed.y * deltaT;


	//compute new speed of object
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y= this->speed.y + this->acceleration.y * deltaT;
	// OR position = position + speed * deltaT; //using library operators



	//Compute new acceleration		//m*g			//Le vent qui souffle à droite
	Vector force = Vector(0, 9.81 * this->mass) + Vector (-50,0); 
	this->acceleration.x = force.x / mass;
	this->acceleration.y = force.y / mass;



}

void GameObject::draw(SDL_Renderer* renderer, Color color)
{
	this->positionA.draw(renderer, color, 5);
}
