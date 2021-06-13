#include "Asteroid.h"
#include <time.h>
#include <iostream>

Asteroid::Asteroid(Point center, Vector speed, double radius, double mass, int width, int height)
	: center(center), speed(speed), acceleration(0, 0), propulsion(0, 0)
{
	this->radius = radius;
	this->mass = mass;
	this->lastUpdate = this->getTimeInNanoSeconds();

	this->width = width;
	this->height = height;
}

void Asteroid::modifyPositionInCase()
{
	if (this->center.x > this->width) {
		this->center.x = 0;
	}
	if (this->center.y > this->height) {
		this->center.y = 0;
	}
	if (this->center.x < 0) {
		this->center.x = this->width;
	}
	if (this->center.y < 0) {
		this->center.y = this->height;
	}
}

void Asteroid::draw(SDL_Renderer* renderer, Color color, SDL_Event& event)
{
	modifyPositionInCase();
	this->center.drawCircle(renderer, this->radius, color, true);
	this->update(event);
}

void Asteroid::update(SDL_Event& event)
{
	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;
	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->center.x = this->center.x + this->speed.x * deltaT;
	this->center.y = this->center.y + this->speed.y * deltaT;

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;

	moveAsteroid(event);

	//	compute sum of forces pallyed on object
	Vector gravity(0, 0 * this->mass);		  //Activate gravity
	Vector propulsion(this->getPropulsion()); //Increase acceleration
	//Vector friction(this->computeFriction()); //Decrease the speed gradually if no acceleration
	Vector sum = gravity /* +propulsion*/ /* + friction*/;

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;


}

Vector& Asteroid::getPropulsion()
{
	return this->propulsion;
}

double& Asteroid::getRadius()
{
	return this->radius;
}

Point& Asteroid::getCenter()
{
	return this->center;
}

void Asteroid::moveAsteroid(SDL_Event& event)
{
	/*Vector randomPropulsion(2000, 200);
	this->setPropulsion(randomPropulsion);*/

	srand((unsigned int)time(NULL));
	//int signX = rand() % 2;
	//int signY = rand() % 2;
	//if (signX == 0) {
	//	signX == -1;
	//	signY == 1;
	//}
	//else {
	//	signX == 1;
	//	signY == -1;
	//}
	//int randomSignSwap = rand() % 2;
	//if (randomSignSwap == 0) {
	//	signX = -signX;
	//}
	//else {
	//	signY = -signY;
	//}

	//Vector randomSpeed(rand() % 2000, rand() % 2000);
	//this->setSpeed(randomSpeed);
}



bool Asteroid::isHitBy(VaisseauObject vaisseau)
{
	double distance = sqrt((vaisseau.getLaserPosition().x - this->center.x)
		* (vaisseau.getLaserPosition().x - this->center.x) +
		(vaisseau.getLaserPosition().y - this->center.y)
		* (vaisseau.getLaserPosition().y - this->center.y));

	return distance <= this->radius;
}

bool Asteroid::Hit(VaisseauObject vaisseau)
{
	double distance = sqrt((this->center.x + this->radius - vaisseau.getPosition().x)
						* (this->center.x + this->radius - vaisseau.getPosition().x) +
							(this->center.y + this->radius - vaisseau.getPosition().y)
						*	(this->center.y + this->radius - vaisseau.getPosition().y));

	return distance <= this->radius;

}


void Asteroid::setPropulsion(Vector newPropulsion)
{
	this->propulsion = newPropulsion;
}

void Asteroid::setSpeed(Vector newSpeed)
{
	this->speed = newSpeed;
}

void Asteroid::decrementRadius(double radius)
{
	this->radius = radius;
}


__time64_t Asteroid::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}