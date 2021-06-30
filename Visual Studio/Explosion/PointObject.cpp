#include "PointObject.h"
#include <time.h>

__time64_t PointObject::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

bool PointObject::isAffectedByExplosion(Explosion explosion)
{
	double distance = sqrt((explosion.getPosition().x - this->position.x)
		* (explosion.getPosition().x - this->position.x) +
		(explosion.getPosition().y - this->position.y)
		* (explosion.getPosition().y - this->position.y));

	return distance <= explosion.getRadius();
}

PointObject::PointObject(Point position, double mass)
	: speed(0,0), acceleration(0,0), friction(0,0), explosionImpact(0,0)
{
	this->position = position;
	this->mass = mass;

	this->lastUpdate = this->getTimeInNanoSeconds();
}

void PointObject::listenForExplosion(Explosion explosion)
{
	if (this->isAffectedByExplosion(explosion)) {
		Vector speed(explosion.getPosition(), this->position);
		speed.normalize();
		//this->setSpeed(speed * explosion.getMagnitude());
		this->setExplosionImpact(speed * explosion.getMagnitude());
	}
}

void PointObject::setSpeed(Vector newSpeed)
{
	this->speed = newSpeed;
}

void PointObject::setExplosionImpact(Vector explosionMagnitude)
{
	this->explosionImpact = explosionMagnitude;
}

Vector PointObject::getExplosionImpact()
{
	return this->explosionImpact;
}

void PointObject::draw(SDL_Renderer* renderer, Color color, SDL_Event& event)
{
	this->position.drawCircle(renderer, this->mass, color, true);
	this->update(event);
}

void PointObject::update(SDL_Event& event)
{
	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;
	this->lastUpdate = currentUpdate;

	//	compute new position of object now
	this->position.x = this->position.x + this->speed.x * deltaT;
	this->position.y = this->position.y + this->speed.y * deltaT;

	//	compute new speed of object now
	this->speed.x = this->speed.x + this->acceleration.x * deltaT;
	this->speed.y = this->speed.y + this->acceleration.y * deltaT;

	//	compute sum of forces played on object
	Vector impact = this->getExplosionImpact();
	Vector friction = this->computeFriction();

	Vector sum = impact + friction;

	// compute new acceleration of object now
	this->acceleration.x = sum.x / mass;
	this->acceleration.y = sum.y / mass;

}

Vector PointObject::computeFriction()
{
	int frictionCoeff = 50;
	this->friction.x = -frictionCoeff * this->speed.x;
	this->friction.y = -frictionCoeff * this->speed.y;

	return this->friction;
}
