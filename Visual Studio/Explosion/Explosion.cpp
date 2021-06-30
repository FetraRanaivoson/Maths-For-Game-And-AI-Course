#include <time.h>
#include <iostream>
#include "Explosion.h"
using namespace std;

__time64_t Explosion::getTimeInNanoSeconds()
{
	struct _timespec64 ts;
	if (_timespec64_get(&ts, TIME_UTC) == 0) {
		throw 0;
	}

	return ts.tv_sec * 1000000000 + ts.tv_nsec;
	//Nombre de nanosec écoulé depuis le 1er janvier 70
}

Explosion::Explosion(Point position, double épicentre, double lossFactor)
{
	this->lastUpdate = this->getTimeInNanoSeconds();

	this->position = position;
	this->épicentre = épicentre;
	this->lossFactor = lossFactor;
	this->radius = 0;

	this->mass = 1;

	this->speed = 0;
	this->acceleration = 0;
}

void Explosion::draw(SDL_Renderer* renderer, Color color, SDL_Event& event)
{
	this->position.draw(renderer, color, 3);
	this->position.drawCircle(renderer, this->radius, color, true);
	this->position.drawCircle(renderer, 3* this->radius/5, color, true);
	this->position.drawCircle(renderer, 4 *this->radius /5, color, true);
	this->update(event);
}

void Explosion::update(SDL_Event& event)
{
	__time64_t currentUpdate = this->getTimeInNanoSeconds();

	//	compute elapsed time (in seconds) since last position change
	double deltaT = (currentUpdate - this->lastUpdate) / 1e9;
	this->lastUpdate = currentUpdate;

	//	compute new position of the shock wave
	this->radius = this->radius + this->speed * deltaT;

	// compute new speed
	this->speed = this->speed + this->acceleration * deltaT;

	//	compute sum of forces played on the shock wave
	double friction = this->computeFriction();
	double magnitude = this->getMagnitude();
	double sum = magnitude + friction;

	// compute new acceleration of the shock wave
	this->acceleration = sum / this->mass;

	/*if (magnitude == 0) {
		cout << "magnitude  = 0" << endl;
	}*/
}

double Explosion::getMagnitude()
{
				//f =      Yo         *   exp ( - k           *             d²  )
	double magnitude = this->épicentre * exp(-this->lossFactor * this->radius * this->radius);
	cout << magnitude << endl;
	return magnitude;
}

double Explosion::computeFriction()
{
	int frictionCoeff = 1;
	double friction = -frictionCoeff * this->speed;
	return friction;
}
