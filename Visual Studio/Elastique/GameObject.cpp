#include "GameObject.h"
#include <time.h>
#include <iostream>

__time64_t GameObject::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

GameObject::GameObject(double mass, Point positionA, Point positionB, Vector speedA, Vector speedB, int width, int height)
	: positionA(positionA), positionB(positionB), speedA(speedA), speedB(speedB), accelerationA(0, 0), accelerationB(0,0) {
	this->mass = mass;
	this->lastUpdate = this->getTimeInNanoSeconds();
	this->width = width;
	this->height = height;

}

void GameObject::update(SDL_Event& event, double& initialDistanceInX, double& initialDistanceInY) {

	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	this->positionA.update(event);
	/*this->positionB.update(event);*/

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;

	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	/*this->positionA.x = this->positionA.x + this->speedA.x * deltaT;
	this->positionA.y = this->positionA.y + this->speedA.y * deltaT;*/
	this->positionB.x = this->positionB.x + this->speedB.x * deltaT;
	this->positionB.y = this->positionB.y + this->speedB.y * deltaT;
	//	OR this->position = this->position + this->speed * deltaT;		//	using library operators

	//	compute new speed of object now
	/*this->speedA.x = this->speedA.x + this->accelerationA.x * deltaT;
	this->speedA.y = this->speedA.y + this->accelerationA.y * deltaT;*/
	this->speedB.x = this->speedB.x + this->accelerationB.x * deltaT;
	this->speedB.y = this->speedB.y + this->accelerationB.y * deltaT;
	//	OR this->speed = this->speed + this->acceleration * deltaT;		//	using library operators

	//	compute sum of forces pallyed on object
	Vector gravity(0, 0 * this->mass);
	double initialLength = sqrt(initialDistanceInX * initialDistanceInX + initialDistanceInY * initialDistanceInY);
	double distanceAB = this->getABDistance();
	Vector fRappelA(0, 0);

	if (distanceAB > initialLength) {
		fRappelA = Vector(positionB, positionA);
		fRappelA /= distanceAB; //normalize
		double kRappel = 50;
		double norme = kRappel * ( distanceAB - initialLength );
		fRappelA *= norme;
	}

	Vector sum = fRappelA;
	
	// compute new acceleration of object now
	/*if (positionA.isDragged()) {
		this->accelerationB.x = sum.x / mass;
		this->accelerationB.y = sum.y / mass;
	}*/
	//if (positionA.isDragged()) {
		this->accelerationB.x = sum.x / mass;
		this->accelerationB.y = sum.y / mass;
	//}
	
}

Point GameObject::getPositionA() {
	return this->positionA;
}

Point GameObject::getPositionB()
{
	return this->positionB;
}

double GameObject::ABInitialDistance(double& initialDistance)
{
	return initialDistance;
}

double GameObject::getABDistanceInX()
{
	double dx = this->positionA.x - this->positionB.x;
	double distanceInX = sqrt(dx * dx);
	return distanceInX;
}


double GameObject::getABDistanceInY()
{
	double dy = this->positionA.y - this->positionB.y;
	double distanceInY = sqrt(dy * dy);
	return distanceInY;
}

double GameObject::getABDistance()
{
	double dx = this->positionA.x - this->positionB.x;
	double dy = this->positionA.y - this->positionB.y;
	double distance = sqrt(dx * dx + dy * dy );
	return distance;
}


double GameObject::getSpeedX() {
	return this->speedA.x;
}

double GameObject::getSpeedY()
{
	return this->speedA.y;
}

void GameObject::setSpeed(Vector newSpeed) {
	this->speedA = newSpeed;
}

void GameObject::setAcceleration(Vector newAcceleration)
{
	this->accelerationA = newAcceleration;
}

void GameObject::draw(SDL_Renderer* renderer, Color color, SDL_Event& event, double& initialDistanceInX, double& initialDistanceInY) {
	this->update(event, initialDistanceInX, initialDistanceInY);

	//	check borders of screen
	if ((this->positionA.x > width && this->speedA.x > 0 ) ||
		(this->positionA.x < 0 && this->speedA.x < 0 )) {
		this->speedA.x *= -0.9;
	}
	if ((this->positionA.y > height && this->speedA.y > 0) ||
		(this->positionA.y < 0 && this->speedA.y < 0)) {
		this->speedA.y *= -0.9;
	}

	if ((this->positionB.x > width && this->speedB.x > 0) ||
		(this->positionB.x < 0 && this->speedB.x < 0)) {
		this->speedB.x *= -0.9;
	}
	if ((this->positionB.y > height && this->speedB.y > 0) ||
		(this->positionB.y < 0 && this->speedB.y < 0)) {
		this->speedB.y *= -0.9;
	}



	this->positionA.draw(renderer, color, 10);
	this->positionB.draw(renderer, color, 10);
	//this->position.drawCircle(renderer, 10, color, true);

	SDL_RenderDrawLine(renderer, this->positionA.x, this->positionA.y, this->positionB.x, this->positionB.y);

}
